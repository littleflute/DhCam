//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: utility.cpp, 2, 10/2/02 2:31:32 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
\file     utility.cpp
\brief   Implementation of utility functions and classes
*/
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "utility.h"


// greatest common divisor
int gcd( int num1, int num2 )
{
  assert ( num1 != 0 );
  int remainder = num2 % num1;
  
  if ( remainder != 0 )
    return gcd( remainder,num1 );
  
  return num1;
}

// least common multiple
int lcm( int num1, int num2 )
{
  return ( num1 * num2 ) / gcd(num1, num2);
}
