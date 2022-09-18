
#include <stdio.h>
#ifdef _WRS_KERNEL
#include <vxbEhrPwmLib.h>
#else
#include <customSystemCall.h>
#endif

void testEhrpwm()
    {
    printf("### %s ###\n", __FUNCTION__);

    printf("ehrpwmDevNumGet = %d\n", ehrpwmDevNumGet());
    vxbEhrPwmGetIntStatus(0);

    printf("\nvxbEhrPwmSetAQSFRC 0x12\n");
    vxbEhrPwmSetAQSFRC(0, 0x12);
    vxbEhrPwmGetAQSFRC(0);

    printf("\nvxbEhrPwmSetTbClk 0x34\n");
    vxbEhrPwmSetTbClk(0, 0x34);
    vxbEhrPwmGetTbClk(0);

    printf("\nvxbEhrPwmSetTbAQConfA 0x56\n");
    vxbEhrPwmSetTbAQConfA(0, 0x56);
    vxbEhrPwmGetTbAQConfA(0);

    printf("\nvxbEhrPwmSetTbAQConfB 0x78\n");
    vxbEhrPwmSetTbAQConfB(0, 0x78);
    vxbEhrPwmGetTbAQConfB(0);

    printf("\nvxbEhrPwmSetTbPWMCfg 0x90 1\n");
    vxbEhrPwmSetTbPWMCfg(0, 0x90, 1);
    vxbEhrPwmGetTbPWMCfg(0);
    }

