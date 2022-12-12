/* includes */

#include <stdio.h>
#include <taskLib.h>
#include <msgQLib.h>

extern int atoi(const char *);

#define MSG_Q_Control "/msgQControl"  /* Public semaphore */
#define MSG_Q_Data "/msgQData"  /* Public semaphore */
#define MSG_Q_Request "/msgQRequest"  /* Public semaphore */
#define TASK_NAME   "/Data_Task"

typedef struct
    {
    UINT32 msgID;
    char Buffer[800];
    }MESSAGE;

#define USER_NAME_SIZE      16
#define BARCODE_DATA_SIZE 50
#define RECIPE_LEN        20
typedef struct
    {
    UINT32    RecipeNum;
    char     PartID[BARCODE_DATA_SIZE];
    char    UserName[USER_NAME_SIZE];
    UINT32  CycleCounter;
    UINT32  RecipeNumber;
    UINT32  RecipeRevNumber;
    UINT32    WeldStatus;
    UINT32    TotalEnergy;
    UINT32    ActualWidth;
    UINT32    Amplitude;
    UINT32    TriggerPressure;
    UINT32    WeldPressure;
    UINT32    WeldTime;
    UINT32    PeakPower;
    UINT32    PreHeight;
    UINT32    PostHeight;
    union
        {
        UINT32    ALARMflags;
        /* There is a little endian for structure bit field alignment for alarm */
        struct
            {
            UINT32    Overload            : 1; //bit 0
            UINT32    HeightSystemFailure    : 1; //bit 1
            UINT32    WeldAborted            : 1; //bit 2

            UINT32    TimeMin                : 1; //bit 3
            UINT32    PowerMin            : 1; //bit 4
            UINT32    PreHeightMin        : 1; //bit 5
            UINT32    PostHeightMin        : 1; //bit 6

            UINT32    TimeMax                : 1; //bit 7
            UINT32    PowerMax            : 1; //bit 8
            UINT32    PreHeightMax        : 1; //bit 9
            UINT32    PostHeightMax        : 1; //bit 10

            UINT32    HeightEncoderBad    : 1; //bit 11
            UINT32    FootPedalAbort        : 1; //bit 12
            UINT32    SafetySwitch        : 1; //bit 13
            UINT32    WidthError            : 1; //bit 14

            UINT32    CutterSwitch        : 1; //bit 15
            }AlarmFlags;
        }ALARMS;
    }WELD_RESULT;

#define STEP_MAX         5
#define PIC_PATH_LEN     60
enum WELDMODE
{
    ENERGY_MODE = 0,
    TIME_MODE,
    HEIGHT_MODE,
    ENERGY_HEIGHT_MODE
};

enum WELDAMPLITUDESTEPMMODE
{
    STEP_NO_MODE    = -1,
    STEP_ENERGY_MODE = 0,
    STEP_TIME_MODE,
    STEP_POWER_MODE,

};

enum TRIGGERMODE
{
    TRIGGER_DISTANCE_MODE,
    TRIGGER_TIME_MODE,
    TRIGGER_FORCE_MODE
};

typedef struct
{
    int               m_TriggerType;
    unsigned int      m_PreTriggerDistance;
    unsigned int      m_PreTriggerForce;
    unsigned int      m_PreTriggerTime;
}Trigger;

typedef struct 
{
    int m_WeldMode;
    int m_WeldStepMode;
    Trigger m_Trigger;
    int m_PreBurst;
    int m_HoldTime;
    int m_SqueezeTime;
    int m_AfterBurstDelay;
    int m_AfterBurstTime;
    int m_AfterBurstAmplitude;
    int m_DisplayedHeightOffSet;
    int m_MeasuredHeightOffSet;
}AdvancedWeldSetting;

typedef struct
{
    int m_Order;
    int m_StepValue;      //step_energy_value/step_time_value/step_power_value
    int m_AmplitudeValue;
}WeldStepValueSetting;

typedef struct 
{
    unsigned int m_EnergySetting;
    int          m_TPpressure;
    int          m_WPpressure;
    unsigned int m_Amplitude;
    WeldStepValueSetting m_EnergyStep[STEP_MAX];
    WeldStepValueSetting m_TimeStep[STEP_MAX];
    WeldStepValueSetting m_PowerStep[STEP_MAX];
    int          m_WidthSetting;
}WeldParameterSetting;

typedef struct 
{
    int m_TimeMax;
    int m_TimeMin;
    unsigned int m_PeakPowerMax;
    unsigned int m_PeakPowerMin;
    int m_PreHeightMax;
    int m_PreHeightMin;
    int m_HeightMax;
    int m_HeightMin;
}QualityWindowSetting;

typedef struct  
    {
	int					  m_RecipeID;
    int                   m_RecipeNumber;
    char                  m_RecipePicPath[PIC_PATH_LEN];
    BOOL                  m_IsTeachMode;
    char                  m_RecipeName[RECIPE_LEN];
    unsigned int          m_BatchSize;
    WeldParameterSetting  m_WeldParameter;
    QualityWindowSetting  m_QualityWindowSetting;
    AdvancedWeldSetting   m_AdvancedSetting;
    }WeldRecipeSC;


enum MESSAGE_IDENTIFY
    {
    /* Macro defined to DATA TASK */
    TO_DATA_TASK_OPEN_DB,
    TO_DATA_TASK_CLOSE_DB,

    TO_DATA_TASK_WELD_RECIPE_QUERY_ALL,
    TO_DATA_TASK_WELD_RECIPE_QUERY_LATEST_PAGE,
    TO_DATA_TASK_WELD_RECIPE_QUERY_NEXT_PAGE,
    TO_DATA_TASK_WELD_RECIPE_QUERY,
    TO_DATA_TASK_WELD_RECIPE_INSERT,
    TO_DATA_TASK_WELD_RECIPE_UPDATE,
    TO_DATA_TASK_WELD_RECIPE_DELETE,
    TO_DATA_TASK_WELD_RECIPE_CLEAR,
    TO_DATA_TASK_WELD_RECIPE_DELETE_SPECIFIC,
    TO_DATA_TASK_WELD_RECIPE_TOTAL_NUMBER,
    TO_DATA_TASK_WELD_RECIPE_RENAME,

    TO_DATA_TASK_WELD_RESULT_QUERY,
    TO_DATA_TASK_WELD_RESULT_QUERY_LATEST_PAGE,
    TO_DATA_TASK_WELD_RESULT_QUERY_NEXT_PAGE,
    TO_DATA_TASK_WELD_RESULT_INSERT,
    TO_DATA_TASK_WELD_RESULT_DELETE,
    TO_DATA_TASK_WELD_RESULT_CLEAR,

    TO_DATA_TASK_WELD_SIGN_QUERY,
    TO_DATA_TASK_WELD_SIGN_INSERT,
    TO_DATA_TASK_WELD_SIGN_DELETE,
    TO_DATA_TASK_WELD_SIGN_CLEAR,

    TO_DATA_TASK_ALARM_LOG_QUERY,
    TO_DATA_TASK_ALARM_LOG_QUERY_LATEST_PAGE,
    TO_DATA_TASK_ALARM_LOG_QUERY_NEXT_PAGE,
    TO_DATA_TASK_ALARM_LOG_INSERT,
    TO_DATA_TASK_ALARM_LOG_CLEAR,
    TO_DATA_TASK_HEIGHT_CALIBRATE_QUERY,
    TO_DATA_TASK_HEIGHT_CALIBRATE_UPDATE,

    TO_DATA_TASK_DB_VERSION_QUERY,

    TO_DATA_TASK_USER_PROFILE_QUERY,
    TO_DATA_TASK_USER_PROFILE_UPDATE,

    TO_DATA_TASK_PRIVILEGE_CONFIG_QUERY,
    TO_DATA_TASK_PRIVILEGE_CONFIG_UPDATE,

    TO_DATA_TASK_PWR_SUPPLY_QUERY,
    TO_DATA_TASK_PWR_SUPPLY_UPDATE,

    TO_DATA_TASK_TEACH_MODE_SET_QUERY,
    TO_DATA_TASK_TEACH_MODE_SET_UPDATE,

    TO_DATA_TASK_SYS_CONFIG_QUERY,
    TO_DATA_TASK_SYS_CONFIG_UPDATE,

    TO_DATA_TASK_ACTIVE_RECIPE_QUERY,
    TO_DATA_TASK_ACTIVE_RECIPE_UPDATE,

    TO_DATA_TASK_ETHERNET_CONFIG_UPDATE,

    TO_DATA_TASK_CONNECTIVITY_QUERY,
    TO_DATA_TASK_CONNECTIVITY_UPDATE,

    TO_DATA_TASK_GATEWAY_SERVER_QUERY,
    };
#define DATA_TASK_EVENT 0x02

int main(int argc, char *argv[])
    {
    TASK_ID tid = taskOpen(TASK_NAME,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    MSG_Q_ID mqidControl = msgQOpen(MSG_Q_Control, 0, 0, 0, 0, 0);
    MSG_Q_ID mqidData = msgQOpen(MSG_Q_Data, 0, 0, 0, 0, 0);
    MSG_Q_ID mqidRequest = msgQOpen(MSG_Q_Request, 0, 0, 0, 0, 0);
    MESSAGE buf = {0};
/*
 * 0 for Database
 *   a for TO_DATA_TASK_OPEN_DB
 *   b for TO_DATA_TASK_CLOSE_DB
 *
 * 1 for WeldRecipeSC
 *   a for TO_DATA_TASK_WELD_RECIPE_INSERT
 *     third parameter for m_RecipeName
 *   b for query
 *     no third parameter for TO_DATA_TASK_WELD_RECIPE_QUERY_ALL
 *     third parameter is ID for TO_DATA_TASK_WELD_RECIPE_QUERY
 *   c for TO_DATA_TASK_WELD_RECIPE_CLEAR
 *   d for delete
 *     no third parameter for TO_DATA_TASK_WELD_RECIPE_DELETE
 *     third parameter is ID for TO_DATA_TASK_WELD_RECIPE_DELETE_SPECIFIC
 *   e for TO_DATA_TASK_WELD_RECIPE_QUERY_LATEST_PAGE
 *   f for TO_DATA_TASK_WELD_RECIPE_QUERY_NEXT_PAGE
 *   u for TO_DATA_TASK_WELD_RECIPE_UPDATE
 *   r for TO_DATA_TASK_WELD_RECIPE_RENAME
 *
 * 2 for WELD_RESULT
 *   a for TO_DATA_TASK_WELD_RESULT_INSERT including table AlarmLog and WeldResultSignature
 *     third parameter is count
 *   b for TO_DATA_TASK_WELD_RESULT_QUERY
 *     third parameter is ID of last in RESULT_FOR_UI_MAX
 *   c for TO_DATA_TASK_WELD_RESULT_CLEAR
 *   d for TO_DATA_TASK_WELD_RESULT_DELETE
 *   e for TO_DATA_TASK_WELD_RESULT_QUERY_LATEST_PAGE
 *   f for TO_DATA_TASK_WELD_RESULT_QUERY_NEXT_PAGE
 *
 * 3 for WeldResultSignature_Data
 *   a for TO_DATA_TASK_WELD_SIGN_INSERT
 *     third parameter is WeldResultID
 *   x for TO_DATA_TASK_WELD_SIGN_INSERT
 *     third parameter is count
 *   b for TO_DATA_TASK_WELD_SIGN_QUERY
 *   c for TO_DATA_TASK_WELD_SIGN_CLEAR
 *   d for TO_DATA_TASK_WELD_SIGN_DELETE
 *
 * 4 for AlarmLog
 *   a for TO_DATA_TASK_ALARM_LOG_INSERT
 *     third parameter is data
 *   b for TO_DATA_TASK_ALARM_LOG_QUERY
 *     third parameter for ID of last in ALARM_LOG_MAX
 *   c for TO_DATA_TASK_ALARM_LOG_CLEAR
 *
 * 5 for HeightCalibration
 *   b for TO_DATA_TASK_HEIGHT_CALIBRATE_QUERY
 *   u for TO_DATA_TASK_HEIGHT_CALIBRATE_UPDATE
 * 6 for DbVersion
 *   b for TO_DATA_TASK_DB_VERSION_QUERY
 *
 * 7 for UserProfiles
 *   b for TO_DATA_TASK_USER_PROFILE_QUERY
 *   u for TO_DATA_TASK_USER_PROFILE_UPDATE
 *     third parameter is PermissionLevel
 *
 * 8 for PrivilegeConfiguration
 *   b for TO_DATA_TASK_PRIVILEGE_CONFIG_QUERY
 *     third parameter is data
 *   u for TO_DATA_TASK_PRIVILEGE_CONFIG_UPDATE
 *     third parameter is ScreenIndex
 *
 * 9 for PowerSupply
 *   b for TO_DATA_TASK_PWR_SUPPLY_QUERY
 *   u for TO_DATA_TASK_PWR_SUPPLY_UPDATE
 *
 * a for TeachModeSetting
 *   b for TO_DATA_TASK_TEACH_MODE_SET_QUERY
 *   u for TO_DATA_TASK_TEACH_MODE_SET_UPDATE
 *
 * b for SystemConfigure
 *   b for TO_DATA_TASK_SYS_CONFIG_QUERY
 *   u for TO_DATA_TASK_SYS_CONFIG_UPDATE
 *
 * c for ActiveRecipe
 *   b for TO_DATA_TASK_ACTIVE_RECIPE_QUERY
 *   u for TO_DATA_TASK_ACTIVE_RECIPE_UPDATE
 *
 * d for Connectivity
 *   b for TO_DATA_TASK_CONNECTIVITY_QUERY
 *   u for TO_DATA_TASK_CONNECTIVITY_UPDATE
 *
 * e for GatewaySever
 *   b for TO_DATA_TASK_GATEWAY_SERVER_QUERY
 */
    if(argc>=3)
        {
        switch(argv[1][0])
            {
            case '0':
                {
                switch(argv[2][0])
                    {
                    case 'a':
                        buf.msgID = TO_DATA_TASK_OPEN_DB;
                        break;
                    case 'b':
                        buf.msgID = TO_DATA_TASK_CLOSE_DB;
                        break;
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }                
            case '1':
                {
                switch(argv[2][0])
                    {
                    case 'a':
                        {
                        int i;    
                        buf.msgID = TO_DATA_TASK_WELD_RECIPE_INSERT;
                        if(argc>=4)
                            {
                            snprintf(buf.Buffer, RECIPE_LEN, "%s", argv[3]);
                            }
                        else
                            {
                            snprintf(buf.Buffer, RECIPE_LEN, "testrecipe");
                            }
                        break;
                        }
                    case 'b':
                        {
                        WeldRecipeSC *pData = (WeldRecipeSC *)&buf.Buffer[0];
                        if(argc>=4)
                            {
                            pData->m_RecipeID = atoi(argv[3]);
                            buf.msgID = TO_DATA_TASK_WELD_RECIPE_QUERY;
                            }
                        else
                            {
                            buf.msgID = TO_DATA_TASK_WELD_RECIPE_QUERY_ALL;
                            }
                        break;
                        }
                    case 'c':
                        buf.msgID = TO_DATA_TASK_WELD_RECIPE_CLEAR;
                        break;
                    case 'd':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            {
                            *pData = atoi(argv[3]);
                            buf.msgID = TO_DATA_TASK_WELD_RECIPE_DELETE_SPECIFIC;
                            }
                        else
                            {
                            buf.msgID = TO_DATA_TASK_WELD_RECIPE_DELETE;
                            }
                        break;
                        }
                    case 'e':
                        buf.msgID = TO_DATA_TASK_WELD_RECIPE_QUERY_LATEST_PAGE;
                        break;
                    case 'f':
                        buf.msgID = TO_DATA_TASK_WELD_RECIPE_QUERY_NEXT_PAGE;
                        break;
                    case 'u':
                        buf.msgID = TO_DATA_TASK_WELD_RECIPE_UPDATE;
                        break;
                    case 'r':
                        buf.msgID = TO_DATA_TASK_WELD_RECIPE_RENAME;
                        break;
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case '2':
                {
                switch(argv[2][0])
                    {
                    case 'a':
                        {
                        int i;    
                        int count = 1;
                        buf.msgID = TO_DATA_TASK_WELD_RESULT_INSERT;

                        if(argc>=4)
                            count = atoi(argv[3]);
                        if((count<1)||(count>1000000))
                            count = 1;
                        for(i=0; i<(count-1); i++)
                            {
                            msgQSend(mqidControl, (char *)&buf, sizeof(MESSAGE),-1,0);
                            eventSend(tid, DATA_TASK_EVENT);
                            }
                        break;
                        }
                    case 'b':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            *pData = atoi(argv[3]);
                        else
                            *pData = 1;
                        buf.msgID = TO_DATA_TASK_WELD_RESULT_QUERY;
                        break;
                        }
                    case 'c':
                        buf.msgID = TO_DATA_TASK_WELD_RESULT_CLEAR;
                        break;
                    case 'd':
                        buf.msgID = TO_DATA_TASK_WELD_RESULT_DELETE;
                        break;
                    case 'e':
                        buf.msgID = TO_DATA_TASK_WELD_RESULT_QUERY_LATEST_PAGE;
                        break;
                    case 'f':
                        buf.msgID = TO_DATA_TASK_WELD_RESULT_QUERY_NEXT_PAGE;
                        break;
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case '3':
                {
                switch(argv[2][0])
                    {
                    case 'a':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            *pData = atoi(argv[3]);
                        else
                            *pData = 1;
                        buf.msgID = TO_DATA_TASK_WELD_SIGN_INSERT;
                        break;
                        }
                    case 'x':
                        {
                        int i;    
                        int count = 1;
                        int *pData = (int *)&buf.Buffer[0];
                        buf.msgID = TO_DATA_TASK_WELD_SIGN_INSERT;

                        if(argc>=4)
                            count = atoi(argv[3]);
                        if((count<1)||(count>5000))
                            count = 1;
                        for(i=0; i<(count-1); i++)
                            {
                            *pData = i+1;
                            msgQSend(mqidControl, (char *)&buf, sizeof(MESSAGE),-1,0);
                            eventSend(tid, DATA_TASK_EVENT);
                            }
                        *pData = i+1;
                        break;
                        }
                    case 'b':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            *pData = atoi(argv[3]);
                        else
                            *pData = 1;
                        buf.msgID = TO_DATA_TASK_WELD_SIGN_QUERY;
                        break;
                        }
                    case 'c':
                        buf.msgID = TO_DATA_TASK_WELD_SIGN_CLEAR;
                        break;
                    case 'd':
                        buf.msgID = TO_DATA_TASK_WELD_SIGN_DELETE;
                        break;
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case '4':
                {
                switch(argv[2][0])
                    {
                    case 'a':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            *pData = atoi(argv[3]);
                        else
                            *pData = 1;
                        buf.msgID = TO_DATA_TASK_ALARM_LOG_INSERT;
                        break;
                        }
                    case 'b':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            *pData = atoi(argv[3]);
                        else
                            *pData = 1;
                        buf.msgID = TO_DATA_TASK_ALARM_LOG_QUERY;
                        break;
                        }
                    case 'c':
                        buf.msgID = TO_DATA_TASK_ALARM_LOG_CLEAR;
                        break;
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case '5':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        {
                        buf.msgID = TO_DATA_TASK_HEIGHT_CALIBRATE_QUERY;
                        break;
                        }
                    case 'u':
                        {
                        buf.msgID = TO_DATA_TASK_HEIGHT_CALIBRATE_UPDATE;
                        break;
                        }
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case '6':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        {
                        buf.msgID = TO_DATA_TASK_DB_VERSION_QUERY;
                        break;
                        }
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case '7':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        {
                        buf.msgID = TO_DATA_TASK_USER_PROFILE_QUERY;
                        break;
                        }
                    case 'u':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            *pData = atoi(argv[3]);
                        else
                            *pData = 1;
                        buf.msgID = TO_DATA_TASK_USER_PROFILE_UPDATE;
                        break;
                        }
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case '8':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            *pData = atoi(argv[3]);
                        else
                            *pData = 1;
                        buf.msgID = TO_DATA_TASK_PRIVILEGE_CONFIG_QUERY;
                        break;
                        }
                    case 'u':
                        {
                        int *pData = (int *)&buf.Buffer[0];
                        if(argc>=4)
                            *pData = atoi(argv[3]);
                        else
                            *pData = 1;
                        buf.msgID = TO_DATA_TASK_PRIVILEGE_CONFIG_UPDATE;
                        break;
                        }
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case '9':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        {
                        buf.msgID = TO_DATA_TASK_PWR_SUPPLY_QUERY;
                        break;
                        }
                    case 'u':
                        {
                        buf.msgID = TO_DATA_TASK_PWR_SUPPLY_UPDATE;
                        break;
                        }
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case 'a':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        {
                        buf.msgID = TO_DATA_TASK_TEACH_MODE_SET_QUERY;
                        break;
                        }
                    case 'u':
                        {
                        buf.msgID = TO_DATA_TASK_TEACH_MODE_SET_UPDATE;
                        break;
                        }
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case 'b':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        {
                        buf.msgID = TO_DATA_TASK_SYS_CONFIG_QUERY;
                        break;
                        }
                    case 'u':
                        {
                        buf.msgID = TO_DATA_TASK_SYS_CONFIG_UPDATE;
                        break;
                        }
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case 'c':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        buf.msgID = TO_DATA_TASK_ACTIVE_RECIPE_QUERY;
                        break;
                    case 'u':
                        buf.msgID = TO_DATA_TASK_ACTIVE_RECIPE_UPDATE;
                        break;
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case 'd':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        buf.msgID = TO_DATA_TASK_CONNECTIVITY_QUERY;
                        break;
                    case 'u':
                        buf.msgID = TO_DATA_TASK_CONNECTIVITY_UPDATE;
                        break;
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            case 'e':
                {
                switch(argv[2][0])
                    {
                    case 'b':
                        buf.msgID = TO_DATA_TASK_GATEWAY_SERVER_QUERY;
                        break;
                    default:
                        printf("invalid cmd:%s\n", argv[2]);
                        return 0;
                    }
                break;
                }
            default:
                printf("invalid cmd:%s\n", argv[1]);
                return 0;
            }
        msgQSend(mqidControl, (char *)&buf, sizeof(MESSAGE),-1,0);
        eventSend(tid, DATA_TASK_EVENT);
        }

    return 0;
    }

