

#ifndef EPWM_H_
#define EPWM_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef int                          Int32;
typedef short                        Int16;
typedef char                         Int8;
        

// eQEP register offsets from its base IO address
#define QPOSCNT    0x0000
#define QPOSINIT   0x0004
#define QPOSMAX    0x0008
#define QPOSCMP    0x000C
#define QPOSILAT   0x0010
#define QPOSSLAT   0x0014
#define QPOSLAT    0x0018
#define QUTMR      0x001C
#define QUPRD      0x0020    
#define QWDTMR     0x0024
#define QWDPRD     0x0026
#define QDECCTL    0x0028
#define QEPCTL     0x002A
#define QCAPCTL    0x002C
#define QPOSCTL    0x002E
#define QEINT      0x0030
#define QFLG       0x0032
#define QCLR       0x0034
#define QFRC       0x0036
#define QEPSTS     0x0038
#define QCTMR      0x003A
#define QCPRD      0x003C
#define QCTMRLAT   0x003E
#define QCPRDLAT   0x0040
#define QREVID     0x005C

// Bits for the QDECTL register
#define QSRC1      (0x0001 << 15)
#define QSRC0      (0x0001 << 14)
#define SOEN       (0x0001 << 13)
#define SPSEL      (0x0001 << 12)
#define XCR        (0x0001 << 11)
#define SWAP       (0x0001 << 10)
#define IGATE      (0x0001 << 9)
#define QAP        (0x0001 << 8)
#define QBP        (0x0001 << 7)
#define QIP        (0x0001 << 6)
#define QSP        (0x0001 << 5)

// Bits for the QEPCTL register
#define FREESOFT1  (0x0001 << 15)
#define FREESOFT0  (0x0001 << 14)
#define PCRM1      (0x0001 << 13)
#define PCRM0      (0x0001 << 12)
#define SEI1       (0x0001 << 11)
#define SEI0       (0x0001 << 10)
#define IEI1       (0x0001 << 9)
#define IEI0       (0x0001 << 8)
#define SWI        (0x0001 << 7)
#define SEL        (0x0001 << 6)
#define IEL1       (0x0001 << 5)
#define IEL0       (0x0001 << 4)
#define PHEN       (0x0001 << 3)
#define QCLM       (0x0001 << 2)
#define UTE        (0x0001 << 1)
#define WDE        (0x0001 << 0)

// Bits for the QCAPCTL register
#define CEN        (0x0001 << 15)
#define CCPS2      (0x0001 << 6)
#define CCPS0      (0x0001 << 5)
#define CCPS1      (0x0001 << 4)
#define UPPS3      (0x0001 << 3)
#define UPPS2      (0x0001 << 2)
#define UPPS1      (0x0001 << 1)
#define UPPS0      (0x0001 << 0)

// Bits for the QPOSCTL register
#define PCSHDW     (0x0001 << 15)
#define PCLOAD     (0x0001 << 14)
#define PCPOL      (0x0001 << 13)
#define PCE        (0x0001 << 12)
#define PCSPW11    (0x0001 << 11)
#define PCSPW10    (0x0001 << 10)
#define PCSPW9    (0x0001 << 9)
#define PCSPW8    (0x0001 << 8)
#define PCSPW7    (0x0001 << 7)
#define PCSPW6    (0x0001 << 6)
#define PCSPW5    (0x0001 << 5)
#define PCSPW4    (0x0001 << 4)
#define PCSPW3    (0x0001 << 3)
#define PCSPW2    (0x0001 << 2)
#define PCSPW1    (0x0001 << 1)
#define PCSPW0    (0x0001 << 0)

#define PWMSS_QPOSCNT    0x0000
#define PWMSS_QPOSINIT   0x0004
#define PWMSS_QPOSMAX    0x0008
#define PWMSS_QPOSCMP    0x000C
#define PWMSS_QPOSILAT   0x0010
#define PWMSS_QPOSSLAT   0x0014
#define PWMSS_QPOSLAT    0x0018
#define PWMSS_QUTMR      0x001C
#define PWMSS_QUPRD      0x0020
#define PWMSS_QWDTMR     0x0024
#define PWMSS_QWDPRD     0x0026
#define PWMSS_QDECCTL    0x0028
#define PWMSS_QEPCTL     0x002A
#define PWMSS_QCAPCTL    0x002C
#define PWMSS_QPOSCTL    0x002E
#define PWMSS_QEINT      0x0030
#define PWMSS_QFLG       0x0032
#define PWMSS_QCLR       0x0034
#define PWMSS_QFRC       0x0036
#define PWMSS_QEPSTS     0x0038
#define PWMSS_QCTMR      0x003A
#define PWMSS_QCPRD      0x003C
#define PWMSS_QCTMRLAT   0x003E
#define PWMSS_QCPRDLAT   0x0040
#define PWMSS_QREVID     0x005C
    
#define EQEPCTL_FREESOFT1  (1 << 15)
#define EQEPCTL_FREESOFT0  (1 << 14)
#define EQEPCTL_PCRM1      (1 << 13)
#define EQEPCTL_PCRM0      (1 << 12)
#define EQEPCTL_SEI1       (1 << 11)
#define EQEPCTL_SEI0       (1 << 10)
#define EQEPCTL_IEI1       (1 << 9)
#define EQEPCTL_IEI0       (1 << 8)
#define EQEPCTL_SWI        (1 << 7)
#define EQEPCTL_SEL        (1 << 6)
#define EQEPCTL_IEL1       (1 << 5)
#define EQEPCTL_IEL0       (1 << 4)
#define EQEPCTL_PHEN       (1 << 3)
#define EQEPCTL_QCLM       (1 << 2)
#define EQEPCTL_UTE        (1 << 1)
#define EQEPCTL_WDE        (1 << 0)

#define EQEP_STS_FDF_SHIFT                                                  (7U)
#define EQEP_STS_FDF_MASK                                                   (0x00000080U)
#define CSR_GET_FIELD(regVal, REG_FIELD)                                        \
    (((regVal) & (uint32_t) REG_FIELD##_MASK) >> (uint32_t) REG_FIELD##_SHIFT)
typedef struct {   
		VXB_DEV_ID  pDev;
		void * regBase;
		void * regHandle;
		uint32_t tbclk;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
		Int16 MfuncPeriod;    // Input: Period scaler (Q15) 
		VXB_RESOURCE *  intRes;
		} EQEPGEN ;  
                  
   
   /* PWM controller read and write interface */

   #define AM43XX_PWM_BAR(p)       (((EQEPGEN *)vxbDevSoftcGet(p))->regBase)
   #define AM43XX_PWM_HANDLE(p)    ((EQEPGEN *)vxbDevSoftcGet(p))->regHandle

  #define CSR_READ_4(pDev, addr)                          \
      vxbRead32 (AM43XX_PWM_HANDLE(pDev),                 \
          (unsigned int *)((char *)AM43XX_PWM_BAR(pDev) + addr))

  #define CSR_WRITE_4(pDev, addr, data)                   \
      vxbWrite32 (AM43XX_PWM_HANDLE(pDev),                \
          (unsigned int *)((char *)AM43XX_PWM_BAR(pDev) + addr), data)

  #define CSR_READ_2(pDev, addr)                          \
      vxbRead16 (AM43XX_PWM_HANDLE(pDev),                 \
          (UINT16 *)((char *)AM43XX_PWM_BAR(pDev) + addr))

  #define CSR_WRITE_2(pDev, addr, data)                   \
      vxbWrite16 (AM43XX_PWM_HANDLE(pDev),                \
    		  (UINT16 *)((char *)AM43XX_PWM_BAR(pDev) + addr), data)

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef EPWM_H_ */
