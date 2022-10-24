/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/
#ifndef L20SCDGTINPUTTASK_H_
#define L20SCDGTINPUTTASK_H_

#include "ScDgtInput.h"
#define DEBOUNCE_TIME 50
class L20ScDgtInputTask: public ScDgtInputTask 
{
public:
							L20ScDgtInputTask		();
	virtual 				~L20ScDgtInputTask		();
protected:
	virtual void			GetDgtInputBits			() override;
private:
	unsigned int			m_OnHoldDebounce[BITS_SIZE];
	unsigned int			m_ReleaseDebounce[BITS_SIZE];
	void					BitsDebounce();
};

#endif /* L20SCDGTINPUTTASK_H_ */
