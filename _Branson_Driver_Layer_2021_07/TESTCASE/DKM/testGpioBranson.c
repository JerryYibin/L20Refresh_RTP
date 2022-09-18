
#include <stdio.h>
#include <logLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <subsys/gpio/vxbGpioLib.h>

#define GPIO_PIN_INTR_TRIG          INTR_TRIGGER_EDGE
#define GPIO_PIN_INTR_POL           INTR_POLARITY_HIGH

/* GPIO Input */
#define GPIO1_3	   		0x03

#define GPIO2_2			0x22
#define GPIO2_19		0x33
#define GPIO2_22		0x36
#define GPIO2_24		0x38
#define GPIO2_25		0x39
#define GPIO2_26		0x3A
#define GPIO2_27		0x3B
#define GPIO2_28		0x3C
#define GPIO2_29		0x3D

#define GPIO3_30		0x5E
#define GPIO3_31		0x5F

#define GPIO4_2			0x62

#define GPIO5_15		0x8F

#define GPIO7_22     	0xD6
#define GPIO7_24     	0xD8
#define GPIO7_25     	0xD9


/* GPIO Output */
#define GPIO1_4     	0x04
#define GPIO1_5     	0x05
#define GPIO1_16	 	0x10
#define GPIO1_17    	0x11

#define GPIO4_3			0x63
#define GPIO4_4			0x64
#define GPIO4_5			0x65
#define GPIO4_8			0x68
#define GPIO4_9			0x69

#define GPIO5_0			0x80
#define GPIO5_1			0x81
#define GPIO5_4			0x84
#define GPIO5_5			0x85
#define GPIO5_6			0x86
#define GPIO5_7			0x87
#define GPIO5_8			0x88
#define GPIO5_9			0x89
#define GPIO5_16		0x90

#define GPIO6_7	     	0xA7
#define GPIO6_19    	0xB3

#define GPIO7_23     	0xD7


typedef struct tagGPIO_TEST_CASE {
    char *name;
    int   id;
    } GPIO_TEST_CASE;

LOCAL GPIO_TEST_CASE output_gpio_list[] = {
	{"GPIO1_4", GPIO1_4},
	{"GPIO1_5", GPIO1_5},
	{"GPIO1_16", GPIO1_16},
	{"GPIO1_17", GPIO1_17},
	
    {"GPIO4_3",  GPIO4_3},
    {"GPIO4_4",  GPIO4_4},
    {"GPIO4_5",  GPIO4_5},
    {"GPIO4_8",  GPIO4_8},
    {"GPIO4_9",  GPIO4_9},

    {"GPIO5_0",  GPIO5_0},	
    {"GPIO5_1",  GPIO5_1},	
    {"GPIO5_4",  GPIO5_4},	
    {"GPIO5_5",  GPIO5_5},	
    {"GPIO5_6",  GPIO5_6},	
    {"GPIO5_7",  GPIO5_7},	
    {"GPIO5_8",  GPIO5_8},	
    {"GPIO5_9",  GPIO5_9},	
    {"GPIO5_16",  GPIO5_16},	

    {"GPIO6_7",  GPIO6_7},	
    {"GPIO6_19",  GPIO6_19},	
	
    {"GPIO7_23",  GPIO7_23},	
	
    {NULL,   0}
    };

LOCAL GPIO_TEST_CASE input_gpio_list[] = {
	{"GPIO1_3", GPIO1_3},
    
	{"GPIO2_2", GPIO2_2},
    {"GPIO2_19", GPIO2_19},
    {"GPIO2_22", GPIO2_22},
    {"GPIO2_24", GPIO2_24},
    {"GPIO2_25", GPIO2_25},
    {"GPIO2_26", GPIO2_26},
    {"GPIO2_27", GPIO2_27},
    {"GPIO2_28", GPIO2_28},
    {"GPIO2_29", GPIO2_29},
	
    {"GPIO3_30", GPIO3_30},
    {"GPIO3_31", GPIO3_31},
	
    {"GPIO4_2", GPIO4_2},

    {"GPIO5_15", GPIO5_15},

    {"GPIO7_22", GPIO7_22},
    {"GPIO7_24", GPIO7_24},
    {"GPIO7_25", GPIO7_25},

    {NULL,   0}
    };

LOCAL GPIO_TEST_CASE test_gpio_list[] = {
	{"GPIO1_3", GPIO1_3},
	{"GPIO1_4", GPIO1_4},
	{"GPIO1_5", GPIO1_5},
	{"GPIO1_16", GPIO1_16},
	{"GPIO1_17", GPIO1_17},
	
	{"GPIO2_2", GPIO2_2},
    {"GPIO2_19", GPIO2_19},
    {"GPIO2_22", GPIO2_22},
    {"GPIO2_24", GPIO2_24},
    {"GPIO2_25", GPIO2_25},
    {"GPIO2_26", GPIO2_26},
    {"GPIO2_27", GPIO2_27},
    {"GPIO2_28", GPIO2_28},
    {"GPIO2_29", GPIO2_29},

    {"GPIO3_30", GPIO3_30},
    {"GPIO3_31", GPIO3_31},
 
    {"GPIO4_2",  GPIO4_2},
    {"GPIO4_3",  GPIO4_3},
    {"GPIO4_4",  GPIO4_4},
    {"GPIO4_5",  GPIO4_5},
    {"GPIO4_8",  GPIO4_8},
    {"GPIO4_9",  GPIO4_9},

    {"GPIO5_0",  GPIO5_0},	
    {"GPIO5_1",  GPIO5_1},	
    {"GPIO5_4",  GPIO5_4},	
    {"GPIO5_5",  GPIO5_5},	
    {"GPIO5_6",  GPIO5_6},	
    {"GPIO5_7",  GPIO5_7},	
    {"GPIO5_8",  GPIO5_8},	
    {"GPIO5_9",  GPIO5_9},	
    {"GPIO5_15", GPIO5_15},
    {"GPIO5_16",  GPIO5_16},	
	
    {"GPIO6_7", GPIO6_7},
    {"GPIO6_19", GPIO6_19},


    {"GPIO7_22", GPIO7_22},
    {"GPIO7_23", GPIO7_23},
    {"GPIO7_24", GPIO7_24},
    {"GPIO7_25", GPIO7_25},
    {NULL,   0}
    };

static int isrCount=0;

static void gpioIsr(void *arg)
    {
    int id = (int)arg;
    logMsg("### %s from gpio %#x ###\n", (_Vx_usr_arg_t)__FUNCTION__, id,0,0,0,0);
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
    UINT32 level = GPIO_VALUE_HIGH;
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

