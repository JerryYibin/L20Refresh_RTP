
#include <stdio.h>
#ifdef _WRS_KERNEL
#include <vxbEqepLib.h>
#else
#include <customSystemCall.h>
#endif

void testEqep()
    {
    printf("### %s ###\n", __FUNCTION__);

    printf("eqepDevNumGet = %d\n", eqepDevNumGet());
    printf("vxbEqepSetMaxCount 0x1234\n", vxbEqepSetMaxCount(0, 0x1234));
    printf("vxbEqepGetMaxCount = %#x\n", vxbEqepGetMaxCount(0));
    printf("vxbEqepSetInitCount 0x5678\n", vxbEqepSetInitCount(0, 0x5678));
    printf("vxbEqepGetInitCount = %#x\n", vxbEqepGetInitCount(0));
    printf("vxbEqepGetPosCount = %d#x\n", vxbEqepGetPosCount(0));
    printf("vxbEqepGetDirection = %d\n", vxbEqepGetDirection(0));
    vxbEqepShow(0);
    }

