#ifndef __INCvxbI2cTpicLedh
#define __INCvxbI2cTpicLedh 

#ifdef __cplusplus
extern "C" {
#endif

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>

struct led
    {
    UINT8 val;
    };

#define LED_WRITE_CMD	0x44
#define LED_READ_CMD	0x11
#define CMD_LENGTH	1
#define DEFAULT_WRTIME  2
#define TPIC_LED_NAME            "tpic-led"

typedef struct vxbLedFunctionality
    {
    char   ledName[MAX_DRV_NAME_LEN + 1];

    STATUS (*ledRead) (VXB_DEV_ID pInst, UINT32 addr, UINT8 *buf, UINT32 len,
                      UINT32* realByte);
    STATUS (*ledWrite)(VXB_DEV_ID pInst, UINT32 addr, UINT8 *buf, UINT32 len,
                      UINT32* realByte);

    } VXB_I2C_LED_FUNC;

struct vxbLedHandle
    {
    struct vxbLedFunctionality *pLedFunc;
    VXB_DEV_ID                  pInst;
    };

/* IMPORT */

/*
IMPORT STATUS vxbI2cLedWrite (VXB_DEV_ID pInst, UINT32 addr, UINT8 *buf, UINT32 len,
                      UINT32* realByte);
IMPORT STATUS vxbI2cLedRead (VXB_DEV_ID pInst, UINT32 addr, UINT8 *buf, UINT32 len,
                      UINT32* realByte);
*/
IMPORT UINT32 ledDrvNextUnit ();
IMPORT STATUS ledDevCreate (char * name, VXB_DEV_ID pDev, VXB_I2C_LED_FUNC * pCandidate);
IMPORT STATUS ledDevRemove (char * name);

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbI2cTpicLedh */
