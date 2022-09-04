/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
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

#define BIT(x)			(1<<x)
// helper macros for abs
#define _ABS(a) \
	({ __typeof__ (a) _a = (a); \
		_a < 0 ? (0 - _a) : _a;})


class Utilities
{
public:
    //um->mm
    static double convertMicrometreToMillimetre(double length);
    //mm->um
    static double convertMillimetreToMicrometre(double length);
    //s->ms
    static double convertSecToMsec(double time);
    //ms->s
    static double convertMsecToSec(double time);
    //inch->um
    static double convertInchesToMicrometre(double dbInches);
    //inch->mm
    static double convertInchesToMillimetre(double dbInches);
    //um->inch
    static double convertMicrometreToInches(double dbmicrometres);
    //mm->inch
    static double convertMillimetreToInches(double dbmillimetres);
    //N->lbs
    static double convertNtolbs(double dbNewtons);
    //lbs->N
    static double convertlbstoN(double dbLecibals);
};





























#endif
