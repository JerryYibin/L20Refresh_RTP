/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef VERSION_H_
#define VERSION_H_

#include "Common.h"
//#include "ActuatorTask.h"

/*
 * System version components
 */
typedef enum {
	FW_VERSION_SC,
	FW_VERSION_AC,
	FW_VERSION_PC,

	FW_VERSION_MAX
} FW_VER_IDX;

/*
 * CRC component
 */
typedef enum {
	FW_CRC_SC,
	FW_CRC_AC,
	FW_CRC_PC,

	FW_CRC_MAX
} FW_CRC_IDX;

/*
 * Info component
 */
typedef enum {
	FW_INFO_SC,
	FW_INFO_AC,
	FW_INFO_PC,
	FW_INFO_UIC,

	FW_INFO_MAX
} FW_INFO_IDX;

/*
 * Type of version
 */
typedef enum {
	VERSION_MAJOR,
	VERSION_MINOR,
	VERSION_BUILD,

	VERSION_FIELD_MAX
} VER_FIELD_IDX;

/*
 * Provides global access to version info, no need to instantiate.
 */
class FWVersion
{
public:
	
	FWVersion ();
	~FWVersion ();
	
	static UINT16 	Get (FW_VER_IDX idx, VER_FIELD_IDX field);
	static void 	Set (FW_VER_IDX idx, VER_FIELD_IDX field, UINT16 val);
	static void 	PrintVersion (FW_VER_IDX idx);

private:

	static UINT16 versions[FW_VERSION_MAX][VERSION_FIELD_MAX];	
};


class FWSerial
{
public:
	
	FWSerial ();
	~FWSerial ();
	
	static char * 	Get (FW_VER_IDX idx);
	static void 	Set (FW_VER_IDX idx, char * serialNo);

private:
	
	//static char serial_ac[SER_VER_NO_SERIAL_SIZE];	
	//static char serial_pc[SER_VER_NO_SERIAL_SIZE];	
	//static char serial_asc[SER_VER_NO_SERIAL_SIZE];	
};

/*
 * Provides global access to CRC info, no need to instantiate.
 */
class FWCrc
{
public:
	
	FWCrc ();
	~FWCrc ();
	
	static UINT16 	Get (FW_CRC_IDX);
	static void 	Set (FW_CRC_IDX idx, UINT16 val);

private:

	static UINT16 crc[FW_CRC_MAX];	
};

/*
 * Provides global access to firmware info (compatibility, restrictions etc.), no need to instantiate.
 */
class FWInfo
{
public:
	
	FWInfo ();
	~FWInfo ();
	
	static UINT32 	Get (FW_INFO_IDX);
	static void 	Set (FW_INFO_IDX idx, UINT32 val);

private:

	static UINT32 info[FW_INFO_MAX];	
};

#endif /* VERSION_H_ */
