/*
 * PRU-ICSS EtherCAT Slave Controller driver for AM437x IDK
 *
 * Copyright information - To be Done.
 *
 */


#ifndef _PRUSS_H_
#define _PRUSS_H_

/* number of PRUs within a PRUSS */
#ifndef PRUSS_NUM_PRUS
#define PRUSS_NUM_PRUS      2
#endif

/* maximum number of system events */
#define MAX_PRU_SYS_EVENTS  64

/* maximum number of interrupt channels */
#define MAX_PRU_CHANNELS    10

/* minimum starting host interrupt number for MPU */
#define MIN_PRU_HOST_INT    2

/* maximum number of host interrupts */
#define MAX_PRU_HOST_INT    10

/* PRU_ICSS_INTC registers */
#define PRU_INTC_REVID      0x0000
#define PRU_INTC_CR     0x0004
#define PRU_INTC_GER        0x0010
#define PRU_INTC_GNLR       0x001C
#define PRU_INTC_SISR       0x0020
#define PRU_INTC_SICR       0x0024
#define PRU_INTC_EISR       0x0028
#define PRU_INTC_EICR       0x002C
#define PRU_INTC_HIEISR     0x0034
#define PRU_INTC_HIDISR     0x0038
#define PRU_INTC_GPIR       0x0080
#define PRU_INTC_SRSR0      0x0200
#define PRU_INTC_SRSR1      0x0204
#define PRU_INTC_SECR0      0x0280
#define PRU_INTC_SECR1      0x0284
#define PRU_INTC_ESR0       0x0300
#define PRU_INTC_ESR1       0x0304
#define PRU_INTC_ECR0       0x0380
#define PRU_INTC_ECR1       0x0384
#define PRU_INTC_CMR(x)     (0x0400 + (x) * 4)
#define PRU_INTC_HMR(x)     (0x0800 + (x) * 4)
#define PRU_INTC_HIPIR(x)   (0x0900 + (x) * 4)
#define PRU_INTC_SIPR0      0x0D00
#define PRU_INTC_SIPR1      0x0D04
#define PRU_INTC_SITR0      0x0D80
#define PRU_INTC_SITR1      0x0D84
#define PRU_INTC_HINLR(x)   (0x1100 + (x) * 4)
#define PRU_INTC_HIER       0x1500

/* HIPIR register bit-fields */
#define INTC_HIPIR_NONE_HINT    0x80000000

/*
 * PRU_ICSS_CFG registers
 * SYSCFG, ISRP, ISP, IESP, IECP, SCRP applicable on AMxxxx devices only
 */
#define PRUSS_CFG_REVID     0x00
#define PRUSS_CFG_SYSCFG    0x04
#define PRUSS_CFG_GPCFG0    0x08
#define PRUSS_CFG_GPCFG1    0x0C
#define PRUSS_CFG_CGR       0x10
#define PRUSS_CFG_ISRP      0x14
#define PRUSS_CFG_ISP       0x18
#define PRUSS_CFG_IESP      0x1C
#define PRUSS_CFG_IECP      0x20
#define PRUSS_CFG_SCRP      0x24
#define PRUSS_CFG_PMAO      0x28
#define PRUSS_CFG_MII_RT    0x2C
#define PRUSS_CFG_IEPCLK    0x30
#define PRUSS_CFG_SPP       0x34
#define PRUSS_CFG_PIN_MX    0x40

/* PRUSS_SYSCFG register bits */
#define PRUSS_SYSCFG_SUB_MWAIT_READY            BIT(5)
#define PRUSS_SYSCFG_STANDBY_INIT               BIT(4)

#define PRUSS_SYSCFG_STANDBY_MODE_FORCE         (0 << 2)
#define PRUSS_SYSCFG_STANDBY_MODE_NO            (1 << 2)
#define PRUSS_SYSCFG_STANDBY_MODE_SMART         (2 << 2)
#define PRUSS_SYSCFG_STANDBY_MODE_MASK          (3 << 2)

#define PRUSS_SYSCFG_IDLE_MODE_FORCE            0
#define PRUSS_SYSCFG_IDLE_MODE_NO               1
#define PRUSS_SYSCFG_IDLE_MODE_SMART            2
#define PRUSS_SYSCFG_IDLE_MODE_MASK             3

/* PRUSS_GPCFG register bits */
#define PRUSS_GPCFG_PRU_GPO_SH_SEL              BIT(25)

#define PRUSS_GPCFG_PRU_DIV1_SHIFT              20
#define PRUSS_GPCFG_PRU_DIV1_MASK               GENMASK(24, 20)

#define PRUSS_GPCFG_PRU_DIV0_SHIFT              15
#define PRUSS_GPCFG_PRU_DIV0_MASK               GENMASK(15, 19)

#define PRUSS_GPCFG_PRU_GPO_MODE                BIT(14)
#define PRUSS_GPCFG_PRU_GPO_MODE_DIRECT         0
#define PRUSS_GPCFG_PRU_GPO_MODE_SERIAL         BIT(14)

#define PRUSS_GPCFG_PRU_GPI_SB                  BIT(13)

#define PRUSS_GPCFG_PRU_GPI_DIV1_SHIFT          8
#define PRUSS_GPCFG_PRU_GPI_DIV1_MASK           GENMASK(12, 8)

#define PRUSS_GPCFG_PRU_GPI_DIV0_SHIFT          3
#define PRUSS_GPCFG_PRU_GPI_DIV0_MASK           GENMASK(7, 3)

#define PRUSS_GPCFG_PRU_GPI_CLK_MODE_POSITIVE   0
#define PRUSS_GPCFG_PRU_GPI_CLK_MODE_NEGATIVE   BIT(2)
#define PRUSS_GPCFG_PRU_GPI_CLK_MODE            BIT(2)

#define PRUSS_GPCFG_PRU_GPI_MODE_MASK           GENMASK(1, 0)
#define PRUSS_GPCFG_PRU_GPI_MODE_SHIFT          0

#define PRUSS_GPCFG_PRU_MUX_SEL_SHIFT           26
#define PRUSS_GPCFG_PRU_MUX_SEL_MASK            GENMASK(29, 26)

/* PRUSS_MII_RT register bits */
#define PRUSS_MII_RT_EVENT_EN                   BIT(0)

/* PRUSS_SPP register bits */
#define PRUSS_SPP_XFER_SHIFT_EN                 BIT(1)
#define PRUSS_SPP_PRU1_PAD_HP_EN                BIT(0)


/* defines */
#define PRCM_ASRT_RST_CTRL  0x2
#define PRCM_CLR_RST_CTRL   0xFFFFFFFD

#define PRUICSS_CFG_SYSCFG_STANDBY_INIT_SHIFT   (4)
#define PRUICSS_CFG_SYSCFG_STANDBY_INIT_MASK    (0x00000010)
#define PRUICSS_CFG_SYSCFG                      (0x4)

/* debug macro */
#undef PRUESC_DEBUG
#ifdef PRUESC_DEBUG
#include <private/kwriteLibP.h>         /* _func_kprintf */

#define DBG_OFF             0x00000000
#define DBG_WARN            0x00000001
#define DBG_ERR             0x00000002
#define DBG_INFO            0x00000004
#define DBG_ALL             0xffffffff
LOCAL uint32_t dbgMask    = DBG_ALL;

#undef DBG_MSG
#define DBG_MSG(mask,...)                                      \
do                                                             \
{                                                              \
    if ((dbgMask & (mask)) || ((mask) == DBG_ALL))            \
        {                                                      \
        if (_func_kprintf != NULL)                             \
            {                                                  \
            (* _func_kprintf)("%s,%d, ",__FUNCTION__,__LINE__);\
            (* _func_kprintf)(__VA_ARGS__);                    \
            }                                                  \
        }                                                      \
}while (0)
#else
#define DBG_MSG(...)
#endif  /* PRUESC_DEBUG */

/* Structure for PRU ESC child Devices: PRUICSS 0/1 */
typedef struct pruEscDevInfo
    {
    struct vxbFdtDev   vxbFdtDev;
    VXB_RESOURCE_LIST  vxbResList;
    } VXB_PRU_ESC_INFO;

/* Structure for PRU ESC Device */
typedef struct pruEsc
    {
    VXB_DEV_ID  pDev;           /* device information               */
    void *      pClkRegbase;    /* Clock Register base address      */
    void *      pClkHandle;     /* Clock Register Handle            */
    void *      pCtrlRegbase;   /* Control Register base address    */
    void *      pCtrlHandle;    /* Control Register Handle          */
    } PRU_ESC_CTRL;

/* Pointer to PRU ESC Device Structure  */
LOCAL PRU_ESC_CTRL      * pPruEscCtrl   = NULL;


/* 4 Byte READ/WRITE MACROS */
#define PRUICSS_BAR(p)          (((PRU_ESC_CTRL *) vxbDevSoftcGet(p))->pClkRegbase)
#define PRUICSS_HANDLE(p)       (((PRU_ESC_CTRL *) vxbDevSoftcGet(p))->pClkHandle )

#undef CSR_READ_4
#define CSR_READ_4(pDev, addr)                                                               \
                                vxbRead32 (PRUICSS_HANDLE(pDev),                            \
                                    (uint32_t *)((char *)PRUICSS_BAR(pDev) + addr))

#undef CSR_WRITE_4
#define CSR_WRITE_4(pDev, addr, data)                                                       \
                                vxbWrite32 (PRUICSS_HANDLE(pDev),                          \
                                    (uint32_t *)((char *)PRUICSS_BAR(pDev) + addr), data)


LOCAL STATUS            pruEscProbe           (VXB_DEV_ID);
LOCAL STATUS            pruEscAttach          (VXB_DEV_ID);
LOCAL STATUS            pruEscChildCreate     (VXB_DEV_ID,
                                                        VXB_FDT_DEV *
                                                        );
LOCAL STATUS            pruEscOCPMasterAccess (PRU_ESC_CTRL *,
                                                        VXB_DEV_ID
                                                        );
LOCAL STATUS            pruEscHwSpinLockInit  (PRU_ESC_CTRL *,
                                                        VXB_DEV_ID
                                                        );

LOCAL STATUS            pruEscConfigPRCM      (PRU_ESC_CTRL *);
LOCAL VXB_FDT_DEV *     PruEscFdtDevGet       (VXB_DEV_ID,
                                                        VXB_DEV_ID
                                                        );
LOCAL STATUS            PruEscResFree         (VXB_DEV_ID,
                                                        VXB_DEV_ID,
                                                        VXB_RESOURCE *
                                                        );
LOCAL VXB_RESOURCE *    PruEscResAlloc        (VXB_DEV_ID,
                                                        VXB_DEV_ID,
                                                        uint32_t
                                                        );


#endif  /* _PRUSS_H_ */
