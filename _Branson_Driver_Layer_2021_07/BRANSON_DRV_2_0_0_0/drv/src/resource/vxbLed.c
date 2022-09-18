/* vxbLed.c - LED device library*/

/*
 * Copyright (c) 2013-2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
25mar16,zly  missing semGive() (V7STO-607)
22dec15,y_y  fix unsupported open flags. (V7RAD-2611)
12jun15,p_x  fix file pointer issue. (V7RAD-120)
22jan15,d_l  fix gnu warnings.
24nov14,xms  fix ioctl FIOSEEK issue. (V7PRO-1333)
18aug14,l_z  fix static analysis issue. (V7PRO-1181)
12may14,l_z  remove unused variable. (V7PRO-909)
22aug13,y_y  created
*/

/*
DESCRIPTION
The logical layers of a typical system using a LED appear:

/cs
         +--------------------+
         | Application module |
         +--------------------+ <-- read(), write(), ioctl(), close()
                   |
         +--------------------+
         | VxWorks I/O System |
         +--------------------+ <-- IOS layer iosRead, iosWrite, ioctl...(vxbLed.c)
                   |
+--------------------------------------+
| Diff interface LED driver module  |
+--------------------------------------+ <-- vxbI2cLed.c, vxbSpiEeprom.c,
                   |                         vxbPallEeprom.c...
+--------------------------------------+
| Diff interface Bus driver module     |
+--------------------------------------+ <-- vxbI2cLib.c, vxbSpiLib.c...
                   |
+-----------------------------------------+
| Diff interface Controller driver module |
+-----------------------------------------+ <-- eg: vxbAm38xxI2c.c, vxbFslI2c.c, vxbFslEspi.c...

/cs

This driver allows the I/O system to access led directly as an I/O device.

vxbLedDrv can be used to simply provide a high-level method of reading
and writing bytes in absolute led locations through I/O calls.

USER-CALLABLE ROUTINES
Most of the routines in this driver are accessible only through the
I/O system. However, the following two routines can be called directly:
ledDrv() to initialize the driver, and ledDevRemove("name") to delete
devices.

Before using the driver, it must be initialized by calling ledDrv().
This routine should be called only once, before any reads, writes, or
ledDrvLoad() calls.

The open() function establishes the connection between a led and a file descriptor.
The file descriptor is used by other I/O functions to refer to that led. The file 
access modes of the open file description will be set according to the value of flag.

Values for flag are constructed by a bitwise-inclusive-OR of flags from the following list,
defined in <fcntlcom.h>. 

Applications must specify exactly one of the first three values (file access modes) 
below in the value of flag:

O_RDONLY
Open for reading only.
O_WRONLY
Open for writing only.
O_RDWR
Open for reading and writing. 

Any combination of the following could not be supported when access led:
O_APPEND
O_CREAT
O_DSYNC
O_EXCL
O_NOCTTY
O_NONBLOCK
O_RSYNC
O_TRUNC

CONFIGURATION
To use the led device driver, configure VxWorks with the
INCLUDE_LEDDRV component.

This will result in the creation of the "/led/0" I/O device for led,
as shown by the output of the 'devs' target shell command:
\cs
-> devs
drv name
  0 /null
  1 /tyCo/0
  1 /tyCo/1
  1 /tyCo/2
  1 /tyCo/3
  4 /tffs0
  2 /eeprom/0
  3 /led/0
  7 host:
value = 0 = 0x0
\ce

SEE ALSO:
the VxWorks programmer guides.

*/

/* includes */

#include <vxWorks.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <logLib.h>
#include <lstLib.h>
#include <iosLib.h>
#include <hwif/vxBus.h>
#include <vxbI2cTpicLed.h>
/* defines */
/* typedefs */

typedef struct
    {
    DEV_HDR    devHdr;        /* device header */
    VXB_DEV_ID devID;         /* device id */
    char       name[MAX_DRV_NAME_LEN];

    STATUS     (*ledRead) (VXB_DEV_ID, UINT32, char*, UINT32, UINT32*);
    STATUS     (*ledWrite)(VXB_DEV_ID, UINT32, char*, UINT32, UINT32*);
    } LED_DEV;

/* led device file descriptor */

typedef struct      /* LED_FILE_DESC */
    {
    NODE         fdNode;       /* linked list node info */
    SEM_ID       fdSemId;      /* semaphore for this file descriptor */
    int          fdStatus;     /* (OK | NOT_IN_USE) */
    int          fdMode;       /* mode: O_RDONLY, O_WRONLY, O_RDWR */
    int          fdCurtPtr;    /* file byte ptr for new read/writes */
    LED_DEV * pDev;
    } LED_FILE_DESC;

/* File descriptor status values */

#define FD_AVAILABLE      (-1)    /* file descriptor available */
#define FD_IN_USE         0       /* file descriptor in-use */

/* max files open at once */

#define LED_DEF_MAX_FILES  1

/* unsupported flag*/

#define LED_UNSUPPORT_FLAG  O_APPEND | O_NONBLOCK | O_TRUNC | O_CREAT | \
                               O_EXCL | O_DSYNC | O_RSYNC | O_NOCTTY

/* LOCALS */

LOCAL int ledDrvNum = ERROR;   /* driver number assigned to this driver */

LOCAL LIST ledFdActiveList;    /* linked list of in-use Fd's */
LOCAL LIST ledFdFreeList;      /* linked list of avail. Fd's */
LOCAL SEM_ID ledFdListSemId;   /* file descr list semaphore  */

/* forward declarations */

LOCAL LED_FILE_DESC * ledDrvOpen (LED_DEV * pLedDev, char * name, int flags);
LOCAL ssize_t ledDrvRead (LED_FILE_DESC * pFd, char * buffer, size_t bytes);
LOCAL ssize_t ledDrvWrite (LED_FILE_DESC * pFd, char * buffer, size_t bytes);
LOCAL STATUS ledDrvClose (LED_FILE_DESC * pFd);
LOCAL LED_FILE_DESC * ledFdGet (void);
LOCAL void ledFdFree (LED_FILE_DESC * pFd);

struct led_info
    {
    char   name[MAX_DRV_NAME_LEN];
    };


/* supported devices list */

LOCAL const struct led_info ledTable[] = {

	{ "ti,tpic2810" }
};

/*******************************************************************************
*
* ledDrv - install a led driver
*
* This routine initializes the led driver. It is called automatically
* when VxWorks is configured with the INCLUDE_LEDDRV component.
*
* It must be called exactly once, before any other routine in the library.
* The maximum number of file descriptors that may be open at once are MAXFILES.
* This routine allocates and sets up the necessary memory structures and
* initializes semaphores.
*
* This routine also installs led library routines in the VxWorks I/O
* system driver table.  The driver number assigned to led device is placed in
* the global variable "ledDrvNum".  This number will later be associated
* with system file descriptors opened to led devices.
*
* RETURNS: OK, or ERROR if the I/O system cannot install the driver.
*
* ERRNO: N/A.
*/

STATUS ledDrv(void)
    {
    LED_FILE_DESC * pLedFd; /* pointer to created file descriptor */
    int ix;                       /* index var */

    /* Check if driver already installed */

    if (ledDrvNum > 0)
        {
        return (OK);
        }

    ledDrvNum = iosDrvInstall ((DRV_CREATE_PTR) ledDrvOpen,
                                  (DRV_REMOVE_PTR) NULL,
                                  (DRV_OPEN_PTR)   ledDrvOpen,
                                  (DRV_CLOSE_PTR)  ledDrvClose,
                                  (DRV_READ_PTR)   ledDrvRead,
                                  (DRV_WRITE_PTR)  ledDrvWrite,
								  (DRV_IOCTL_PTR)  NULL);

    if (ledDrvNum <= 0)
        {
        return (ERROR);
        }

    /* Create semaphore for locking access to file descriptor list */

    ledFdListSemId = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE);

    /* Can't create semaphore */

    if (ledFdListSemId == NULL)
        {
        return (ERROR);
        }

    /* Take control of fd list */

    if (semTake (ledFdListSemId, WAIT_FOREVER) != OK)
        return ERROR;

    /* Allocate memory for required number of file descriptors */

    pLedFd = (LED_FILE_DESC *)
                malloc (LED_DEF_MAX_FILES * sizeof (LED_FILE_DESC));

    if (NULL == pLedFd)
        {
        (void) semGive (ledFdListSemId);
        return (ERROR);
        }

    bzero ((char *) pLedFd, (LED_DEF_MAX_FILES * sizeof (LED_FILE_DESC)));

    for (ix = 0; ix < LED_DEF_MAX_FILES; ix++)
        {
        pLedFd->fdStatus = FD_AVAILABLE;

        /* Create semaphore for this fd (initially available) */

        pLedFd->fdSemId = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE);

        if (pLedFd->fdSemId == NULL)
            {
            free (pLedFd);
            (void) semGive (ledFdListSemId);
            return (ERROR);     /* Could not create semaphore */
            }

        /* Add file descriptor to free list */

        lstAdd (&ledFdFreeList, &pLedFd->fdNode);

        /* Next Fd */

        pLedFd++;
        }

    /* Release Fd lists */

    (void) semGive (ledFdListSemId);

    return OK;
    }

/*******************************************************************************
*
* ledDrvNextUnit - assign the unused unit number
*
* RETURN - OK or ERROR
*
* ERRNO - NA
*/

UINT32 ledDrvNextUnit()
    {
    static int ledDrvNextNumber = 0;

    return(ledDrvNextNumber++);
    }

/*******************************************************************************
*
* ledDrvLoad - retrieve the led info and create device
*
* RETURN - OK or ERROR
*
* ERRNO - NA
*/

STATUS ledDevCreate
    (
    char *            name,   /* name to use for this device */
    VXB_DEV_ID        pDev,
    VXB_I2C_LED_FUNC * pCandidate
    )
    {
    LED_DEV * pLedDev = NULL;

    /* Check for the validity of the function parameters */

    pLedDev = malloc (sizeof (LED_DEV));

    VXB_ASSERT (pLedDev != NULL, ERROR);

    bzero ((char *) pLedDev, sizeof (LED_DEV));

    /* Retrieve the led device name */

    strncpy (pLedDev->name, pCandidate->ledName, MAX_DRV_NAME_LEN - 1);

    /* Retrieve the led device read/write functions */

    pLedDev->ledWrite = (void *) pCandidate->ledWrite;
    pLedDev->ledRead = (void *) pCandidate->ledRead;

    pLedDev->devID = pDev;

    /* Add the device to the I/O systems device list */

    if (iosDevAdd ((DEV_HDR *) & (pLedDev->devHdr), name, ledDrvNum) != OK)
        {
        free (pLedDev);
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* ledDrvOpen - open led device
*
* This routine is called to open a led.  It returns a pointer to the
* device.  This routine is normally reached only via the I/O system.
*
* RETURNS: The opaque LED_FILE_DESC ptr. For this driver it is really a
* pointer to a LED_FILE_DESC structure.
*
* ERRNO: N/A
*/

LOCAL LED_FILE_DESC * ledDrvOpen
    (
    LED_DEV * pLedDev,
    char *       name,
    int          flags
    )
    {
    LED_FILE_DESC * pFd;     /* file descriptor pointer */

    /* Make sure it's a valid led id */

    if (pLedDev == NULL)
        {
        return((LED_FILE_DESC *) ERROR);
        }

    /* Non-null filename */

    if (name[0] != EOS)
        {
        return ((LED_FILE_DESC *) ERROR);
        }

    /* Check for invalid mode */

    flags &= ~(LED_UNSUPPORT_FLAG); 

    if ((flags != O_RDONLY) && (flags != O_WRONLY) && (flags != O_RDWR))
        {
        return ((LED_FILE_DESC *) ERROR);
        }

    /* Get a free file descriptor */

    if ((pFd = ledFdGet ()) == NULL)
        {
        return ((LED_FILE_DESC *) ERROR);
        }

    /* Take control of fd */

    if (semTake (pFd->fdSemId, WAIT_FOREVER) != OK)
        {
        return ((LED_FILE_DESC *) ERROR);
        }

    /* Save the flags */

    pFd->fdMode = flags;
    pFd->pDev = (LED_DEV *) pLedDev;

    /* Set the location to 0 */

    pFd->fdCurtPtr = 0;

    /* Release fd */

    (void) semGive (pFd->fdSemId);

    return (pFd);
    }



/*******************************************************************************
*
* ledDrvRead - read bytes from led device
*
* Perform a read operation on an open led file.
*
* RETURNS: The Number of bytes actually read
*
* ERRNO - NA
*/

LOCAL ssize_t ledDrvRead
    (
    LED_FILE_DESC * pFd,
    char *             buffer,     /* buffer to read */
    size_t             bytes       /* maximum length of read */
    )
    {
    LED_DEV * pVd;
    UINT32 readByte = 0;

    /* Make sure it's a valid id */

    if (pFd == NULL || (pVd = pFd->pDev) == NULL)
        {
        return ERROR;
        }

    /* Make sure the file isn't opened in write-only mode */

    if ((pFd->fdMode & O_ACCMODE) == O_WRONLY)
        {
        return (ERROR);
        }

    /* Take control of fd */

    if (semTake (pFd->fdSemId, WAIT_FOREVER) != OK)
        {
        return (ERROR);
        }

	if (bytes > 1)
		{
		bytes = 1;
		}
		
    if (pVd->ledRead (pVd->devID,
                         pFd->fdCurtPtr,
                         buffer,
                         (UINT32) bytes,
                         &readByte) != OK)
        {
    	
        (void) semGive (pFd->fdSemId);
        return ERROR;
        }

    pFd->fdCurtPtr += readByte;

    (void) semGive (pFd->fdSemId);

    return (ssize_t) readByte;
    }

/*******************************************************************************
*
* ledDrvWrite - write led device
*
* Perform a write operation to an open led file.
*
* RETURNS: The number of bytes written
*
* ERRNO - NA
*/

LOCAL ssize_t ledDrvWrite
    (
    LED_FILE_DESC * pFd,
    char *             buffer,     /* buffer to write */
    size_t             bytes       /* maximum length of read */
    )
    {
    LED_DEV * pVd;
    UINT32 writeByte = 0;

    /* Make sure it's a valid led id */

    if (pFd == NULL || (pVd = pFd->pDev) == NULL)
        {
        return ERROR;
        }

    /* Make sure the file isn't opened in read-only mode */

    if ((pFd->fdMode & O_ACCMODE) == O_RDONLY)
        {
        return (ERROR);
        }

    /* Take control of fd */

    if (semTake (pFd->fdSemId, WAIT_FOREVER) != OK)
        {
        return (ERROR);
        }


    if (pVd->ledWrite (pVd->devID,
                          pFd->fdCurtPtr,
                          buffer,
                          (UINT32) bytes,
                          &writeByte) != OK)
        {
        (void) semGive (pFd->fdSemId);
        return ERROR;
        }

    pFd->fdCurtPtr += writeByte;

    (void) semGive (pFd->fdSemId);
    
    return (ssize_t) writeByte;
    }

/*******************************************************************************
*
* ledDrvClose - close a led device
*
* This routine is called to close a led.  This routine is normally reached
* only via the I/O system.
*
* RETURNS:  OK or ERROR if NULL led device pointer.
*
* ERRNO: N/A.
*/

LOCAL STATUS ledDrvClose
    (
    LED_FILE_DESC * pLedFd
    )
    {
    /* Take control of file descriptor */

    if (semTake (pLedFd->fdSemId, WAIT_FOREVER) != OK)
        {
        return (ERROR);
        }

    /* Put fd on free list */

    ledFdFree (pLedFd);

    /* Release fd */

    (void) semGive (pLedFd->fdSemId);

    return (OK);
    }

/*******************************************************************************
*
* ledDevRemove - remove a led device
*
* This routine deletes a led device. The device is deleted with it own name.
*
* For example, to delete the device, the proper call would be:
* \cs
*   ledDevRemove ("/led/0");
* \ce
*
* RETURNS: OK, or ERROR if the device doesn't exist.
*
* ERRNO: N/A.
*/

STATUS ledDevRemove
    (
    char * name           /* device name */
    )
    {
    DEV_HDR * pDevHdr;

    /* Get the device pointer corresponding to the device name */

    if ((pDevHdr = iosDevFind (name, NULL)) == NULL)
        {
        return (ERROR);
        }

    /* Delete the device from the I/O system */

    (void) iosDevDelete (pDevHdr);

    /* Free the device pointer */

    free ((LED_DEV *) pDevHdr);

    return (OK);
    }

/*******************************************************************************
*
* ledFdGet - get an available file descriptor
*
* This routine obtains a free led file descriptor.
*
* RETURNS: Pointer to file descriptor, or NULL if none available.
*
* ERRNO - NA
*/

LOCAL LED_FILE_DESC * ledFdGet
    (
    void
    )
    {
    LED_FILE_DESC * pFd;

    /* Take control of Fd lists */

    if (semTake (ledFdListSemId, WAIT_FOREVER) != OK)
        return NULL;

    /* Get a free Fd */

    pFd = (LED_FILE_DESC *) lstGet (&ledFdFreeList);

    if (pFd != NULL)
        {
        /* Mark Fd as in-use */
        pFd->fdStatus = FD_IN_USE;

        /* Add to active list */
        lstAdd (&ledFdActiveList, (NODE *) pFd);
        }

    /* Release Fd lists */

    (void) semGive (ledFdListSemId);

    return (pFd);
    }

/*******************************************************************************
*
* ledFdFree - free a file descriptor
*
* This routine removes a led device file descriptor from the active
* Fd list and places it on the free Fd list.
*
* RETURN - NA
*
* ERRNO - NA
*/

LOCAL void ledFdFree
    (
    LED_FILE_DESC * pLedFd      /* pointer to file descriptor to free */
    )
    {
    /* Take control of Fd lists */

    if (semTake (ledFdListSemId, WAIT_FOREVER) != OK)
        return;

    pLedFd->fdStatus = FD_AVAILABLE;

    /* Remove Fd from active list */

    lstDelete (&ledFdActiveList, &pLedFd->fdNode);

    /* Add Fd to free list */

    lstAdd (&ledFdFreeList, &pLedFd->fdNode);

    /* Release Fd lists */

    (void) semGive (ledFdListSemId);
    }

