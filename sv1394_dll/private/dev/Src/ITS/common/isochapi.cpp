//File : isochapi.cpp

#include "CAMOBJ.h"


DWORD IsochFreeBandwidth
(TCHAR* szDeviceName, HANDLE  hBandwidth)
{
    HANDLE hDevice;
    DWORD  dwRet=0, dwBytesRet;

#ifdef _DEBUGISOCH
    _RPT1(_CRT_WARN, "[IsochFreeBandwidth] hBandwidth = 0x%x\r\n", hBandwidth);
#endif

    //Qiao,2005.4.28
    //hDevice = OpenDevice(szDeviceName, FALSE);
    hDevice = OpenDevice(szDeviceName, TRUE);
    //End,Qiao,2005.4.28

    if (hDevice != INVALID_HANDLE_VALUE)
    {
        OVERLAPPED StopOverlapped = { 0 };

        StopOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_ISOCH_FREE_BANDWIDTH,
                                 &hBandwidth,
                                 sizeof(HANDLE),
                                 NULL,
                                 0,
                                 &dwBytesRet,
                                 &StopOverlapped
                               );

        if (!GetOverlappedResult(hDevice,&StopOverlapped,&dwBytesRet,TRUE))
        {
            _RPT1(_CRT_WARN,"IsochFreeBandwidth: Error %08x on Overlapped Isoch Stop",GetLastError());
        }

        CloseHandle(StopOverlapped.hEvent);

        if (!dwRet)
        {
            dwRet = GetLastError();
        }
        else
        {
            dwRet = ERROR_SUCCESS;
        }

        CloseHandle(hDevice);
    }

    return(dwRet);
}

DWORD
IsochFreeChannel
(TCHAR* szDeviceName, DWORD nChannel)
{
    HANDLE      hDevice;
    DWORD       dwRet=0, dwBytesRet;

#ifdef _DEBUGISOCH
    _RPT1(_CRT_WARN, "[IsochFreeChannel] nChannel = 0x%x\r\n", nChannel);
#endif

    //Qiao,2005.4.28
    //hDevice = OpenDevice(szDeviceName, FALSE);
    hDevice = OpenDevice(szDeviceName, TRUE);
    //End,Qiao,2005.4.28

    if (hDevice != INVALID_HANDLE_VALUE)
    {
        OVERLAPPED StopOverlapped = { 0 };

        StopOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_ISOCH_FREE_CHANNEL,
                                 &nChannel,
                                 sizeof(DWORD),
                                 NULL,
                                 0,
                                 &dwBytesRet,
                                 &StopOverlapped
                               );

        if (!GetOverlappedResult(hDevice,&StopOverlapped,&dwBytesRet,TRUE))
        {
            // Some error, but we cannot return here.
#ifdef _DEBUGISOCH
            _RPT1(_CRT_WARN, "IsochFreeChannel: Error %08x on Overlapped Isoch Stop",GetLastError());
#endif
        }

        CloseHandle(StopOverlapped.hEvent);

        if (!dwRet)
        {
            dwRet = GetLastError();
        }
        else
        {
            dwRet = ERROR_SUCCESS;
        }

        CloseHandle(hDevice);
    }

    return(dwRet);
}

DWORD
IsochFreeResources
(TCHAR* szDeviceName, HANDLE  hResource)
{
    HANDLE      hDevice;
    DWORD       dwRet=0, dwBytesRet;

#ifdef _DEBUGISOCH
    _RPT1(_CRT_WARN, "[IsochFreeResources] hResource = 0x%x\r\n", hResource);
#endif

    //Qiao,2005.4.28
    //hDevice = OpenDevice(szDeviceName, FALSE);
    hDevice = OpenDevice(szDeviceName, TRUE);
    //End,Qiao,2005.4.28

    if (hDevice != INVALID_HANDLE_VALUE)
    {
        OVERLAPPED StopOverlapped = { 0 };

        StopOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_ISOCH_FREE_RESOURCES,
                                 &hResource,
                                 sizeof(HANDLE),
                                 NULL,
                                 0,
                                 &dwBytesRet,
                                 &StopOverlapped
                               );

        if (!GetOverlappedResult(hDevice,&StopOverlapped,&dwBytesRet,TRUE))
        {
            // Some error, but we cannot return here.
#ifdef _DEBUGISOCH
            _RPT1(_CRT_WARN, "IsochFreeResources: Error %08x on Overlapped Isoch Stop",GetLastError());
#endif
        }
        CloseHandle(StopOverlapped.hEvent);
        //////////////////////////////////////////////////////////////////////////
        if (!dwRet)
        {
            dwRet = GetLastError();
        }
        else
        {
            dwRet = ERROR_SUCCESS;
        }

        CloseHandle(hDevice);
    }

    return(dwRet);
}

DWORD IsochAllocateBandwidth
(TCHAR* szDeviceName, PISOCH_ALLOCATE_BANDWIDTH isochAllocateBandwidth)
{
    HANDLE hDevice;
    DWORD  dwRet=0, dwBytesRet;

#ifdef _DEBUGISOCH
    _RPT0(_CRT_WARN, "Enter IsochAllocateBandwidth\r\n");
    _RPT1(_CRT_WARN, "nMaxBytesPerFrameRequested = 0x%x\r\n", isochAllocateBandwidth->nMaxBytesPerFrameRequested);
    _RPT1(_CRT_WARN, "fulSpeed = 0x%x\r\n", isochAllocateBandwidth->fulSpeed);
#endif
    hDevice = OpenDevice(szDeviceName, FALSE);

    if (hDevice != INVALID_HANDLE_VALUE)
    {

        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_ISOCH_ALLOCATE_BANDWIDTH,
                                 isochAllocateBandwidth,
                                 sizeof(ISOCH_ALLOCATE_BANDWIDTH),
                                 isochAllocateBandwidth,
                                 sizeof(ISOCH_ALLOCATE_BANDWIDTH),
                                 &dwBytesRet,
                                 NULL
                               );

        if (dwRet)
        {
            dwRet = ERROR_SUCCESS;

            _RPT1(_CRT_WARN, "hBandwidth = 0x%x\r\n", isochAllocateBandwidth->hBandwidth);
            _RPT1(_CRT_WARN, "BytesPerFrameAvailable = 0x%x\r\n", isochAllocateBandwidth->BytesPerFrameAvailable);
            _RPT1(_CRT_WARN, "Speed Selected = 0x%x\r\n", isochAllocateBandwidth->SpeedSelected);
        }
        else
        {
            dwRet = GetLastError();
            _RPT1(_CRT_WARN, "Error = 0x%x\r\n", dwRet);
        }

        // Free resources
        CloseHandle(hDevice);
    }

    return(dwRet);
}


DWORD IsochAllocateChannel
(TCHAR* szDeviceName, PISOCH_ALLOCATE_CHANNEL isochAllocateChannel)
{
    HANDLE hDevice;
    DWORD  dwRet=0, dwBytesRet;

#ifdef _DEBUGISOCH
    _RPT0(_CRT_WARN, "Enter IsochAllocateChannel\r\n");
    _RPT1(_CRT_WARN, "nRequestedChannel = 0x%x\r\n", isochAllocateChannel->nRequestedChannel);
#endif

    hDevice = OpenDevice(szDeviceName, FALSE);

    if (hDevice != INVALID_HANDLE_VALUE)
    {

        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_ISOCH_ALLOCATE_CHANNEL,
                                 isochAllocateChannel,
                                 sizeof(ISOCH_ALLOCATE_CHANNEL),
                                 isochAllocateChannel,
                                 sizeof(ISOCH_ALLOCATE_CHANNEL),
                                 &dwBytesRet,
                                 NULL
                               );

        if (dwRet)
        {
            dwRet = ERROR_SUCCESS;
#ifdef _DEBUGISOCH
            _RPT1(_CRT_WARN, "Channel = 0x%x\r\n", isochAllocateChannel->Channel);
            _RPT1(_CRT_WARN, "ChannelsAvailable.LowPart = 0x%x\r\n", isochAllocateChannel->ChannelsAvailable.LowPart);
            _RPT1(_CRT_WARN, "ChannelsAvailable.HighPart = 0x%x\r\n", isochAllocateChannel->ChannelsAvailable.HighPart);
#endif
        }
        else
        {
            dwRet = GetLastError();
#ifdef _DEBUGISOCH
            _RPT1(_CRT_WARN, "Error = 0x%x\r\n", dwRet);
#endif
        }

        // free up resources
        CloseHandle(hDevice);
    }

    return(dwRet);
} // IsochAllocateChannel

DWORD IsochAllocateResources
(TCHAR* szDeviceName, PISOCH_ALLOCATE_RESOURCES isochAllocateResources)
{
    HANDLE hDevice;
    DWORD  dwRet=0, dwBytesRet;

#ifdef _DEBUGISOCH
    _RPT1(_CRT_WARN, "[IsochAllocateResources] fulSpeed = 0x%x\r\n", isochAllocateResources->fulSpeed);
    _RPT1(_CRT_WARN, "fulFlags = 0x%x\r\n", isochAllocateResources->fulFlags);
    _RPT1(_CRT_WARN, "nChannel = 0x%x\r\n", isochAllocateResources->nChannel);
    _RPT1(_CRT_WARN, "nMaxBytesPerFrame = 0x%x\r\n", isochAllocateResources->nMaxBytesPerFrame);
    _RPT1(_CRT_WARN, "nNumberOfBuffers = 0x%x\r\n", isochAllocateResources->nNumberOfBuffers);
    _RPT1(_CRT_WARN, "nMaxBufferSize = 0x%x\r\n", isochAllocateResources->nMaxBufferSize);
    _RPT1(_CRT_WARN, "nQuadletsToStrip = 0x%x\r\n", isochAllocateResources->nQuadletsToStrip);
#endif

    hDevice = OpenDevice(szDeviceName, FALSE);

    if (hDevice != INVALID_HANDLE_VALUE)
    {
        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_ISOCH_ALLOCATE_RESOURCES,
                                 isochAllocateResources,
                                 sizeof(ISOCH_ALLOCATE_RESOURCES),
                                 isochAllocateResources,
                                 sizeof(ISOCH_ALLOCATE_RESOURCES),
                                 &dwBytesRet,
                                 NULL
                               );

        if (dwRet)
        {
            dwRet = ERROR_SUCCESS;
        }
        else
        {
            dwRet = GetLastError();
        }

        // free up resources
        CloseHandle(hDevice);
    }

    return dwRet;
}
