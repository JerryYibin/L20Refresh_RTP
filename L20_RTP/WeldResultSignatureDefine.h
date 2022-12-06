/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#include <memory>
#ifndef WELDRESULTSIGNATURE_H_
#define WELDRESULTSIGNATURE_H_

#define HMI_SIGNA_POINT_MAX 200

enum SIGNATURE_DATA_TYPE
{
	FRQUENCY = 0,
	POWER,
	HEIGHT,
	AMPLITUDE,
	TOTAL
};

struct WELD_SIGNATURE
{
	unsigned int Frquency;
	unsigned int Power;
	unsigned int Height;
	unsigned int Amplitude;
};

struct HMI_WELD_SIGNATURE
{
	unsigned int Frquency;
	unsigned int Power;
	unsigned int Height;
	unsigned int Time;
	unsigned int length;
};

/* TODO The above definition is to ensure that the current code can run normally. 
 * After the code review, it will be changed to the new definition structure below.*/
using namespace std;
class WeldResultSignatureDefine{
protected:
	unsigned int 	Frquency;
	unsigned int 	Amplitude;
	unsigned int	Power;
public:
	enum PARA_LIST {
		HEIGHT = 0,
		FRQUENCY,
		AMPLITUDE,
		POWER,
		TIME,
		CURRENT,
		PHASE,
		TOTAL
	};
public:
    virtual int 			Set(const int index, const unsigned int para) 		= 0;
    virtual unsigned int 	Get(const int index, int* ok = nullptr)				= 0;
    virtual void			Init()												= 0;
    static shared_ptr<WeldResultSignatureDefine> GetWeldSignature();
    WeldResultSignatureDefine(){};
    virtual ~WeldResultSignatureDefine(){}
};

class L20_WeldResultSignatureDefine : public WeldResultSignatureDefine{
private:
	unsigned int	Height;
public:
    virtual int 			Set(const int index, const unsigned int para) 	override;
    virtual unsigned int	Get(const int index, int* ok = nullptr) 		override;
    virtual void			Init()											override;
};

class P1_WeldResultSignatureDefine : public WeldResultSignatureDefine
{
private:
	unsigned int	Time;
	unsigned int 	Current;
	unsigned int  	Phase;
public:
    virtual int 			Set(const int index, const unsigned int para) 	override;
    virtual unsigned int 	Get(const int index, int* ok = nullptr) 		override;
    virtual void 			Init()											override;
};

#endif /* WELDRESULTSIGNATURE_H_ */
