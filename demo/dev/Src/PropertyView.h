//------------------------------------------------------------------------------
// class CPropertyView
// Author: ilovedrv
// Date: 2004.03.12
//------------------------------------------------------------------------------
/**
* \brief   Tree view which shows the devcies found on the usb bus
*
* This class implements the bus viewer window, 
* which allows the user to navigate the Bcam devices found on the usb bus.
* The bus viewer window is registered for Bcam related device notifications 
* sent by the plug and play manager. New devices are added to the tree, unplugged
* devices are removed.
* 
*/

#if !defined(AFX_PropertyView_H__F4CED1A3_9CED_4DF5_9C91_692B10F7E930__INCLUDED_)
#define AFX_PropertyView_H__F4CED1A3_9CED_4DF5_9C91_692B10F7E930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "dialogview.h"
#include "controls\PropertyList.h"

class CMainFrame;
class CDeviceManager;


#define ITEM_VIDEOMODE				0	 
#define ITEM_SHUTTERUNIT			2
#define SCALAR_SHUTTERSPEED			3
#define ITEM_SNAPMODE				4
#define ITEM_TRIGGERPOLARITY		5
#define ITEM_STROBEPOLARITY			6
#define SCALAR_GAINLEVEL			7
#define ITEM_ADCLEVEL				8
#define ITEM_BLACKLEVELENABLE		9
#define SCALAR_BLACKLEVEL			10
#define SCALAR_BLANKHOR				11
#define SCALAR_BLANKVERT			12
#define SCALAR_PACKET				13
#define SCALAR_BRIGHTNESS			14
#define SCALAR_BLUE_GAIN			15
#define SCALAR_RED_GAIN				16

#define ITEM_LUT					17
#define ITEM_FRAMEFROZEN			18
#define ITEM_SOFTTRIGGER			19
#define ITEM_TEST_IMAGE				20  
#define ITEM_STROBE_ON_OFF			21
#define ITEM_TRIGGERDELAY_STATE		22   
#define SCALAR_TRIGGERDELAY_VALUE	23
#define ITEM_8or12BitMode			24  
#define ITEM_OUTPUTIO_0				25  
#define ITEM_OUTPUTIO_1				26
#define ITEM_OUTPUTIO_2				27
#define ITEM_OUTPUTIO_0_CONTROL		28   
#define ITEM_OUTPUTIO_1_CONTROL		29  
#define ITEM_OUTPUTIO_2_CONTROL		30  
#define ITEM_INPUTIOSET				31   
#define SCALAR_TRANSDELAY_VALUE		32 
#define ITEM_TRIGGERDELAY_UNIT		33 

#define SCALAR_FILTER_TIME			35
#define ITEM_LE_BE_MODE				36
#define ITEM_FILTER_TIME_UNIT		37

#define ITEM_BEGIN_SAVE				39		//xupxSaveParam 0519
#define ITEM_SAVE_PARAM				40		//xupxSaveParam 0520
#define ITEM_LOAD_PARAM				41		//xupxSaveParam 0520
#define SCALAR_NETWORK_PACKET_SIZE	42		//XUPXNetwork 1410GC 0520
#define SCALAR_REQUEST_OVETTIME		45		//xupx1410GC
#define ITEM_ANTI_FLICKER			46
#define ITEM_ANTI_FLICKER_SOURCE	47
#define SCALAR_AC_PHASE_ADJUST		48

#define ITEM_SHUTTER_AUTO			52
#define ITEM_GAIN_AUTO				53
#define ITEM_STROBE_AUTO			54

#define SCALAR_RB_GAIN_EX			58
#define SCALAR_GREEN_GAIN_EX		59
#define SCALAR_GREEN_GAIN			60
#define SCALAR_TRIGGER_NUM			61	//一次触发采集帧数
#define SCALAR_TRIGGER_TIME			62	//一次触发采集帧间隔
#define ITEM_MULTI_TRIGGER_ONOFF	63

#define SCALAR_PACKET_DELAY			65
#define SCALAR_EXPECT_DATA_RATE		66
#define ITEM_CONNECT_SPEED			67

#define ITEM_PASSIVE_TRANSMIT		70
#define ITEM_DIGITAL_GAIN			71

#define ITEM_WB_ON_OFF				75
#define ITEM_WB_AUTO_MANU			76
#define	ITEM_WB_ONE_PUSH			77
#define SCALAR_R_VALUE				78
#define SCALAR_B_VALUE				79
#define SCALAR_G_VALUE				80

const int Item2DigitalGain[] = { 1, 2, 4, 8, 16};
const int DigitalGain2Item[] = {0, 0, 1, 0, 2, 0, 0, 0,	
3, 0, 0, 0, 0, 0, 0, 0,
4};


class CPropertyView : public CDialogView<CPropertyView>  
{
public:
	enum {IDD = IDD_PROPERTYVIEW};
	
	BEGIN_MSG_MAP(CPropertyView)
		CHAIN_MSG_MAP(CDialogView<CPropertyView>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		NOTIFY_CODE_HANDLER(PIN_ITEMCHANGED, OnItemChanged);
		NOTIFY_CODE_HANDLER(PIN_SCALARCHANGED, OnScalarChanged);
		REFLECT_NOTIFICATIONS()
		END_MSG_MAP()
		
		// Handler prototypes (uncomment arguments if needed):
		//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		LRESULT OnScalarChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
		LRESULT OnItemChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

		CPropertyView(CDeviceManager& DeviceManager, CMainFrame& MainFrame);
	
public:
		/// A client informs us that the current device has changed ( e.g. the user activated an other 
		/// image view associated with an other device)
		void Update_PRP(CDevice* pdevice);
		void SendPassiveTrans(CDevice *pDevice , int commandID);
		/// set auto、TestImage、triggerDelay onoff&value
		void Update_DefaultValue(CDevice* pDevice, BOOL b);
		void Set_Param(CDevice * pDevice);
		void Spec_Fun_SaveParam(CDevice *pDevice);
		void Spec_Fun_LoadParam(CDevice *pDevice);
		void Spec_Fun_StartParam(CDevice *pDevice);
		int  Spec_Fun_Get_Basic(CDevice *pDevice, int Id);
		void Spec_Fun_Set_AdvTrigger(CDevice *pDevice, ReadFunID Id, int value);
		void Spec_Fun_SetColorCode(CDevice* pDevice, HV_COLOR_CODE value, HV_VIDEO_MODE mode);
		void Set_AutoFunc(CDevice *pDevice, int FunID, int nValue);

		void UpdateMultiTriggerTime(CDevice* pDevice);
		void UpdateShutter(CDevice* pDevice);
		void UpdateShutterHV(CDevice* pDevice);
		void UpdateGain(CDevice * pDevice);
		void UpdateTriggerDelay(CDevice *pDevice);
		void UpdateWB(CDevice* pDevice);
		void UpdateRGBChannel(CDevice* pDevice);
		void UpdateOutputIoControl(CDevice* pDevice, int nID, int nIDS);
		void UpdateVideoMode(CDevice* pDevice);
		void UpdateDigitalGain(CDevice *pDevice);
		void UpdateColorMode(CDevice *pDevice);
		void UpdateGainLevel(CDevice *pDevice,int flag);
		void UpdatePacketSize(CDevice *pDevice);
		
		int		Spec_Fun_Get_AdvDescript(CDevice *pDevice, int readFunID, int n);
		int		Spec_Fun_ADV_Get(CDevice *pDevice, int readID);
		
		CPropertyListCtrl		m_ListView;	

private:
		int		Spec_Fun_Get(CDevice *pDevice, int Id);
		int		Spec_Fun_Get_Descript(CDevice *pDevice, int readFunID, int n);
		int		Spec_Fun_AutoDescript(CDevice *pDevice, int readFunID, int n);
 		int		Spec_Fun_Get_AutoFunc(CDevice *pDevice, int Id);
		int		Spec_Fun_Get_FMT7_DESCRIPTOR(CDevice *pDevice, int Id, int n);

		int		Spec_Fun_GetLUT(CDevice* pDevice);
		
		CString Spec_Fun_Get_SV(CDevice *pDevice, HV_STRING_ID stringID );
		CString Spec_Fun_Get_APIV(CDevice *pDevice);
		CString Spec_Fun_Get_DailtV(CDevice *pDevice);
		void	Set_Shutter_Auto(CDevice *pDevice, int index);
		void	Set_Gain_Auto(CDevice *pDevice, int index);
		
		void Spec_Fun_Interface_1(CDevice*pDevice,HV_INTERFACE1_ID Interface_ID,int value);
//		void Spec_Trigger_Interval(CDevice*pDevice,HV_INTERFACE1_ID Interface_ID,int value);//xupxTrigger 0522
//		void Spec_Snap_Shoot_Num(CDevice*pDevice,HV_INTERFACE1_ID Interface_ID,int value);
//		void Spec_Fun_Color_Mode(CDevice*pDevice, HV_COMMAND_CODE Interface_ID, int value);

		CString Spec_Fun_Address_Get(CDevice*pDevice, int FuncID);

		void	ListAddBandWidthOrg(CDevice *pDevice);	// 带宽管理
		void	OnNetWorkSetting(CDevice *pDevice, int FunID, int nValue);
		int		Spec_NetWork_Get_Range(CDevice *pDevice, int FunID, int n);
		int		Spec_NetWork_Get(CDevice *pDevice, int readFunID);
		void	Spec_NetWork_Set(CDevice *pDevice, int readFunID, int nValue);
		void	RefreshDelayResult(int nValue);
		void	Set_Adv_Feature(CDevice *pDevice, int readFunID, int nValue);
		void	OnAdvFeatureSetting(CDevice *pDevice, int FunID, int nValue);

		int		Spec_Fun_Presence(CDevice*pDevice, int FunID);	//功能查询

		
		void OnStrobeOnOffChanged(CDevice*pDevice,int value);
		void OnTestImageChanged(CDevice*pDevice,int value);
		void OnTriggerDelayChanged(CDevice*pDevice,int value);
		void OnTriggerDelayValueChanged(CDevice*pDevice,int value);
		void OnTriggerDelayUnitChange(CDevice* pDevice, int value);
	    void On8or12BitModeChanged(CDevice*pDevice,int value);
	    void OnLeBeModeChanged(CDevice*pDevice,int value);
		void OnOutPutIO_0Changed(CDevice*pDevice,int value); 
        void OnOutPutIO_1Changed(CDevice*pDevice,int value);
		void OnOutPutIO_2Changed(CDevice*pDevice,int value);
		void OnOutPutIO_ControlChanged(CDevice*pDevice,HV_INTERFACE1_ID FuncID, int value);
//		void OnOutPutIO_0_ControlChanged(CDevice*pDevice,int value);
//        void OnOutPutIO_1_ControlChanged(CDevice*pDevice,int value);
//        void OnOutPutIO_2_ControlChanged(CDevice*pDevice,int value);
        void OnInPutIOSetChanged(CDevice*pDevice,int value); 
	    void OnTransfersDelayValueChanged(CDevice*pDevice,int value);
	    void OnFilterTimeChanged(CDevice*pDevice,int value);


		void ListAddInfoProperty(CDevice* pDevice);
		void ListAddVideoFormat(CDevice* pDevice);
		void ListAddShutter(CDevice* pDevice);
		void ListAddSnapMode(CDevice* pDevice);
		void ListAddADCControl(CDevice* pDevice);
		void ListAddGain(CDevice* pDevice);
		void ListAddBlank(CDevice* pDevice);

		//1410GC
		void OnBeginSave1410GC(CDevice *pDevice, int value);
		void OnSaveParam1410GC(CDevice *pDevice, int value);
		void OnLoadParam1410GC(CDevice *pDevice, int value);
		void ListAddInfoProperty_NetCam(CDevice *pDevice);	//xupx 0519
		void ListAddSnapMode_NetCam(CDevice *pDevice);
		void ListAddGain_NetCam(CDevice* pDevice);
		void ListAddOutPutIoSet_NetCam(CDevice *pDevice);  
// 		void ListAddOutPutIoControl1410GC(CDevice *pDevice);
		void ListAddSaveParam(CDevice *pDevice);
		void ListAddPassiveTransmit(CDevice *pDevice);
		void ListAddWB(CDevice* pDevice);
			
		//End 1410 list

		//PD1420EX
		void	OnLutNetCam(CDevice *pDevice, int index);
		void	OnMultiTriggerNumChanged(CDevice *pDevice, int index);
		void	OnMultiTriggerTimeChanged(CDevice *pDevice, int index);
		void	ListAddOutPutIoControl_NetCam(CDevice *pDevice);
		void	ListAdvTrigger(CDevice *pDevice);
		//End PD1420
 
		void ListAddShutter_NetCam(CDevice *pDevice);

 
		void ListAddTestImage(CDevice* pDevice);   
	    void ListAddTriggerDelay_NetCam(CDevice* pDevice); 
		void ListAddTriggerDelay(CDevice* pDevice); 
		void ListAddOutPutIoSet(CDevice* pDevice); 
		void ListAddOutPutIoControl(CDevice* pDevice);
		void ListAddInPutIoSet(CDevice* pDevice);
        void ListAdd8or12BitMode(CDevice* pDevice);
        void ListAddTransfersDelay(CDevice* pDevice);
        void ListAddFilterTime(CDevice* pDevice);
        void ListAddFilterTime_Readable(CDevice* pDevice);
		void ListAddAntiFilter(CDevice* pDevice);
		void ListAddACPhaseAdjust(CDevice* pDevice);
		
        void OnBrightnessChanged(CDevice* pDevice, int value);
        void OnWhiteBalanceUChanged(CDevice* pDevice, int value);
        void OnWhiteBalanceVChanged(CDevice* pDevice, int value);
        void OnWhiteBalanceGChanged(CDevice* pDevice, int value);	// add by xupx for 1410
        void OnFrameFrozenChanged(CDevice* pDevice, int index);


		void OnVideoModeChanged(CDevice *pDevice, int index); 
		void OnShutterSpeedChanged(CDevice *pDevice, int value);
		void OnSnapModeChanged(CDevice *pDevice, int index);
		void OnTriggerPolarityChanged(CDevice *pDevice, int index);
		void OnStrobePolarityChanged(CDevice *pDevice, int index);
		void OnBlackLevelEnableChanged(CDevice *pDevice, int index);
		void OnBlackLevelChanged(CDevice *pDevice, int value);
		void OnGainLevelChanged(CDevice *pDevice, int value);
		void OnBlankHorChanged(CDevice *pDevice, int value);
		void OnBlankVertChanged(CDevice *pDevice, int value);
		void OnFilterTimeUnitChanged(CDevice *pDevice, int index);
		//--PD1420
		void OnVideoModeChanged1420(CDevice *pDevice, int index);// xupx 模式选择		
		//--1410GC
		void OnShutterAuto(CDevice *pDevice, int index);		//快门 自动模式
		void OnGainAuto(CDevice *pDevice, int index);			//增益 自动模式
		void OnStrobeAuto(CDevice *pDevice, int index);			//闪光灯自动模式
		void OnConnectSpeed1410GC(CDevice *pDevice, int value);
		void OnAntiFlicker(CDevice* pDevice, int value);
		void OnAntiFlickerSource(CDevice* pDevice, int value);
		void OnACPhaseAdjust(CDevice* pDevice, int value);
		void OnWBCtrl(CDevice* pDevice, int FuncID, int value);

		CString DoOEM(CString strOEM, CString strModeName);
		
protected:
		// Message Handlers
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		
		/// Invoke the error message box
		void ReportError(HVBaseException& e);
		
		/// reference to the viewer's camera manager object
		CDeviceManager          &m_DeviceManager;
		/// reference to the application's main window
		CMainFrame              &m_MainFrame;	
		/// list view control
//		CPropertyListCtrl		m_ListView;	//xupxTest0801
	
		// camera video mode control 
		typedef map<int, HV_VIDEO_MODE>         Idx2Mode_t;
		Idx2Mode_t      m_Idx2Mode;
		HV_VIDEO_MODE	m_CurrentMode;
		int				m_CurrentModeIdx;
		
		
		//	camera snap mode control
		int			    m_CurrentSnapMode;
		int			    m_CurrentTriggerPolarity;
		int			    m_CurrentStrobePolarity;
		
		// A/D convert control
		int			    m_CurrentADLevel;
		int			    m_CurrentBlackLevelEnable;
		unsigned long	m_CurrentBlackLevel;

		// shutter control
		unsigned long	m_CurrentShutterSpeed;
		int				m_CurrentUnit;		 

		// blank control
		int				m_CurrentBlankHor;
		int				m_CurrentBlankVert;	

		BOOL			m_IS_OldITS;
		BOOL			m_IS_OldGC;
		BOOL			m_IS_OldGM;

		HHV				m_hhv;
		

};

#endif // !defined(AFX_PropertyView_H__F4CED1A3_9CED_4DF5_9C91_692B10F7E930__INCLUDED_)





















