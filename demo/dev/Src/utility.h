//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: utility.h, 5, 10/2/02 2:31:32 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
  \file     utility.h
 *
 * Utility functions and classes
 *
  \brief   Utility functions and classes
 */
//-----------------------------------------------------------------------------

#pragma once

#include <vector>


#define CATCH_REPORT() \
	catch ( HVBaseException& e ) \
		{ \
		ReportError(e); \
			} \
			catch ( ... ) \
		{ \
		HVBaseException e(STATUS_INTERNAL_ERROR); \
		ReportError(e); \
	} 







//------------------------------------------------------------------------------
// class CCriticalSection
// Author: 
// Date: 
//------------------------------------------------------------------------------
/**
 * \brief   Guard for a critical section
 *
 */
//------------------------------------------------------------------------------

class CCriticalSection
{
public:
  /// Enter critical section and lock it
  void Lock() {EnterCriticalSection(&m_sec);}
  /// Nonblocking try to enter the critical section. If the function succeeds, the critical
  /// section is entered
//  BOOL TryLock() {return TryEnterCriticalSection(&m_sec);}
  /// Leave critical section and unlock it
  void Unlock() {LeaveCriticalSection(&m_sec);}
  /// Create critical section and initialize it.
  CCriticalSection() {InitializeCriticalSection(&m_sec);}
  /// Destroy critical section
  ~CCriticalSection() {DeleteCriticalSection(&m_sec);}
  /// Win32 critiical section
  CRITICAL_SECTION m_sec;
};


//------------------------------------------------------------------------------
// class CMovingAvg
// Author: 
// Date: 
//------------------------------------------------------------------------------
/**
 * \brief   Template class to calculate a moving average
 *
 */
//------------------------------------------------------------------------------

template <class T, unsigned int span = 10>
class CMovingAvg
{
  double m_Sum;
  unsigned int m_n;
  std::vector<T> m_v;
  CCriticalSection m_CritSect;


public:
  CMovingAvg() : m_v(span) 
  {
    Reset();
  }

  void Add(T sample) 
  { 
    m_CritSect.Lock();
    int idx = m_n % span;
    if ( m_n < span )
    {
      m_v[idx] = sample;
      m_Sum += sample;
    }
    else
    {
      m_Sum -= m_v[idx];
      m_Sum += sample;
      m_v[idx] = sample;
    }
    m_n++;

    m_CritSect.Unlock();
  }

  double Avg() {
    double res = 0;
    m_CritSect.Lock();
    if ( m_n != 0 )
    {
       res = m_n < span ? m_Sum / (double) m_n : m_Sum / (double) span;
    }
    m_CritSect.Unlock();
    return res;
  }

  void Reset() {
    m_CritSect.Lock();
    m_Sum = 0.0;
    m_n = 0;
    m_CritSect.Unlock();
  }

};



//------------------------------------------------------------------------------
// class CStopWatch
// Author: 
// Date: 
//------------------------------------------------------------------------------
/**
 * \brief   Measurement of time intervalls
 *
 */
//------------------------------------------------------------------------------


class CStopWatch
{
public:
  //////////////////////////////////////////////////////////////////////////////////////////
  // cconstructor, starts time measurement
  CStopWatch()
  {
    Start();
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Start the stop watch 
  void Start()
  {
     QueryPerformanceCounter(&m_StartTime);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Stop. The elapsed time is returned. The stop watch may be started againn neuen Zeitpunkt
  double Stop(bool StartAgain)
  {
    QueryPerformanceCounter(&m_StopTime);
    double theElapsedTime = ElapsedTime();
    if(StartAgain)
      m_StartTime = m_StopTime; 
    return theElapsedTime;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Return the elapsed time in seconds between start() and stop()
  double ElapsedTime()
  {
    LARGE_INTEGER timerFrequency;
    QueryPerformanceFrequency(&timerFrequency);
  
    __int64 oldTicks = ((__int64)m_StartTime.HighPart << 32) + (__int64)m_StartTime.LowPart;
    __int64 newTicks = ((__int64)m_StopTime.HighPart << 32) + (__int64)m_StopTime.LowPart;
    long double timeDifference = (long double) (newTicks - oldTicks);

    long double ticksPerSecond = (long double) (((__int64)timerFrequency.HighPart << 32) 
				  + (__int64)timerFrequency.LowPart);

    return (double)(timeDifference / ticksPerSecond);
  }

protected:
  // zero-point for time measurment
  LARGE_INTEGER m_StartTime;

  // last time stamp
  LARGE_INTEGER m_StopTime;
};


//------------------------------------------------------------------------------
// gcd( int num1, int num2 );
// Author: 
//------------------------------------------------------------------------------
/**
 * Calculate greatest common divisor of two numbers
 *
 * \param     num1  
 * \param     num2  
 * \return    greatest common divisor of num1 and num2
 *
 */
//------------------------------------------------------------------------------
int gcd( int num1, int num2 );

//------------------------------------------------------------------------------
// int lcm( int num1, int num2 )
// Author: 
//------------------------------------------------------------------------------
/**
 * Calculate least common multiple of two numbers
 *
 * \param     num1
 * \param     num2
 * \return    least common multiple of num1 and num2
 *
 */
//------------------------------------------------------------------------------

int lcm( int num1, int num2 );






