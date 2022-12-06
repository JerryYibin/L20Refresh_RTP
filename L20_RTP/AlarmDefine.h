#include <iostream>
#include <memory>
typedef uint32_t UINT32;
typedef int32_t  INT32;
#define RECIPE_NAME_LENGTH   20
#define DATA_TIME_LENGTH	20

class AlarmLogDefine{
public:
	enum PARALIST{
		WELD_COUNT = 0,
		RECIPE_NAME,
		DATE_TIME,
		ALARM_TYPE,
	};
public:
    virtual int Set(int, void*) = 0;
    virtual int Get(int, void*) = 0;
    virtual void Init() 		= 0;
    static std::shared_ptr<AlarmLogDefine> GetAlarmLog();
};

class L20_AlarmLogDefine : public AlarmLogDefine{
public:
	unsigned int	WeldCount;
	char			RecipeName[RECIPE_NAME_LENGTH];
	char			DateTime[DATA_TIME_LENGTH];
	unsigned int	AlarmType;
public:
    int Set(int, void*) 	override;
    int Get(int, void*) 	override;
    void Init()				override;
};

class P1_AlarmLogDefine : public AlarmLogDefine{
public:
	UINT32  ResetRequired		: 	1;
	UINT32  LogAlarm			: 	1; 
	UINT32  GeneralAlarm		: 	1;
	UINT32  CustomAlarmOutput	: 	1;
	UINT32  CycleCounter		: 	1;
	UINT32  Persistent			: 	1;
	UINT32  Reserved			: 	2;
public:
    int Set(int, void*) 	override;
    int Get(int, void*) 	override;
    void Init()				override;
};
