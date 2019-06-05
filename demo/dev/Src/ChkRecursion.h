//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: ChkRecursion.h, 3, 10/2/02 2:31:25 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
\file     ChkRecursion.h
\brief    interface for the CChkRecursion class.
*/

#if !defined(AFX_CHKRECURSION_H__469B1733_FC1F_11D4_8EF7_00105AC44283__INCLUDED_)
#define AFX_CHKRECURSION_H__469B1733_FC1F_11D4_8EF7_00105AC44283__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>

class CChkRecursion  
{
	int                     * m_pnRecursionCounter;
	
public:
	CChkRecursion(int *pnRecursionCounter);
	virtual                   ~CChkRecursion();
	
	inline bool               Check(int nLimit) 
	{ 
		return (NULL != m_pnRecursionCounter && *m_pnRecursionCounter > nLimit); 
	};
	
};

#endif // !defined(AFX_CHKRECURSION_H__469B1733_FC1F_11D4_8EF7_00105AC44283__INCLUDED_)
