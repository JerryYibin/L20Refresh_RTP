#include <vxWorks.h>
#include <stdio.h>
#include <string.h>

#define TEST_AUX    "aux"
#define TEST_EEPROM "eeprom"
#define TEST_GPIO   "gpio"
#define TEST_EQEP   "eqep"
#define TEST_EPWM   "ehrpwm"
#define TEST_MCSPI  "mcspi"
#define TEST_RTC    "rtc"

extern void testAux();
extern void testEeprom();
extern void testGpio(int);
extern void testEqep();
extern void testEhrpwm();
extern void testSpi3();
extern void testRtc();

int main(int argc, char *argv[])
    {
    int i;
    for(i=0; i<argc; i++)
        {
        printf("argv[%d] = %s\n", i, argv[i]);
        if(strcmp(argv[i], TEST_AUX) == 0)
            {
            testAux();
            }
        else if(strcmp(argv[i], TEST_EEPROM) == 0)
            {
            testEeprom();
            }
        else if(strncmp(argv[i], TEST_GPIO, strlen(TEST_GPIO)) == 0)
            {
            UINT32 num = 0;
            if(strlen(argv[i])>strlen(TEST_GPIO))
                {
                num = argv[i][strlen(TEST_GPIO)]-'0';
                if(num>4)
                    num = 0;
                }
            testGpio(num);
            }
        else if(strcmp(argv[i], TEST_EQEP) == 0)
            {
            testEqep();
            }
        else if(strcmp(argv[i], TEST_EPWM) == 0)
            {
            testEhrpwm();
            }
        else if(strcmp(argv[i], TEST_MCSPI) == 0)
            {
            testSpi3();
            }
        else if(strcmp(argv[i], TEST_RTC) == 0)
            {
            testRtc();
            }
        }
    }

