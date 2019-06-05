// CGVideoEx.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"



/*
		Global variable 
 */


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH){
      	
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		
	}

	return TRUE;
}








