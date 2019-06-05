// sharelib_hvapi.cpp: implementation of the sharelib_hvapi class.
//
//////////////////////////////////////////////////////////////////////

#include "sharelib_hvapi.h"


HVSTATUS sharelib_hvapi::ReadQuadlet(DWORD dwOffset, DWORD *pValue)
{
    return (HVSTATUS)(m_pCamera->ReadQuadlet(dwOffset|0xF0000000,pValue));
}


HVSTATUS sharelib_hvapi::WriteQuadlet(DWORD dwOffset, DWORD dwValue)
{
    return (HVSTATUS)(m_pCamera->WriteQuadlet(dwOffset|0xF0000000,dwValue));
}


HVSTATUS sharelib_hvapi::ReadBlock(DWORD dwOffset, PBYTE pOutBuff, DWORD outBufSize)
{
    HVSTATUS status = STATUS_OK;
    DWORD *pData = (DWORD*)pOutBuff;
    for (DWORD i=0; i<outBufSize/sizeof(DWORD); i++)
    {
        status = ReadQuadlet(dwOffset,&pData[i]);
        if (!HV_SUCCESS(status))
        {
            break;
        }
    }
    return status;
}

// 块写
HVSTATUS sharelib_hvapi::WriteBlock(DWORD dwOffset, PBYTE pInBuff, DWORD inBufSize)
{
    HVSTATUS status = STATUS_OK;
    DWORD *pData = (DWORD*)pInBuff;
    for (DWORD i=0; i<inBufSize/sizeof(DWORD); i++)
    {
        status = WriteQuadlet(dwOffset,pData[i]);
        if (!HV_SUCCESS(status))
        {
            break;
        }
    }
    return status;
}

HVSTATUS sharelib_hvapi::Control(
    const DWORD code,
    PVOID pInBuff,  const DWORD dwInBuffSize,
    PVOID pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{

    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    switch (code)
    {
        //////////////////////////////////////////////////////////////////////////
        //HVInter.h 接口
    case ORD_READ_QUADLET:
        status = Fun_ORD_READ_QUADLET(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_WRITE_QUADLET:
        status = Fun_ORD_WRITE_QUADLET(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_READ_BLOCK:
        status = Fun_ORD_READ_BLOCK(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_WRITE_BLOCK:
        status = Fun_ORD_WRITE_BLOCK(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
        //HVDef.h 接口
        // ORD            // 注释ORD值，便于按照HV_CONTROL_CODE顺序排列
    case ORD_INIT_CAMERA: // = 0x00000000,
        status = Fun_ORD_INIT_CAMERA(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_STRING_PRESENCE://= 0x00000004,
        status = Fun_ORD_QUERY_STRING_PRESENCE(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_STRING_DESCRIPTOR://= 0x00000005,
        status = Fun_ORD_QUERY_STRING_DESCRIPTOR(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_STRING://= 0x00000006
        status = Fun_ORD_GET_STRING(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_RESET_CAMERA_PRESENCE://= 0x00000008
        status = Fun_ORD_QUERY_RESET_CAMERA_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_RESET_CAMERA://= 0x0000000B,
        status = Fun_ORD_RESET_CAMERA(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_LAST_STATUS_PRESENCE://0x0000000C,
        status = Fun_ORD_QUERY_LAST_STATUS_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_LAST_STATUS://= 0x0000000E,
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_CYCLE_PORT_PRESENCE://= 0x00000010,
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;
    case ORD_CYCLE_PORT://= 0x00000013,
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_VIDEO_FORMAT_PRESENCE://= 0x00001000,
        status = Fun_ORD_QUERY_VIDEO_FORMAT_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_VIDEO_MODE_PRESENCE://= 0x00001004,
        status = Fun_ORD_QUERY_VIDEO_MODE_PRESENCE(
                     pInBuff, dwInBuffSize,pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_FRAME_RATE_PRESENCE://= 0x00001008,
        status = Fun_ORD_QUERY_FRAME_RATE_PRESENCE(
                     pInBuff, dwInBuffSize,pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE://= 0x0000100A,
        status = Fun_ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE(
                     pInBuff, dwInBuffSize,pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE://= 0x0000100B,
        status = Fun_ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE(
                     pInBuff, dwInBuffSize,pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

    case ORD_QUERY_FMT7_MODE_DESCRIPTOR://= 0x0000100D,
        status = Fun_ORD_QUERY_FMT7_MODE_DESCRIPTOR(
                     pInBuff, dwInBuffSize,pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_FMT7_MODE://= 0x0000100E,
        status = Fun_ORD_GET_FMT7_MODE(
                     pInBuff, dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_FMT7_MODE://= 0x0000100F,
        status = Fun_ORD_SET_FMT7_MODE(
                     pInBuff, dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_FEATURE_PRESENCE://= 0x00001010,
        status = Fun_ORD_QUERY_FEATURE_PRESENCE(
                     pInBuff, dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_FEATURE_DESCRIPTOR://= 0x00001011,
        status = Fun_ORD_QUERY_FEATURE_DESCRIPTOR(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_FEATURE://= 0x00001012,
        status = Fun_ORD_GET_FEATURE(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_FEATURE://= 0x00001013,
        status = Fun_ORD_SET_FEATURE(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_BASIC_FUNC_PRESENCE://= 0x0000101C,
        status = Fun_ORD_QUERY_BASIC_FUNC_PRESENCE(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_BASIC_FUNC_DESCRIPTOR://= 0x0000101D,
        status = Fun_ORD_QUERY_BASIC_FUNC_DESCRIPTOR(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_BASIC_FUNC://= 0x0000101E,
        status = Fun_ORD_GET_BASIC_FUNC(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_BASIC_FUNC://= 0x0000101F,
        status = Fun_ORD_SET_BASIC_FUNC(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_OPT_FUNC_PRESENCE://= 0x00001020,
        status = Fun_ORD_QUERY_OPT_FUNC_PRESENCE(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
        // PIO
    case ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE://= 0x00001024,
        status = Fun_ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR://= 0x00001025,
        status = Fun_ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_PIO_OUTPUT_PORT://= 0x00001026,
        status = Fun_ORD_GET_PIO_OUTPUT_PORT(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_PIO_OUTPUT_PORT://= 0x00001027,
        status = Fun_ORD_SET_PIO_OUTPUT_PORT(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
        //SIO
    case ORD_QUERY_SIO_DESCRIPTOR://= 0x00001029,
        status = Fun_ORD_QUERY_SIO_DESCRIPTOR(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_SIO://= 0x0000102A,
        status = Fun_ORD_GET_SIO(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_SIO://= 0x0000102B,
        status = Fun_ORD_SET_SIO(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_STROBE_PRESENCE://= 0x0000102C,
        status = Fun_ORD_QUERY_STROBE_PRESENCE(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_STROBE_DESCRIPTOR://= 0x0000102D,
        status = Fun_ORD_QUERY_STROBE_DESCRIPTOR(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_STROBE://= 0x0000102E,
        status = Fun_ORD_GET_STROBE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_STROBE://= 0x0000102F,
        status = Fun_ORD_SET_STROBE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_OPEN_SNAP://= 0x00002000,
        status = Fun_ORD_OPEN_SNAP(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_CLOSE_SNAP://= 0x00002001,
        status = Fun_ORD_CLOSE_SNAP(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_START_SNAP:// = 0x00002002,
        status = Fun_ORD_START_SNAP(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_STOP_SNAP://= 0x00002003,
        status = Fun_ORD_STOP_SNAP(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SNAP_SHOT://= 0x00002004,
        status = Fun_ORD_SNAP_SHOT(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_TRIGGER_SHOT://= 0x00002005,
        status = Fun_ORD_TRIGGER_SHOT(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

    case ORD_SNAP_SHOT_EX:// = 0x00002006,
        status = Fun_ORD_SNAP_SHOT_EX(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_REGISTER_CALLBACK://= 0x00002007,
        status = Fun_ORD_REGISTER_CALLBACK(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_DEREGISTER_CALLBACK:// = 0x00002008,
        status = Fun_ORD_DEREGISTER_CALLBACK(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_CMD_RESERVED0://= 0x00002100,
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;
    case ORD_CMD_RESERVED1://= 0x00002101,
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;
    case ORD_CMD_RESERVED2://= 0x00002102,
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_ADV_FEATURE_PRESENCE://= 0x00003000,
        status = Fun_ORD_QUERY_ADV_FEATURE_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_ADV_FEATURE_DESCRIPTOR://= 0x00003001,
        status = Fun_ORD_QUERY_ADV_FEATURE_DESCRIPTOR(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_ADV_FEATURE://= 0x00003002,
        status = Fun_ORD_GET_ADV_FEATURE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_ADV_FEATURE://= 0x00003003,
        status = Fun_ORD_SET_ADV_FEATURE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_ADV_BASIC_FUNC_PRESENCE://= 0x00003004,
        status = Fun_ORD_QUERY_ADV_BASIC_FUNC_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_ADV_BASIC_FUNC://= 0x00003006,
        status = Fun_ORD_GET_ADV_BASIC_FUNC(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_ADV_BASIC_FUNC:// = 0x00003007,
        status = Fun_ORD_SET_ADV_BASIC_FUNC(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_ADV_OPT_FUNC_PRESENCE://= 0x00003008,
        status = Fun_ORD_QUERY_ADV_OPT_FUNC_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_ADV_LUT_DESCRIPTOR://= 0x0000300D,
        status = Fun_ORD_QUERY_ADV_LUT_DESCRIPTOR(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_ADV_LUT://= 0x0000300E,
        status = Fun_ORD_GET_ADV_LUT(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_ADV_LUT://= 0x0000300F,
        status = Fun_ORD_SET_ADV_LUT(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR://= 0x00003011,
        //status = Fun_ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR(
        //     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;
    case ORD_GET_ADV_USER_CUSTOM://= 0x00003012,
        //status = Fun_ORD_GET_ADV_USER_CUSTOM(
        //     pInBuff, dwInBuffSize,pOutBuff, dwOutBuffSize,pBytesReturned);
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;
    case ORD_SET_ADV_USER_CUSTOM://= 0x00003013,
        //status = Fun_ORD_SET_ADV_USER_CUSTOM(pInBuff, dwInBuffSize);
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE://= 0x00003014,
        status = Fun_ORD_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE(
                     pInBuff,dwInBuffSize,pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR://= 0x00003015,
        status = Fun_ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR(
                     pInBuff,dwInBuffSize,pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_ADV_USER_ENCRYPT_ZONE://= 0x00003016,
        status = Fun_ORD_GET_ADV_USER_ENCRYPT_ZONE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_ADV_USER_ENCRYPT_ZONE://= 0x00003017,
        status = Fun_ORD_SET_ADV_USER_ENCRYPT_ZONE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_NET_FUNC_PRESENCE://= 0x00004000,
        status = Fun_ORD_QUERY_NET_FUNC_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_NET_FUNC_DESCRIPTOR://= 0x00004001,
        status = Fun_ORD_QUERY_NET_FUNC_DESCRIPTOR(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_NET_FUNC://= 0x00004002,
        status = Fun_ORD_GET_NET_FUNC(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_NET_FUNC://= 0x00004003,
        status = Fun_ORD_SET_NET_FUNC(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_IMAGE_INFO_PRESENCE://= 0x00005000,
        status = Fun_ORD_QUERY_IMAGE_INFO_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_IMAGE_INFO_DESCRIPTOR://= 0x00005001,
        status = Fun_ORD_QUERY_IMAGE_INFO_DESCRIPTOR(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_IMAGE_INFO://= 0x00005002,
        status = Fun_ORD_GET_IMAGE_INFO(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;

        //////////////////////////////////////////////////////////////////////////
    case ORD_QUERY_AUTO_FUNC_PRESENCE://= 0x00006000,
        status = Fun_ORD_QUERY_AUTO_FUNC_PRESENCE(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_AUTO_FUNC_DESCRIPTOR://= 0x00006001,
        status = Fun_ORD_QUERY_AUTO_FUNC_DESCRIPTOR(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_AUTO_FUNC://= 0x00006002,
        status = Fun_ORD_GET_AUTO_FUNC(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_AUTO_FUNC:// = 0x00006003,
        status = Fun_ORD_SET_AUTO_FUNC(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
        ///////////////////// by xupx 2009.05.27 //////////////////////////////////////
    case ORD_QUERY_OSD_FUNC_PRESENCE://= 0x00007000,
        status = Fun_ORD_QUERY_OSD_FUNC_PRESENCE(
                     pInBuff,dwInBuffSize,pOutBuff,dwOutBuffSize,pBytesReturned);
        break;
    case ORD_QUERY_OSD_FUNC_DESCRIPTOR://= 0x00007000,
        status = Fun_ORD_QUERY_OSD_FUNC_DESCRIPTOR(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_GET_OSD_FUNC://= 0x00007000,
        status = Fun_ORD_GET_OSD_FUNC(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
    case ORD_SET_OSD_FUNC://= 0x00007000,
        status = Fun_ORD_SET_OSD_FUNC(
                     pInBuff, dwInBuffSize, pOutBuff, dwOutBuffSize,pBytesReturned);
        break;
        /////////////////////////// end by xupx ////////////////////////////////
    default:
        status = STATUS_NOT_SUPPORT_INTERFACE;
        break;
    }
    return status;
}

//////////////////////////////////////////////////////////////////////////
//HVInter.h 接口, ORD_RawIO.cpp
HVSTATUS sharelib_hvapi::Fun_ORD_READ_QUADLET(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    if ( pInBuff==NULL||pOutBuff==NULL||
            dwInBuffSize<sizeof(HV_ARG_READ_QUADLET)||
            dwOutBuffSize<sizeof(HV_RES_READ_QUADLET))
    {
        return STATUS_PARAMETER_INVALID;
    }

    HV_ARG_READ_QUADLET *pArg = (HV_ARG_READ_QUADLET*)pInBuff;
    HV_RES_READ_QUADLET *pRes = (HV_RES_READ_QUADLET*)pOutBuff;

    HVSTATUS status = ReadQuadlet(pArg->dwAddress,&pRes->dwData);
    if (HV_SUCCESS(status))
    {
        MAC_SET_RETURN_BYTES(pBytesReturned,sizeof(DWORD));
    }
    return status;
}

HVSTATUS sharelib_hvapi::Fun_ORD_WRITE_QUADLET(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    if (pInBuff==NULL||dwInBuffSize<sizeof(HV_ARG_WRITE_QUADLET))
    {
        return STATUS_PARAMETER_INVALID;
    }
    HV_ARG_WRITE_QUADLET *pArg = (HV_ARG_WRITE_QUADLET*)pInBuff;
    return WriteQuadlet(pArg->dwAddress,pArg->dwData);
}

HVSTATUS sharelib_hvapi::Fun_ORD_READ_BLOCK(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    if ( pInBuff==NULL||pOutBuff==NULL||
            dwInBuffSize<sizeof(HV_ARG_READ_BLOCK)||
            dwOutBuffSize<sizeof(HV_RES_READ_BLOCK) )
    {
        return STATUS_PARAMETER_INVALID;
    }

    HV_ARG_READ_BLOCK *pArg = (HV_ARG_READ_BLOCK*)pInBuff;
    HV_RES_READ_BLOCK *pRes = (HV_RES_READ_BLOCK*)pOutBuff;

    HVSTATUS status = ReadBlock(pArg->dwAddress,pRes->pData,pArg->nLength);
    if (HV_SUCCESS(status))
    {
        MAC_SET_RETURN_BYTES(pBytesReturned,dwOutBuffSize);
    }
    return status;
}


HVSTATUS sharelib_hvapi::Fun_ORD_WRITE_BLOCK(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    if ( pInBuff==NULL||dwInBuffSize<sizeof(HV_ARG_WRITE_BLOCK) )
    {
        return STATUS_PARAMETER_INVALID;
    }

    HV_ARG_WRITE_BLOCK *pArg = (HV_ARG_WRITE_BLOCK*)pInBuff;
    return WriteBlock(pArg->dwAddress,pArg->pData,pArg->nLength);
}


//HVDef.h 接口
HVSTATUS sharelib_hvapi::Fun_ORD_INIT_CAMERA(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_STRING_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_STRING_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_STRING(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_RESET_CAMERA_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_RESET_CAMERA(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_LAST_STATUS_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_VIDEO_FORMAT_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_VIDEO_MODE_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_FRAME_RATE_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_FMT7_MODE_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_FMT7_MODE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_FMT7_MODE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_FEATURE_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_FEATURE_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_FEATURE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_FEATURE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_BASIC_FUNC_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_BASIC_FUNC_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_BASIC_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_BASIC_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_OPT_FUNC_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_PIO_OUTPUT_PORT(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_PIO_OUTPUT_PORT(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_SIO_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_SIO(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_SIO(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_STROBE_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_STROBE_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_STROBE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_STROBE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


// ORD_SNAP.cpp
HVSTATUS sharelib_hvapi::Fun_ORD_OPEN_SNAP(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_START_SNAP(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_STOP_SNAP(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_CLOSE_SNAP(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SNAP_SHOT(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_TRIGGER_SHOT(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_SNAP_SHOT_EX(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_REGISTER_CALLBACK(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_DEREGISTER_CALLBACK(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_ADV_FEATURE_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    if ( pOutBuff==NULL||dwOutBuffSize<sizeof(HV_RES_QUERY_ADV_FEATURE_PRESENCE))
    {
        return STATUS_PARAMETER_INVALID;
    }

    HV_RES_QUERY_ADV_FEATURE_PRESENCE *pRes =
        (HV_RES_QUERY_ADV_FEATURE_PRESENCE*)pOutBuff;

    DWORD AdvFeatureHiInq = 0;
    DWORD AdvFeatureLoInq = 0;

    HVSTATUS status = ReadQuadlet(0x2F00404,&AdvFeatureHiInq);
    if (HV_SUCCESS(status))
    {
        status = ReadQuadlet(0x2F00408,&AdvFeatureLoInq);
        if (HV_SUCCESS(status))
        {
            DWORD *pData = (DWORD*)pOutBuff;
            pData[0] = AdvFeatureHiInq;
            pData[1] = AdvFeatureLoInq;
            MAC_SET_RETURN_BYTES(pBytesReturned,sizeof(DWORD)*2);
        }
    }
    return status;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_ADV_FEATURE_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    if ( pInBuff==NULL||pOutBuff==NULL||
            dwInBuffSize<sizeof(HV_ARG_QUERY_ADV_FEATURE_DESCRIPTOR)||
            dwOutBuffSize<sizeof(HV_RES_QUERY_ADV_FEATURE_DESCRIPTOR))
    {
        return STATUS_PARAMETER_INVALID;
    }

    HV_ARG_QUERY_ADV_FEATURE_DESCRIPTOR *pArg =
        (HV_ARG_QUERY_ADV_FEATURE_DESCRIPTOR*)pInBuff;

    HVSTATUS status = STATUS_OK;
    DWORD dwOffset  = 0;
    status = DHCAM_ADV_FEATURE_INQ_OFFSET(pArg->AdvFeatureId,dwOffset);
    if (HV_SUCCESS(status))
    {
        status = ReadQuadlet(dwOffset,(DWORD*)pOutBuff);
        if (HV_SUCCESS(status))
        {
            MAC_SET_RETURN_BYTES(pBytesReturned,sizeof(DWORD));
        }
    }
    return status;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_ADV_FEATURE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    if ( pInBuff==NULL||pOutBuff==NULL||
            dwInBuffSize<sizeof(HV_ARG_GET_ADV_FEATURE)||
            dwOutBuffSize<sizeof(HV_RES_GET_ADV_FEATURE))
    {
        return STATUS_PARAMETER_INVALID;
    }

    HV_ARG_GET_ADV_FEATURE *pArg = (HV_ARG_GET_ADV_FEATURE*)pInBuff;

    DWORD dwOffset = 0;
    HVSTATUS status = DHCAM_ADV_FEATURE_OFFSET(pArg->AdvFeatureId,dwOffset);
    if (HV_SUCCESS(status))
    {
        status = ReadQuadlet(dwOffset,(DWORD*)pOutBuff);
        if (HV_SUCCESS(status))
        {
            MAC_SET_RETURN_BYTES(pBytesReturned,sizeof(DWORD));
        }
    }
    return status;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_ADV_FEATURE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    if ( pInBuff==NULL||dwInBuffSize<sizeof(HV_ARG_SET_ADV_FEATURE))
    {
        return STATUS_PARAMETER_INVALID;
    }

    HV_ARG_SET_ADV_FEATURE *pArg = (HV_ARG_SET_ADV_FEATURE*)pInBuff;

    // 获取寄存器地址
    DWORD dwOffset  = 0;
    HVSTATUS status = DHCAM_ADV_FEATURE_OFFSET(pArg->AdvFeatureId,dwOffset);
    if (!HV_SUCCESS(status))
    {
        return status;
    }

    // 获取寄存器当前值
    HV_ARG_GET_ADV_FEATURE getArg;
    getArg.AdvFeatureId = pArg->AdvFeatureId;
    HV_RES_GET_ADV_FEATURE advFeature;
    status = Fun_ORD_GET_ADV_FEATURE(&getArg,sizeof(getArg),&advFeature,sizeof(advFeature));
    if (!HV_SUCCESS(status))
    {
        return status;
    }

    switch (pArg->AdvFeatureItemId)
    {
        // Selector for shutter unit, test image, multi_trigger
    case ADV_FEATURE_ITEM_ID_SELECTOR_PRESENCE_INQ:
        advFeature.Selector.PresenceInq = pArg->AdvFeature.Selector.PresenceInq;
        break;
    case ADV_FEATURE_ITEM_ID_SELECTOR_ABS_CONTROL:
        advFeature.Selector.AbsControl = pArg->AdvFeature.Selector.AbsControl;
        break;
    case ADV_FEATURE_ITEM_ID_SELECTOR_ONE_PUSH:
        advFeature.Selector.OnePush = pArg->AdvFeature.Selector.OnePush;
        break;
    case ADV_FEATURE_ITEM_ID_SELECTOR_ON_OFF:
        advFeature.Selector.OnOff = pArg->AdvFeature.Selector.OnOff;
        break;
    case ADV_FEATURE_ITEM_ID_SELECTOR_MODE:
        advFeature.Selector.Mode = pArg->AdvFeature.Selector.Mode;
        break;
    case ADV_FEATURE_ITEM_ID_SELECTOR_PARAMETER:
        advFeature.Selector.Parameter = pArg->AdvFeature.Selector.Parameter;
        break;
        // scalar for MULTI_TRIGGER_NUM, MULTI_TRIGGER_INTERVAL_TIME
    case ADV_FEATURE_ITEM_ID_SCALAR_PRESENCE_INQ:
        advFeature.Scalar.PresenceInq = pArg->AdvFeature.Scalar.PresenceInq;
        break;
    case ADV_FEATURE_ITEM_ID_SCALAR_ABS_CONTROL:
        advFeature.Scalar.PresenceInq = pArg->AdvFeature.Scalar.PresenceInq;
        break;
    case ADV_FEATURE_ITEM_ID_SCALAR_ONE_PUSH:
        advFeature.Scalar.OnePush = pArg->AdvFeature.Scalar.OnePush;
        break;
    case ADV_FEATURE_ITEM_ID_SCALAR_ON_OFF:
        advFeature.Scalar.OnOff = pArg->AdvFeature.Scalar.OnOff;
        break;
    case ADV_FEATURE_ITEM_ID_SCALAR_AUTO_MANUAL_MODE_VALUE:
        advFeature.Scalar.AutoManualMode = pArg->AdvFeature.Scalar.AutoManualMode;
        break;
    case ADV_FEATURE_ITEM_ID_SCALAR_VALUE:
        advFeature.Scalar.Value = pArg->AdvFeature.Scalar.Value;
        break;

        // startup memory channel
    case ADV_FEATURE_ITEM_ID_STARTUP_MEM_CH_PRESENCE_INQ:
        advFeature.StartupMemCh.PresenceInq =
            pArg->AdvFeature.StartupMemCh.PresenceInq;
        break;
    case ADV_FEATURE_ITEM_ID_STARTUP_MEM_CH_NUMBER:
        advFeature.StartupMemCh.PresenceInq = 1;
        advFeature.StartupMemCh.Number = pArg->AdvFeature.StartupMemCh.Number;
        break;
    case ADV_FEATURE_ITEM_ID_MEASURESPEED_TRIGGERMODE_ONOFF:
        advFeature.MeasureSpeedTriggerMode.OnOff =
            pArg->AdvFeature.MeasureSpeedTriggerMode.OnOff;
        break;
    case ADV_FEATURE_ITEM_ID_MEASURESPEED_TRIGGERPOLARITY:
        advFeature.MeasureSpeedTriggerMode.TriggerPolarity =
            pArg->AdvFeature.MeasureSpeedTriggerMode.TriggerPolarity;
        break;
    case ADV_FEATURE_ITEM_ID_MEASURESPEED_TRIGGERSOURCE:
        advFeature.MeasureSpeedTriggerMode.TriggerSource =
            pArg->AdvFeature.MeasureSpeedTriggerMode.TriggerSource;
        break;
    case ADV_FEATURE_ITEM_ID_MEASURESPEED_TIMEOUT_VALUE:
        advFeature.MeasureSpeedTimeout.TimeoutValue =
            pArg->AdvFeature.MeasureSpeedTimeout.TimeoutValue;
        break;
        
    case ADV_FEATURE_ITEM_ID_SCALAREX_VALUE:
        advFeature.ScalarEx.Value = pArg->AdvFeature.ScalarEx.Value;
        break;
    case ADV_FEATURE_ITEM_ID_ANTI_FLICKER_ONOFF:
        advFeature.AntiFlicker.OnOff = pArg->AdvFeature.AntiFlicker.OnOff;
        break;
    case ADV_FEATURE_ITEM_ID_ANTI_FLICKER_TRIGGERSOURCE:
        advFeature.AntiFlicker.TriggerSource = pArg->AdvFeature.AntiFlicker.TriggerSource;
        break;
    case ADV_FEATURE_ITEM_ID_MCU_DATE:
        memcpy(&advFeature.Date, &pArg->AdvFeature.Date, sizeof(DWORD));
        break;
    case ADV_FEATURE_ITEM_ID_MCU_TIME:
        advFeature.Time.Second = pArg->AdvFeature.Time.Second;
        advFeature.Time.Minute = pArg->AdvFeature.Time.Minute;
        advFeature.Time.Hour = pArg->AdvFeature.Time.Hour;
        break;

        // 分量增益控制
    case ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_R_VALUE:
        advFeature.PixelSeparate.RValue = pArg->AdvFeature.PixelSeparate.RValue;
        break;
    case ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GR_VALUE:
        advFeature.PixelSeparate.GrValue = pArg->AdvFeature.PixelSeparate.GrValue;
        break;
    case ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GB_VALUE:
        advFeature.PixelSeparate.GbValue = pArg->AdvFeature.PixelSeparate.GbValue;
        break;
    case ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_B_VALUE:
        advFeature.PixelSeparate.BValue = pArg->AdvFeature.PixelSeparate.BValue;
        break;
    default:
        status =  STATUS_NOT_SUPPORT_INTERFACE;
        break;
    }

    if (HV_SUCCESS(status))
    {
        status = WriteQuadlet(dwOffset,*(DWORD*)&advFeature);
    }
    return status;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_ADV_BASIC_FUNC_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_ADV_BASIC_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_ADV_BASIC_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_ADV_OPT_FUNC_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_ADV_LUT_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_ADV_LUT(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_ADV_LUT(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_ADV_USER_CUSTOM(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_ADV_USER_CUSTOM(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}



HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_ADV_USER_ENCRYPT_ZONE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_ADV_USER_ENCRYPT_ZONE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}



HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_NET_FUNC_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_NET_FUNC_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_NET_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_NET_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_IMAGE_INFO_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_IMAGE_INFO_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_IMAGE_INFO(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}



HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_AUTO_FUNC_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_AUTO_FUNC_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_AUTO_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_AUTO_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}


HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_OSD_FUNC_PRESENCE(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_QUERY_OSD_FUNC_DESCRIPTOR(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_GET_OSD_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS sharelib_hvapi::Fun_ORD_SET_OSD_FUNC(
    void* pInBuff,  const DWORD dwInBuffSize,
    void* pOutBuff, const DWORD dwOutBuffSize,
    DWORD *pBytesReturned
)
{
    return STATUS_NOT_SUPPORT_INTERFACE;
}

HVSTATUS DHCAM_ADV_FEATURE_INQ_OFFSET(
    HV_ADV_FEATURE_ID ID,
    DWORD &dwOffset
)
{
    switch (ID)
    {
    case ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN: //=0
        dwOffset = 0x2F00500;
        break;
    case ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL://=1
        dwOffset = 0x2F00504;
        break;
    case ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL://=2
        dwOffset = 0x2F00508;
        break;
    case ADV_FEATURE_ID_SKIP_MODE://=3
        dwOffset = 0x2F0050C;
        break;
    case ADV_FEATURE_ID_BIN_MODE://=4
        dwOffset = 0x2F00510;
        break;
    case ADV_FEATURE_ID_MIRROR_MODE://= 5
        dwOffset = 0x2F00514;
        break;
    case ADV_FEATURE_ID_SHUTTER_UNIT://= 6
        dwOffset = 0x2F00518;
        break;
    case ADV_FEATURE_ID_TEST_IMAGE://= 7
        dwOffset = 0x2F0051C;
        break;
    case ADV_FEATURE_ID_AD_CONTROL://= 8
        dwOffset = 0x2F00520;
        break;
    case ADV_FEATURE_ID_MULTI_TRIGGER_NUM://= 9,
        dwOffset = 0x2F00540;
        break;
    case ADV_FEATURE_ID_MULTI_TRIGGER_TIME://= 10,
        dwOffset = 0x2F00544;
        break;
    case ADV_FEATURE_ID_DIGITAL_GAIN://= 11,
        dwOffset = 0x2F0054C;
        break;
    case ADV_FEATURE_ID_STARTUP_MEM_CH://= 12,
        dwOffset = 0x2F00528;
        break;
    case ADV_FEATURE_ID_DELAY_TRANSMIT://= 13,
        dwOffset = 0x2F00524;
        break;
    case ADV_FEATURE_ID_FILTER_TIME://= 14,
        dwOffset = 0x2F00550;
        break;
        //case ADV_FEATURE_ID_FILTER_TIME_UNIT://= 15,
        //dwOffset = 0x2F00500;
        //	break;
    case ADV_FEATURE_ID_PIXEL_ENDIANESS://= 16,
        dwOffset = 0x2F00558;
        break;
        // 在当前协议中，找不到??
        //case ADV_FEATURE_ID_TRIGGER_SOURCE_STATUS://= 17,
        //	dwOffset;
        //	break;
        //case ADV_FEATURE_ID_INTER_PACKET_DELAY://= 18,
        //	dwOffset;
        //	break;
        //case ADV_FEATURE_ID_WATCHDOG_RESET_MARK://= 19,
        //	dwOffset;
        //	break;
    case ADV_FEATURE_ID_MEASURESPEED_TRIGGERMODE://= 20,
        dwOffset = 0x2F00564;
        break;
    case ADV_FEATURE_ID_MEASURESPEED_TIMEOUT://= 21,
        dwOffset = 0x2F00568;
        break;
    case ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT://= 22,
        dwOffset = 0x2F0056C;
        break;
    case ADV_FEATURE_ID_JPEG_COMPRESS_QUALITY://= 23,
        dwOffset = 0x2F00570;
        break;
    case ADV_FEATURE_ID_MCU_DATE://= 24,
        dwOffset = 0x2F00574;
        break;
    case ADV_FEATURE_ID_MCU_TIME://= 25,
        dwOffset = 0x2F00578;
        break;
    case ADV_FEATURE_ID_ANTI_FLICKER://= 26,
        dwOffset = 0x2F0057C;
        break;
    case ADV_FEATURE_ID_AC_PHASE_ADJUST://= 27,
        dwOffset = 0x2F00580;
        break;
    case ADV_FEATURE_ID_TRIGGER_DELAY_UNIT://= 28
        dwOffset = 0x2F00584;
        break;
    default:
        return STATUS_NOT_SUPPORT_INTERFACE;
        break;
    }
    return STATUS_OK;
}

HVSTATUS DHCAM_ADV_FEATURE_OFFSET(
    HV_ADV_FEATURE_ID ID,
    DWORD &dwOffset
)
{
    switch (ID)
    {
    case ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN: //=0
        dwOffset = 0x2F00800;
        break;
    case ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL://=1
        dwOffset = 0x2F00804;
        break;
    case ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL://=2
        dwOffset = 0x2F00808;
        break;
    case ADV_FEATURE_ID_SKIP_MODE://=3
        dwOffset = 0x2F0080C;
        break;
    case ADV_FEATURE_ID_BIN_MODE://=4
        dwOffset = 0x2F00810;
        break;
    case ADV_FEATURE_ID_MIRROR_MODE://= 5
        dwOffset = 0x2F00814;
        break;
    case ADV_FEATURE_ID_SHUTTER_UNIT://= 6
        dwOffset = 0x2F00818;
        break;
    case ADV_FEATURE_ID_TEST_IMAGE://= 7
        dwOffset = 0x2F0081C;
        break;
    case ADV_FEATURE_ID_AD_CONTROL://= 8
        dwOffset = 0x2F00820;
        break;
    case ADV_FEATURE_ID_MULTI_TRIGGER_NUM://= 9,
        dwOffset = 0x2F00840;
        break;
    case ADV_FEATURE_ID_MULTI_TRIGGER_TIME://= 10,
        dwOffset = 0x2F00844;
        break;
    case ADV_FEATURE_ID_DIGITAL_GAIN://= 11,
        dwOffset = 0x2F0084C;
        break;
    case ADV_FEATURE_ID_STARTUP_MEM_CH://= 12,
        dwOffset = 0x2F00828;
        break;
    case ADV_FEATURE_ID_DELAY_TRANSMIT://= 13,
        dwOffset = 0x2F00824;
        break;
    case ADV_FEATURE_ID_FILTER_TIME://= 14,
        dwOffset = 0x2F00850;
        break;
        //case ADV_FEATURE_ID_FILTER_TIME_UNIT://= 15,
        //dwOffset = ;
        //	break;
    case ADV_FEATURE_ID_PIXEL_ENDIANESS://= 16,
        dwOffset = 0x2F00858;
        break;
        // 在当前协议中，找不到??
        //case ADV_FEATURE_ID_TRIGGER_SOURCE_STATUS://= 17,
        //	dwOffset;
        //	break;
        //case ADV_FEATURE_ID_INTER_PACKET_DELAY://= 18,
        //	dwOffset;
        //	break;
    case ADV_FEATURE_ID_WATCHDOG_RESET_MARK://= 19,
        dwOffset = 0x2F00860;
        break;
    case ADV_FEATURE_ID_MEASURESPEED_TRIGGERMODE://= 20,
        dwOffset = 0x2F00864;
        break;
    case ADV_FEATURE_ID_MEASURESPEED_TIMEOUT://= 21,
        dwOffset = 0x2F00868;
        break;
    case ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT://= 22,
        dwOffset = 0x2F0086C;
        break;
    case ADV_FEATURE_ID_JPEG_COMPRESS_QUALITY://= 23,
        dwOffset = 0x2F08570;
        break;
    case ADV_FEATURE_ID_MCU_DATE://= 24,
        dwOffset = 0x2F00874;
        break;
    case ADV_FEATURE_ID_MCU_TIME://= 25,
        dwOffset = 0x2F00878;
        break;
    case ADV_FEATURE_ID_ANTI_FLICKER://= 26,
        dwOffset = 0x2F0087C;
        break;
    case ADV_FEATURE_ID_AC_PHASE_ADJUST://= 27,
        dwOffset = 0x2F00880;
        break;
    case ADV_FEATURE_ID_TRIGGER_DELAY_UNIT://= 28
        dwOffset = 0x2F00884;
        break;
    default:
        return STATUS_NOT_SUPPORT_INTERFACE;
        break;
    }
    return STATUS_OK;
}
