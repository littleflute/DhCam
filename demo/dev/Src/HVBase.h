// HVBase.h: interface for the CHVBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HV_H__F2EAD769_A3AB_48E2_99A7_CF32024FCA7A__INCLUDED_)
#define AFX_HV_H__F2EAD769_A3AB_48E2_99A7_CF32024FCA7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <HVDAILT.h>
#include <HVUnknown.h>

#include "Exception.h"
#include "PropertyBag.h"
#include "HVUtil.h"
#include "Shlwapi.h"

//#pragma comment(lib,"..//Lib//hvdailt.lib")

/// the bcam namespace
namespace HVDevice
{
	/** These are the color modes, as defined by the Digital Device Specification. */
// 	typedef enum
// 	{
// 		Res_IgnoreVideoMode = -1,
// 		Res_Mode0 = 0,
// 		Res_Mode1 = 1,
// 		Res_Mode2 = 2,
// 		Res_Mode3 = 3,
// 		Res_Mode4 = 4,
// 		Res_Mode5 = 5,
// 		Res_Mode6 = 6,
// 		Res_Mode7 = 7
// 	}	HVResolution;
	
	typedef enum {
		UNIT_MICROSECOND = 0,
		UNIT_MILLISECOND,
		UNIT_SECOND
	} TUnit;
	
	/**
	* \define Used by nested classes to get access to the outer class
	*/
#define IMPLEMENT_THIS(OUTER, MEMBER_VARIABLE) \
	OUTER* This() \
	{ \
	return (OUTER*)((BYTE*)this - offsetof(OUTER, MEMBER_VARIABLE)); \
		};
	
	/**------------------------------------------------------------------
	* class:       BcamUtility
	*
	* \brief     Helper class which offers the user some convenience 
	*
	*/
	//--------------------------------------------------------------------
	
	class HVBaseUtility
	{
	public:
		/// Mapping DCSColorCode to number of bits per pixel
		static unsigned short BitsPerPixel(HV_COLOR_CODE ColorCode);

	};
	
	//! Wrapper class for the Win32 thread api
	class CThread
	{
	public:
		
		//! Default constructor 
		CThread()
		{
			m_hThread = NULL;
			m_ID = 0;
			m_bSuspended = false;
		}
		
		//! destructor
		virtual ~CThread() 
		{ 
			Release(1000); 
		}
		
		//! Creates a thread
		BOOL Create(
			LPTHREAD_START_ROUTINE pProcess,        //!< Thread function
			LPVOID pParam=NULL,                     //!< Parameter handed over to the thread function
			int Priority=THREAD_PRIORITY_NORMAL,    //!< The thread's priority
			DWORD CreationFlags = 0                 //!< Flags controlling the thread's creation
			)
		{
			assert(pProcess);
			m_hThread = ::CreateThread(NULL, 0, pProcess, pParam, CreationFlags, &m_ID);
			ATLTRACE("CThread.Create(). Handle = %x, ID = %x\n", m_hThread, m_ID);
			m_bSuspended = CreationFlags && CREATE_SUSPENDED;
			if( m_hThread==NULL ) 
				return FALSE;
			if( !::SetThreadPriority(m_hThread, Priority) ) {
				::CloseHandle(m_hThread);
				return FALSE;
			}
			return TRUE;
		}
		
		//! Close the thread handle and wait for the thread to die
		BOOL Release(DWORD timeout = INFINITE)
		{
			if( m_hThread==NULL ) 
				return TRUE;
			if ( WaitForSingleObject(m_hThread, timeout) != WAIT_OBJECT_0 )
			{
				// The thread didn't die. Terminate it
				ATLTRACE("Must terminate thread. Handle = %x\n", m_hThread);
				Terminate(0);
				WaitForSingleObject(m_hThread, 10000);
			}
			
			return Detach();
		}
		
		//! Close the thread handle, but don't wait for the thread to die
		BOOL Detach()
		{
			if ( m_hThread == NULL )
				return TRUE;
			if( ::CloseHandle(m_hThread)==FALSE ) 
				return FALSE;
			m_hThread = NULL;
			m_ID = 0;
			return TRUE;
		}
		
		//! Set the thread's priority
		BOOL SetPriority(int Priority)
		{
			assert(m_hThread);
			return ::SetThreadPriority(m_hThread, Priority);
		}
		
		//! Suspend the thread
		BOOL Suspend()
		{
			assert(m_hThread);
			if( m_bSuspended ) 
				return TRUE;
			if( ::SuspendThread(m_hThread)==-1 ) 
				return FALSE;
			m_bSuspended = true;
			return TRUE;
		}
		
		//! Resume the thread
		BOOL Resume()
		{
			assert(m_hThread);
			if( !m_bSuspended ) return TRUE;
			if( ::ResumeThread(m_hThread)==-1 ) return FALSE;
			m_bSuspended = false;
			return TRUE;
		}
		
		//! Terminate the thread (don't...)
		BOOL Terminate(DWORD dwExitCode)
		{
			assert(m_hThread);
			return ::TerminateThread(m_hThread, dwExitCode);
		}
		
		//! Get the thread's exit code
		BOOL GetExitCode(DWORD *pExitCode)
		{
			assert(m_hThread);
			assert(pExitCode);
			return ::GetExitCodeThread(m_hThread, pExitCode);
		}
		
		//! Get the thread's handle
		operator HANDLE() 
		{ 
			return m_hThread; 
		}
		
		//! Get the thread's ID
		operator DWORD()
		{
			return m_ID;
		}
		
		//! Check if the thread is currently suspended
		bool IsSuspended() 
		{ 
			return m_bSuspended; 
		}
		
	private:
		//! The thread's handle
		HANDLE m_hThread;
		
		//! The thread's ID
		DWORD m_ID;
		
		//! Indicates if the thread is currently suspended
		bool m_bSuspended;
		
	};   ///CThread
	
	//! Wrapper class for the Win32 events
	class CEvent
	{
		HANDLE m_hEvent;
		
	public:
		CEvent()
		{
			m_hEvent = NULL;
		}
		
		CEvent( const CEvent & src )
		{
			Duplicate(&m_hEvent, src.m_hEvent);
		}
		
		CEvent & operator =(const CEvent& src)
		{
			if ( m_hEvent != NULL )
				::CloseHandle(m_hEvent);
			Duplicate(&m_hEvent, src.m_hEvent);
			return *this;
		}
		
		BOOL Create(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPSECURITY_ATTRIBUTES lpEventAttributes = NULL, LPCTSTR lpName = NULL)
		{
			m_hEvent = ::CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
			return m_hEvent != NULL;
		}
		
		BOOL Set()
		{
			return ::SetEvent(m_hEvent);
		}
		
		BOOL Reset()
		{
			return ::ResetEvent(m_hEvent);
		}
		
		operator HANDLE() const
		{ 
			
			
			return m_hEvent;
		}
		
		HANDLE* operator &()
		{ 
			return &m_hEvent;
		}
		
		
		~CEvent()
		{
			if ( m_hEvent != NULL )
			{
				CloseHandle(m_hEvent);
				m_hEvent = NULL;
			}
		}
		
		
	private:
		BOOL Duplicate(HANDLE * phDest, HANDLE hSource)
		{
			if (hSource == NULL ) {
				
				*phDest = hSource; 
				
				return TRUE;
				
			} else {
				
				return ::DuplicateHandle(
					GetCurrentProcess(), hSource,
					GetCurrentProcess(), phDest,
					0,                        
					FALSE,                    
					DUPLICATE_SAME_ACCESS);
			}
		}
	}; // CEvent
	
	
	class HVBaseException : public Bvc::Exception
	{
	public:
		HVBaseException( HVSTATUS e, CString Context = "", va_list *pArgs = NULL);
		HVBaseException( DWORD e, CString Context = "", va_list *pArgs = NULL );
		virtual ~HVBaseException(){}
		
		/// retrieve the error code
		DWORD Error() const { return m_Error; }
		/// retrieve the error message
		CString Description() const { return m_Description; }
		/// retrieve additional context information
		CString Context() const { return m_Context; }
		
	protected:
		void FormatMessage( HVSTATUS e, CString Context );
	};
	
	
	
	class CHVBase  
	{
		typedef std::map<HANDLE, CHVBase*> HVBaseMap_t ;
		
	public:
		CHVBase();
		
	private: // copying prohibited
		CHVBase( const CHVBase& );
		CHVBase& operator=( const CHVBase& );
		
	public:
		virtual ~CHVBase();
		
		//	static int GetDeviceTotal();
		//	static void GetTypeFromIndex(int nNumber, HVTYPE *pType, int *pIndex);
		//	static CString GetErrorString(HVSTATUS e);
		
		// Is the driver open
		bool IsOpen();
		// Opens the driver
		void Open( int nNumber, HWND hWndNotify = NULL );
		// Closes the driver and frees all resources (esp. OVERLAPPED structures from cancelled driver calls)
		void Close();
		// Initialize camera
		void Initialize(); 
		
		protected:
			void SetResolution(HV_VIDEO_MODE Mode);
			void SetOutputWindow(int nLeft, int nTop, int nWidth, int nHeight);
			void AGCControl(BYTE byParam, long lValue);
			void AECControl(BYTE byParam, long lValue);
			void AECControl_HV3(BYTE byParam, long lValue);
			void ADCControl(BYTE byParam, long lValue);
			
			void SnapShot(BYTE **ppBuffer, int nNumber);
			
			void OpenSnap(HV_SNAP_PROC lpSnapFunc, void *pParam);
			void CloseSnap();
			void StartSnap(BYTE **ppBuffer, size_t BufferNumber);
			void StopSnap();

			void SetSnapMode(HV_SNAP_MODE mode);
			void SetTriggerPolarity(HV_POLARITY polarity);
			void SetStrobePolarity(HV_POLARITY polarity);
			void SetSnapSpeed(HV_SNAP_SPEED speed);
			void SetBlank(int hor, int vert);

		public:
			BOOL GetDeviceInfo(HV_DEVICE_INFO Param, void *pContext, int *pSize);
//			HVSTATUS GetDeviceIPAdd( HV_RES_GET_DEVICE_IP ResGetDevIP);
			HHV	GetHandle(){return m_hDevice;}			
		protected:
			// is the camera object valid
			bool IsValid();
			
			HHV					m_hDevice;             ///< handle to the opened device 
			int					m_nNumber;				///< friendly devcie name of device we have opened
			static HVBaseMap_t	s_HVBaseMap;             ///< map the open devices
			static HWND			s_hWndNotify;          ///< window handle for which we register device notifications
//			HVSTATUS			m_OpenStatus;			//xupx 2008.07.31
			int					m_Counter;
	};
}

#endif // !defined(AFX_HV_H__F2EAD769_A3AB_48E2_99A7_CF32024FCA7A__INCLUDED_)
