/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
/*
  * Utils.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: DShilonie
 *      
 *  Implementations for various utilities should be here.
 *  
 */
 

/* Header files */
#include "Utils.h"


 /**************************************************************************//**
 *
 * \brief Utilities::convertSecToMsec: Convert seconds to millseconds
 *
 * \param time
 *
 * \return ms
 *
 ******************************************************************************/
double Utilities::convertSecToMsec(double time)
{
    double tmp;
    tmp = time * (double)1000;
    return tmp;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertMsecToSec: Convert Milliseconds to seconds
 *
 * \param time
 *
 * \return s
 *
 ******************************************************************************/
double Utilities::convertMsecToSec(double time)
{
    double tmp;
    tmp = time / (double)1000;
    return tmp;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertMicrometreToMillimetre: ConvertMicrometreToMillimetre
 *
 * \param length
 *
 * \return mm
 *
 ******************************************************************************/
double Utilities::convertMicrometreToMillimetre(double length)
{
    double tmp;
    tmp = length / (double)1000;
    return tmp;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertMillimetreToMicrometre : Convert Millimetre To Micrometre
 *
 * \param length
 *
 * \return um
 *
 ******************************************************************************/
double Utilities::convertMillimetreToMicrometre(double length)
{
    double tmp;
    tmp = length * (double)1000;
    return tmp;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertinchestomicrometre : Convert inches to micrometre
 *
 * \param dbInches
 *
 * \return um
 *
 ******************************************************************************/
double Utilities::convertInchesToMicrometre(double dbInches)
{
    double dbMicrometre;
    dbMicrometre = dbInches * (double)25400;
    return dbMicrometre;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertinchestomillimetre : Convert inches to millimetre
 *
 * \param dbInches
 *
 * \return mm
 *
 ******************************************************************************/
double Utilities::convertInchesToMillimetre(double dbInches)
{
    double dbMillimetre;
    dbMillimetre = dbInches * (double)25.4;
    return dbMillimetre;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertmicrometretoinches : Convert micrometre to inches
 *
 * \param dbmicrometres
 *
 * \return inch
 *
 ******************************************************************************/
double Utilities::convertMicrometreToInches(double dbmicrometres)
{
    double dbInches;
    dbInches = dbmicrometres / (double)25400;
    return dbInches;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertmillimetretoinches : Convert millimetre to inches
 *
 * \param dbMillimeter
 *
 * \return inch
 *
 ******************************************************************************/
double Utilities::convertMillimetreToInches(double dbMillimeter)
{
    double dbInches;
    dbInches = dbMillimeter / (double)25.4;
    return dbInches;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertNtolbs : Convert Newtons to LECIBALS
 *
 * \param dbNewtons
 *
 * \return lbs
 *
 ******************************************************************************/
double Utilities::convertNtolbs(double dbNewtons)
{
    double dbLecibals;
    dbLecibals = dbNewtons / (double)4.44822;
    return dbLecibals;
}

 /**************************************************************************//**
 *
 * \brief Utilities::convertlbstoN : Convert LECIBALS to Newtons
 *
 * \param dbLecibals
 *
 * \return N
 *
 ******************************************************************************/
double Utilities::convertlbstoN(double dbLecibals)
{
    double dbNeutons;
    dbNeutons = dbLecibals * (double)4.44822;
    return dbNeutons;
}
