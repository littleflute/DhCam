//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: ChkRecursion.cpp, 2, 10/2/02 2:31:24 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
\file     ChkRecursion.cpp
\brief    implementation of the CChkRecursion class.
*/
//////////////////////////////////////////////////////////////////////

#include "ChkRecursion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CChkRecursion::CChkRecursion(int *pnRecursionCounter)
{
	m_pnRecursionCounter = pnRecursionCounter;
	if ( NULL != m_pnRecursionCounter )
		(*m_pnRecursionCounter)++;
}

CChkRecursion::~CChkRecursion()
{
	if ( NULL != m_pnRecursionCounter && *m_pnRecursionCounter > 0 )
		(*m_pnRecursionCounter)--;
}
