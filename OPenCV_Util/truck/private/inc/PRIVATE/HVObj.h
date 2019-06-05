/*
 *-----------------------------------------------------------------------------
 *       Name				HVObj.H
 *       Purpose			Video obj definition
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2002/12/7
 *
 *
 *       Description
 *
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(HV_HVVIDEOOBJ)
#define HV_HVVIDEOOBJ

#include <HVDef.h>

//#define TRANSFER_DEBUG

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


#define WIDTH_BYTES(bits) (((bits) + 31) / 32 * 4)
#define PIXEL_BYTES(bits) (((bits) + 7) / 8 )
#define PIXEL_BITS(bits) (((bits) + 7) / 8 * 8)


#define RECT_WIDTH(lpRect) ((lpRect)->right - (lpRect)->left)
#define RECT_HEIGHT(lpRect) ((lpRect)->bottom - (lpRect)->top)

#define ABSOLUTE_ZERO	0.000000001

#define VERIFY_POINTER(p, status)	\
		if ((p) == NULL){	return (status); }	

#define GET_DEVICE_TYPE_NUMBER()	(HV1351UMTYPE - HV1300UCTYPE + 1)

typedef HANDLE HHV1300UC;

typedef enum tagMAX_VALUE{ 
		
	MAX_STRING      	= 256,
	MAX_COUNT			= 2000,
	DEVICE_MAX_NUMBER	= 16,
	
//	MAX_DEVICE_TYPE		= 10
		
} MAX_VALUE;



typedef struct tagHVFuncIndex {
	
	char chFuncName[MAX_STRING];
	HVTYPE type;
	
} HVFuncIndex;



typedef enum tagHV_STATUS{ 
	
	HW_NONE			= 0, 
	HW_SNAP_INT		= 1, 
	HW_SNAP			= 2,
	HW_OTHER		= 3
		
} HV_STATUS;


typedef unsigned ( __stdcall * PTHREAD_START)( void * );

typedef LONG USBD_STATUS;

typedef struct _USBD_ISO_PACKET_DESCRIPTOR{
    ULONG Offset;       // INPUT Offset of the packet from the begining of the
	// buffer.
	
    ULONG Length;       // OUTPUT length of data received (for in).
	// OUTPUT 0 for OUT.
    USBD_STATUS Status; // status code for this packet.     
}USBD_ISO_PACKET_DESCRIPTOR, *PUSBD_ISO_PACKET_DESCRIPTOR;


typedef struct _ISO_TRANSFER_CONTROL{
	//
	// pipe number to perform the ISO transfer to/from.  Direction is
	// implied by the pipe number.
	//
	DWORD	PipeNum;
	//
	// ISO packet size.  Determines how much data is transferred each
	// frame.  Should be less than or equal to the maxpacketsize for
	// the endpoint.
	//
	DWORD	PacketSize;
	//
	// Total number of ISO packets to transfer.
	//
	DWORD	PacketCount;
	//
	// The following two parameters detmine how buffers are managed for
	// an ISO transfer.  In order to maintain an ISO stream, the driver
	// must create at least 2 transfer buffers and ping pong between them.
	// BufferCount determines how many buffers the driver creates to ping
	// pong between.  FramesPerBuffer specifies how many USB frames of data
	// are transferred by each buffer.
	//
	DWORD	FramesPerBuffer;     // 10 is a good value
	DWORD	BufferCount;         // 2 is a good value
}ISO_TRANSFER_CONTROL, *PISO_TRANSFER_CONTROL;


// Define HV device id of I/O commamd
#define IOCMD_HV_DEVICE_TYPE			0x00
#define IOCMD_HV_DEVICE_SERIESNUM		0x01
#define IOCMD_HV_TRANSFER_TYPE			0x02
#define IOCMD_HV_CHIP_ID				0x03  
#define IOCMD_HV_HARDWARE_VERSION		0x04
#define IOCMD_HV_RESOLUTION				0x10
#define IOCMD_HV_BLANK_SIZE				0x12


#define IOCMD_HV_RES_OUTWND_RATIO		0x11
#define IOCMD_HV_ISO_TRANSFER_CONTROL	0x30
#define IOCMD_HV_SYNC_SIGN				0x31


//the handle structure       
typedef struct tagHVOBJ {
	
	HANDLE				hDevice;
	HVTYPE				type;
	int					nDevice;
	
	HV_STATUS			HvStatus;
	HV_RESOLUTION	    resolution;
	
	BOOL				bISOTransfer;
	int					nRsCoef;			//Resolution coefficent
	DWORD				dwImageSize;		//Image size
	DWORD				dwIdealSize;		//Transfer size

	DWORD				dwDataRead;
	DWORD				dwSyncSign;
	ISO_TRANSFER_CONTROL IsoControl;

	HV_SNAP_PROC		lpSnapFunc;
	void				*pSnapParam;
	BYTE				**ppSnapBuffer;
	int					CurrentIndex;
	int					BufferNumber;
	BOOL				fSuspendGrab;
	HANDLE				hSnapThread;		//Snap Thread handle
	unsigned			uThreadID;			//Snap Thread ID
	BOOL				fContinuousGrabActive;
	int					nLastTransferStatus;

	CRITICAL_SECTION	csSnapShot;
	CRITICAL_SECTION	csWaitSync;
	CRITICAL_SECTION	csOpen;
	CRITICAL_SECTION	csClose;
	CRITICAL_SECTION	csStart;
	CRITICAL_SECTION	csStop;

	HANDLE				evtStop;
	BOOL				bEvtSet;

	BOOL				bLock;
	
} HVOBJ;



typedef enum tagHV_CTRL_CODE { 
	DEVCTRL_SECRETKEY_LEVEL			= 0,
	DEVCTRL_SECRETKEY_LENGTH		= 1,
	DEVCTRL_READ_SECRETKEY			= 2,
	DEVCTRL_STATUS_CHECK_SECRETKEY	= 3,
	DEVCTRL_CLEAR_SECRETKEY			= 4
} HV_CTRL_CODE;


#endif













