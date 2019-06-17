/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			Exception.h                                         */
/*  Description:	This file define the exception class                */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-20                                          */
/*                                                                      */
/************************************************************************/

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

//////////////////////////////////////////////////////////////////////////

class CPropertyBagException
{
public:
	CPropertyBagException(DWORD code, CString description){};
};

#endif