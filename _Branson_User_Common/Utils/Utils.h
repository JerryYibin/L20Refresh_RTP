/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
/*
  * Utils.h
 *
 *  Created on: Feb 6, 2018
 *      Author: DShilonie
 *      
 *  Prototypes for various utilities and quick type safe useful macros that are used
 *  for all components, should be here.
 *  
 */

#ifndef UTILS_H_
#define UTILS_H_

// helper macros for boundary checking
#define _CHECKMAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
     
#define _CHECKMIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

// helper macros for function entry conditions 
#define _CHECK(expr) \
    if (!(expr)) \
		return;

#define _CHECKRETURN(expr,rtn) \
    if (!(expr)) \
		return rtn;

#define BIT_MASK(x)		(1<<x)






class Utilities
{

public:

    static Utilities* getInstance();

public:
    //um->mm
    double convertMicrometreToMillimetre(double length);
    //mm->um
    double convertMillimetreToMicrometre(double length);
    //s->ms
    double convertSecToMsec(double time);
    //ms->s
    double convertMsecToSec(double time);
    //inch->um
    double convertInchesToMicrometre(double dbInches);
    //inch->mm
    double convertInchesToMillimetre(double dbInches);
    //um->inch
    double convertMicrometreToInches(double dbmicrometres);
    //mm->inch
    double convertMillimetreToInches(double dbmillimetres);
    //N->lbs
    double convertNtolbs(double dbNewtons);
    //lbs->N
    double convertlbstoN(double dbLecibals);


private:

    explicit Utilities();

private:

    static Utilities* m_utilities;
};





























#endif
