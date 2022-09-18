
#include <stdio.h>
#include <sysLib.h>
#include <taskLib.h>
#ifdef _WRS_KERNEL
#include <subsys/gpio/vxbGpioLib.h>
#else
#include <customSystemCall.h>
#endif

#define GPIO_PIN_INTR_TRIG          INTR_TRIGGER_EDGE
#define GPIO_PIN_INTR_POL           INTR_POLARITY_HIGH

/* GPIO Input */
#define GPIO_1_22     0x16
#define GPIO_1_24     0x18
#define GPIO_1_25     0x19

#define GPIO_6_10     0xAA
#define GPIO_6_11     0xAB
#define GPIO_6_17     0xB1
#define GPIO_6_18     0xB2
#define GPIO_6_19     0xB3
#define GPIO_6_20     0xB4
#define GPIO_6_29     0xBD

#define GPIO_7_22     0xD6
#define GPIO_7_23     0xD7

/* GPIO Output */
#define GPIO_3_28     0x5C
#define GPIO_3_29     0x5D
#define GPIO_3_30     0x5E
#define GPIO_3_31     0x5F

#define GPIO_4_0      0x60
#define GPIO_4_1      0x61
#define GPIO_4_2      0x62
#define GPIO_4_3      0x63
#define GPIO_4_4      0x64
#define GPIO_4_5      0x65
#define GPIO_4_6      0x66
#define GPIO_4_7      0x67
#define GPIO_4_8      0x68
#define GPIO_4_9      0x69
#define GPIO_4_10     0x6A
#define GPIO_4_11     0x6B

typedef struct tagGPIO_TEST_CASE {
    char *name;
    int   id;
    } GPIO_TEST_CASE;

LOCAL GPIO_TEST_CASE output_gpio_list[] = {
    {"GPIO_3_28", GPIO_3_28},
    {"GPIO_3_29", GPIO_3_29},
    {"GPIO_3_30", GPIO_3_30},
    {"GPIO_3_31", GPIO_3_31},

    {"GPIO_4_0",  GPIO_4_0},
    {"GPIO_4_1",  GPIO_4_1},
    {"GPIO_4_2",  GPIO_4_2},
    {"GPIO_4_3",  GPIO_4_3},
    {"GPIO_4_4",  GPIO_4_4},
    {"GPIO_4_5",  GPIO_4_5},
    {"GPIO_4_6",  GPIO_4_6},
    {"GPIO_4_7",  GPIO_4_7},
    {"GPIO_4_8",  GPIO_4_8},
    {"GPIO_4_9",  GPIO_4_9},
    {"GPIO_4_10", GPIO_4_10},
    {"GPIO_4_11", GPIO_4_11},

    {NULL,   0}
    };

LOCAL GPIO_TEST_CASE input_gpio_list[] = {
    {"GPIO_1_22", GPIO_1_22},
    {"GPIO_1_24", GPIO_1_24},
    {"GPIO_1_25", GPIO_1_25},
    {"GPIO_6_10", GPIO_6_10},
    {"GPIO_6_11", GPIO_6_11},
    {"GPIO_6_17", GPIO_6_17},
    {"GPIO_6_18", GPIO_6_18},
    {"GPIO_6_19", GPIO_6_19},
    {"GPIO_6_20", GPIO_6_20},
    {"GPIO_6_29", GPIO_6_29},
    {"GPIO_7_22", GPIO_7_22},
    {"GPIO_7_23", GPIO_7_23},
    {NULL,   0}
    };

LOCAL GPIO_TEST_CASE test_gpio_list[] = {
    {"GPIO_1_22", GPIO_1_22},
    {"GPIO_1_24", GPIO_1_24},
    {"GPIO_1_25", GPIO_1_25},

    {"GPIO_3_28", GPIO_3_28},
    {"GPIO_3_29", GPIO_3_29},
    {"GPIO_3_30", GPIO_3_30},
    {"GPIO_3_31", GPIO_3_31},

    {"GPIO_4_0",  GPIO_4_0},
    {"GPIO_4_1",  GPIO_4_1},
    {"GPIO_4_2",  GPIO_4_2},
    {"GPIO_4_3",  GPIO_4_3},
    {"GPIO_4_4",  GPIO_4_4},
    {"GPIO_4_5",  GPIO_4_5},
    {"GPIO_4_6",  GPIO_4_6},
    {"GPIO_4_7",  GPIO_4_7},
    {"GPIO_4_8",  GPIO_4_8},
    {"GPIO_4_9",  GPIO_4_9},
    {"GPIO_4_10", GPIO_4_10},
    {"GPIO_4_11", GPIO_4_11},

    {"GPIO_6_10", GPIO_6_10},
    {"GPIO_6_11", GPIO_6_11},
    {"GPIO_6_17", GPIO_6_17},
    {"GPIO_6_18", GPIO_6_18},
    {"GPIO_6_19", GPIO_6_19},
    {"GPIO_6_20", GPIO_6_20},
    {"GPIO_6_29", GPIO_6_29},

    {"GPIO_7_22", GPIO_7_22},
    {"GPIO_7_23", GPIO_7_23},
    {NULL,   0}
    };

static int isrCount=0;

static void gpioIsr(void *arg)
    {
    int id = (int)arg;
#ifdef _WRS_KERNEL
    logMsg("### %s from gpio %#x ###\n", (_Vx_usr_arg_t)__FUNCTION__, id,0,0,0,0);
#else
    printf("### %s from gpio %#x ###\n", __FUNCTION__, id);
#endif
    isrCount++;
    }

static STATUS testGpioAllocation()
    {
    GPIO_TEST_CASE *p;
    for(p = (GPIO_TEST_CASE*)test_gpio_list; p->name != NULL; p++)
        {
        printf("allocate pin for %s\n", p->name);
        if(vxbGpioAlloc(p->id) != OK)
            {
            printf("%s failed to allocate pin, try to free it first\n", p->name);
            if(vxbGpioFree(p->id) != OK)
                {
                printf("%s failed to free pin\n", p->name);
                return ERROR;
                }

            if(vxbGpioAlloc(p->id) != OK)
                {
                printf("%s failed to allocate pin again\n", p->name);
                return ERROR;
                }
            }
        }
    return OK;
    }

static STATUS testGpioDirection()
    {
    GPIO_TEST_CASE *p;
    int dir;
    for(p = (GPIO_TEST_CASE*)test_gpio_list; p->name != NULL; p++)
        {
        dir = vxbGpioGetDir(p->id);
        printf("%s original direction: %s\n", p->name, (dir == GPIO_DIR_OUTPUT) ? "output" : "input");
        }
    return OK;
    }

static STATUS testGpioOutput()
    {
    UINT32 level = GPIO_VALUE_LOW;
    GPIO_TEST_CASE *p;
    for(p = (GPIO_TEST_CASE*)output_gpio_list; p->name != NULL; p++)
        {
        printf("switching %s to output direction\n", p->name);
        if(vxbGpioSetDir(p->id, GPIO_DIR_OUTPUT) != OK)
            {
            printf("failed to switch %s to output direction\n", p->name);
            return ERROR;
            }
        if(level == GPIO_VALUE_LOW)
            {
            printf("%s output value %d\n", p->name, GPIO_VALUE_HIGH);
            level = GPIO_VALUE_HIGH;
            if(vxbGpioSetValue(p->id, level) != OK)
                {
                printf("%s failed to output value\n", p->name);
                return ERROR;
                }
            }
        else
            {
            printf("%s output value %d\n", p->name, GPIO_VALUE_LOW);
            level = GPIO_VALUE_LOW;
            if(vxbGpioSetValue(p->id, level) != OK)
                {
                printf("%s failed to output value\n", p->name);
                return ERROR;
                }
            }
        }
    return OK;
    }

static STATUS testGpioInput()
    {
    GPIO_TEST_CASE *p;
    for(p = (GPIO_TEST_CASE*)input_gpio_list; p->name != NULL; p++)
        {
        if(vxbGpioSetDir(p->id, GPIO_DIR_INPUT) != OK)
            {
            printf("%s failed to input direction\n", p->name);
            return ERROR;
            }
        }
    for(p = (GPIO_TEST_CASE*)input_gpio_list; p->name != NULL; p++)
        {
        printf("waiting 5 seconds for input\n");
        for (int i = 0; i < 5; i++)
            {
            taskDelay(sysClkRateGet());
            printf(" %d ", i);
            printf("%s read value: %d\n",p->name, vxbGpioGetValue(p->id));
            }
        printf("\n");
        }

    return OK;
    }

static STATUS testGpioIsr()
    {  
    GPIO_TEST_CASE *p;
    for(p = (GPIO_TEST_CASE*)input_gpio_list; p->name != NULL; p++)
        {
        if(vxbGpioSetDir(p->id, GPIO_DIR_INPUT) != OK)
            {
            printf("%s failed to input direction\n", p->name);
            return ERROR;
            }
        }

    for(p = (GPIO_TEST_CASE*)input_gpio_list; p->name != NULL; p++)
        {
        printf("\nconfiguring IRQ attribute for %s\n", p->name);
        if(vxbGpioIntConfig(p->id, GPIO_PIN_INTR_TRIG, GPIO_PIN_INTR_POL) != OK)
            {
            printf("failed to configure IRQ attribute for input pin\n");
            return ERROR;
            }

        printf("connecting ISR for %s\n", p->name);
        if(vxbGpioIntConnect(p->id, gpioIsr, (void *)p->id) != OK)
            {
            printf("failed to connect ISR for input pin\n");
            return ERROR;
            }

        printf("enabling ISR for %s\n", p->name);
        if(vxbGpioIntEnable(p->id, gpioIsr, (void *)p->id) != OK)
            {
            printf("failed to enable ISR for input pin\n");
            return ERROR;
            }
        }

    printf("\nwaiting for 10 times ISR\n");
    while(isrCount<10)
        taskDelay(sysClkRateGet());

    return OK;
    }

static STATUS testGpioFree()
    {
    GPIO_TEST_CASE *p;
    for(p = (GPIO_TEST_CASE*)test_gpio_list; p->name != NULL; p++)
        {
        printf("free pin %s\n", p->name);
        if(vxbGpioFree(p->id) != OK)
            {
            printf("failed to free pin %s\n", p->name);
            return ERROR;
            }
        }
    return OK;
    }
void testGpio(int num)
    {
    STATUS sta;

    printf("### %s ###\n", __FUNCTION__);

    sta = testGpioAllocation();
    if(sta == ERROR)
        return;
    if((num==0)||(num==1))
        {
        sta = testGpioDirection();
        if(sta == ERROR)
            return;
        }
    if((num==0)||(num==2))
        {
        sta = testGpioOutput();
        if(sta == ERROR)
            return;
        }

    if((num==0)||(num==3))
        {
        sta = testGpioInput();
        if(sta == ERROR)
            return;
        }

    if((num==0)||(num==4))
        {
        sta = testGpioIsr();
        if(sta == ERROR)
            return;
        }

    testGpioFree();
    }

