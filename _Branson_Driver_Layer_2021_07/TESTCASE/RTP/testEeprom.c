
#include <ioLib.h>
#include <stdio.h>
#include <taskLib.h>

extern void *calloc(size_t, size_t);
extern int isprint(int);

#define EEPROM_DEV_NAME             "/eeprom/0"
#define EEPROM_DATA_START_ADDR      0
#define EEPROM_DATA_SIZE            256

static void testBufDump(UINT8 *buf, UINT32 len)
    {
    UINT32  i;
    UINT32  j;
    int     retLen;
    int     unalign;

    unalign = len & 0xF;
    retLen  = len - unalign;
    for(i = 0; i < retLen; i += 16)
        {
        printf("    %03u:", i);
        for(j = 0; j < 16; j++)
            {
            printf(" %02x", buf[i + j]);
            }
        printf("    | ");
        for(j = 0; j < 16; j++)
            {
            if(isprint(buf[i + j]))
                {
                printf("%c", buf[i + j]);
                }
            else
                {
                printf(".");
                }
            }
        printf(" |\n");
        }

    if(unalign == 0)
        {
        return;
        }

    printf("    %03u:", i);
    for(j = 0; j < unalign; j++)
        {
        printf(" %02x", buf[i + j]);
        }
    for(j = unalign; j < 16; j++)
        {
        printf("   ");
        }
    printf("    | ");
    for(j = 0; j < unalign; j++)
        {
        if(isprint(buf[i + j]))
            {
            printf("%c", buf[i + j]);
            }
        else
            {
            printf(".");
            }
        }
    for(j = unalign; j < 16; j++)
        {
        printf(".");
        }

    printf(" |\n");
    }

/*******************************************************************************
 * eepromIoRead - read EEPROM
 *
 * The function reads 'length' bytes into 'pRdBuf' from 'devName', starting from 'startAddr'.
 *
 * RETURN - OK or ERROR
 *
 * ERRNO - NA
 */

static STATUS eepromIoRead(char *devName, UINT32 startAddr, UINT32 length, void *pRdBuf)
    {
    int fd;
    UINT32 curAddr;

    /* open the EEPROM */
    if((fd = open(devName, O_RDONLY, 0)) == ERROR)
        {
        printf("open\n");
        return ERROR;
        }

    /* redirection */
    if(ioctl(fd, FIOSEEK, startAddr) == ERROR)
        {
        printf("FIOSEEK\n");
        close(fd);
        return ERROR;
        }

    /*get current position before read*/
    curAddr = ioctl(fd, FIOWHERE, 0);
    printf("location before read: 0x%x\n", curAddr);

    /* read the data */
    if(read(fd, pRdBuf, length) == ERROR)
        {
        printf("read\n");
        close(fd);
        return ERROR;
        }

    curAddr = ioctl(fd, FIOWHERE, 0);
    printf("location after read: 0x%x\n", curAddr);

    close(fd);
    return OK;
    }

/*******************************************************************************
 * eepromIoWrite - write EEPROM
 *
 * The function writes 'length' bytes, saved in 'pWrBuf', into 'devName', starting from 'startAddr'.
 *
 * RETURN - OK or ERROR
 *
 * ERRNO - NA
 */

static STATUS eepromIoWrite(char *devName, UINT32 startAddr, UINT32 length, void *pWrBuf)
    {
    int fd;
    UINT32 curAddr;

    /* open the EEPROM */
    if((fd = open(devName, O_WRONLY, 0)) == ERROR)
        {
        printf("open\n");
        return ERROR;
        }

    /* redirection */
    if(ioctl(fd, FIOSEEK, startAddr) == ERROR)
        {
        printf("FIOSEEK");
        close(fd);
        return ERROR;
        }

    curAddr = ioctl(fd, FIOWHERE, 0);
    printf("location before write: 0x%x\n", curAddr);

    /* write data */
    if(write(fd, pWrBuf, length) == ERROR)
        {
        printf("write\n");
        close(fd);
        return ERROR;
        }

    /*get current position after write*/
    curAddr = ioctl(fd, FIOWHERE, 0);
    printf("location after write: 0x%x\n", curAddr);

    close(fd);
    return OK;
    }

void testEeprom()
    {
    int   i;
    UINT8 *wrData = NULL;
    UINT8 *rdData = NULL;
    UINT8 *origData = NULL;

    printf("### %s ###\n", __FUNCTION__);
    printf("device name: %s\n", EEPROM_DEV_NAME);
    printf("data size: %d\n", EEPROM_DATA_SIZE);
    wrData = (UINT8 *)calloc(1, 3 * EEPROM_DATA_SIZE);
    if(wrData == NULL)
        {
        printf("failed to allocate memory\n");
        return;
        }
    rdData   = wrData + EEPROM_DATA_SIZE;
    origData = rdData + EEPROM_DATA_SIZE;

    printf("reading and saving original data...\n");
    if(eepromIoRead(EEPROM_DEV_NAME, EEPROM_DATA_START_ADDR, EEPROM_DATA_SIZE, (void *)origData) != OK)
        {
        printf("failed to read original data\n");
        return;
        }
    printf("original data:\n");
    testBufDump(origData, EEPROM_DATA_SIZE);

    for (i = 0; i < EEPROM_DATA_SIZE; i++)
        {
        wrData[i] = 'A' + (i % 26);
        }
    printf("writing data...\n");
    testBufDump(wrData, EEPROM_DATA_SIZE);
    if(eepromIoWrite(EEPROM_DEV_NAME, EEPROM_DATA_START_ADDR, EEPROM_DATA_SIZE, (void *)wrData) != OK)
        {
        printf("failed to write data\n");
        return;
        }
    taskDelay(20);
    printf("reading data...\n");
    if(eepromIoRead(EEPROM_DEV_NAME, EEPROM_DATA_START_ADDR, EEPROM_DATA_SIZE, (void *)rdData) != OK)
        {
        printf("failed to read data\n");
        return;
        }
    testBufDump(rdData, EEPROM_DATA_SIZE);
    printf("checking data...\n");
    for(i = 0; i < EEPROM_DATA_SIZE; i++)
        {
        if(wrData[i] != rdData[i])
            {
            printf("error @ %d\n", i);
            break;
            }
        }
    printf("restoring original data...\n");
    if(eepromIoWrite(EEPROM_DEV_NAME, EEPROM_DATA_START_ADDR, EEPROM_DATA_SIZE, (void *)origData))
        {
        printf("failed to write original data back\n");
        }
    }

