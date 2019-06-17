// DlgSetParameter.cpp : implementation file
//

#include "stdafx.h"
#include "HVColor.h"
#include "DlgSetParameter.h"
#include "..\inc\HVDAILT.H"
#include "..\inc\imageload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CHECK_WND(id) hWnd = ::GetDlgItem(m_hWnd, id); \
::EnableWindow(hWnd, bEnable);

#define ENABLE_WND(id, bEnable) hWnd = ::GetDlgItem(m_hWnd, id); \
::EnableWindow(hWnd, bEnable);

/////////////////////////////////////////////////////////////////////////////
// CDlgSetParameter dialog


CDlgSetParameter::CDlgSetParameter(CWnd* pParent)
	: CDialog(CDlgSetParameter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetParameter)
	m_EditBlue = 0;
	m_EditGama = 0;
	m_EditRed = 0;
	m_Bayer = -1;
	m_bCorrect = FALSE;
	m_Light = -1;
	m_Quality = -1;
	m_bDoColor = FALSE;
	m_EditBrightness = 0;
	m_EditContrast = 0;
	m_EditSharpen = 0.0f;
	m_bEnableGamma = FALSE;
	m_bEnableBrightness = FALSE;
	m_bEnableContrast = FALSE;
	m_bEnableSharpen = FALSE;
	//}}AFX_DATA_INIT

	m_hMenu     = NULL;
	m_pTempBuff = NULL;
	m_nTempSize = 0;
	m_pDispBuff = NULL;
	m_nDispSize = 0;
	m_KeyPath   = COLOR_KEY;
	m_bShowDlg	= FALSE;

}

CDlgSetParameter::~CDlgSetParameter()
{
	delete m_pTempBuff;
	delete m_pDispBuff;
}

void CDlgSetParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetParameter)
	DDX_Control(pDX, IDC_SLIDER_SHARPEN, m_SliderSharpen);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_SliderContrast);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_SliderBrightness);
	DDX_Control(pDX, IDC_COMBO_BIT, m_BitCombo);
	DDX_Control(pDX, IDC_SLIDER_R, m_SliderRed);
	DDX_Control(pDX, IDC_SLIDER_GAMA, m_SliderGama);
	DDX_Control(pDX, IDC_SLIDER_B, m_SliderBlue);
	DDX_Control(pDX, IDC_PREVIEW_WINDOW, m_ImageView);
	DDX_Text(pDX, IDC_RATIO_B, m_EditBlue);
	DDV_MinMaxInt(pDX, m_EditBlue, 10, 500);
	DDX_Text(pDX, IDC_RATIO_GAMA, m_EditGama);
	DDV_MinMaxInt(pDX, m_EditGama, 10, 500);
	DDX_Text(pDX, IDC_RATIO_R, m_EditRed);
	DDV_MinMaxInt(pDX, m_EditRed, 10, 500);
	DDX_Radio(pDX, IDC_BAYER_GB, m_Bayer);
	DDX_Check(pDX, IDC_CHECK_CORRECT, m_bCorrect);
	DDX_Radio(pDX, IDC_LIGHT_DAYLIGHT, m_Light);
	DDX_Radio(pDX, IDC_QUALITY_LOW, m_Quality);
	DDX_Check(pDX, IDC_COLOR, m_bDoColor);
	DDX_Text(pDX, IDC_FACTOR_BRIGHTNESS, m_EditBrightness);
	DDV_MinMaxInt(pDX, m_EditBrightness, -150, 150);
	DDX_Text(pDX, IDC_FACTOR_CONTRAST, m_EditContrast);
	DDV_MinMaxInt(pDX, m_EditContrast, -50, 100);
	DDX_Text(pDX, IDC_FACTOR_SHARPEN, m_EditSharpen);
	DDV_MinMaxFloat(pDX, m_EditSharpen, 0.f, 5.f);
	DDX_Check(pDX, IDC_ENABLE_GAMMA, m_bEnableGamma);
	DDX_Check(pDX, IDC_ENABLE_BRIGHTNESS, m_bEnableBrightness);
	DDX_Check(pDX, IDC_ENABLE_CONTRAST, m_bEnableContrast);
	DDX_Check(pDX, IDC_ENABLE_SHARPEN, m_bEnableSharpen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetParameter, CDialog)
	//{{AFX_MSG_MAP(CDlgSetParameter)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_RATIO_B, OnKillfocusRatioB)
	ON_EN_KILLFOCUS(IDC_RATIO_R, OnKillfocusRatioR)
	ON_EN_KILLFOCUS(IDC_RATIO_GAMA, OnKillfocusRatioGama)
	ON_BN_CLICKED(IDC_CHECK_CORRECT, OnCheckCorrect)
	ON_BN_CLICKED(IDBALANCE, OnBalance)
	ON_BN_CLICKED(IDC_WB_RESET, OnWbReset)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_DEFAULT_GAMMA, OnDefaultGamma)
	ON_CBN_SELCHANGE(IDC_COMBO_BIT, OnSelchangeComboBit)
	ON_EN_KILLFOCUS(IDC_FACTOR_BRIGHTNESS, OnKillfocusEditBrightness)
	ON_EN_KILLFOCUS(IDC_FACTOR_CONTRAST, OnKillfocusEditContrast)
	ON_EN_KILLFOCUS(IDC_FACTOR_SHARPEN, OnKillfocusEditSharpen)
	ON_BN_CLICKED(IDC_DEFAULT_BRIGHTNESS, OnDefaultBrightness)
	ON_BN_CLICKED(IDC_DEFAULT_CONTRAST, OnDefaultContrast)
	ON_BN_CLICKED(IDC_DEFAULT_SHARPEN, OnDefaultSharpen)
	ON_BN_CLICKED(IDC_ENABLE_BRIGHTNESS, OnEnableBrightness)
	ON_BN_CLICKED(IDC_ENABLE_GAMMA, OnEnableGamma)
	ON_BN_CLICKED(IDC_ENABLE_CONTRAST, OnEnableContrast)
	ON_BN_CLICKED(IDC_ENABLE_SHARPEN, OnEnableSharpen)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BAYER_GB, IDC_BAYER_RG, OnBayerChange)
	ON_COMMAND_RANGE(IDC_QUALITY_LOW, IDC_QUALITY_HIGH, OnQualityChange)
	ON_COMMAND_RANGE(IDC_LIGHT_DAYLIGHT, IDC_LIGHT_INCANDESCENCE, OnLightChange)
	ON_MESSAGE(WM_SETPARAMDLG_UPDATE, OnUpdateView)
	ON_MESSAGE(WM_SETPARAMDLG_SHOWDIALOG, OnShowDlg)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CDlgSetParameter implement

void CDlgSetParameter::InitializeControl()
{
	m_Light      = m_pParam.m_Illuminant;
	m_bCorrect   = m_pParam.m_bCorrect;
    m_bDoColor   = m_pParam.m_bDoColor;
	m_nLastColor = m_pParam.m_bDoColor;
	m_Quality    = ConvertToControl(m_pParam.m_ConvertType);
	m_Bayer      = BayerToControl(m_pParam.m_BayerLayout);
	m_EditRed    = RATIOTOINTEGER(m_pParam.m_RatioRed);
	m_EditGama   = RATIOTOINTEGER(m_pParam.m_RatioGama);
	m_EditBlue   = RATIOTOINTEGER(m_pParam.m_RatioBlue);
	m_EditBrightness = m_pParam.m_Brightness;
	m_EditContrast   = m_pParam.m_Contrast;
	m_EditSharpen    = m_pParam.m_Sharpen;

	m_SliderRed.SetRange(10, 500);
	m_SliderBlue.SetRange(10, 500);
	m_SliderGama.SetRange(10, 500);
	m_SliderBrightness.SetRange(-150,150,TRUE);
	m_SliderContrast.SetRange(-50,100,TRUE);
	m_SliderSharpen.SetRange(0,5*10,TRUE);

	m_SliderBlue.SetPos(m_EditBlue);
	m_SliderGama.SetPos(m_EditGama);
	m_SliderRed.SetPos(m_EditRed);
	m_SliderBrightness.SetPos(m_EditBrightness);
	m_SliderContrast.SetPos(m_EditContrast);
	m_SliderSharpen.SetPos(m_EditSharpen*10);

	m_bBalance = FALSE;	//	 by xupx 2009.02.12
}

void CDlgSetParameter::EnableControl(BOOL bEnable)
{
	HWND hWnd = ::GetDlgItem(m_hWnd, IDC_LIGHT_DAYLIGHT);
	::EnableWindow(hWnd, m_bCorrect);
	hWnd = ::GetDlgItem(m_hWnd, IDC_LIGHT_FLUENCE);
	::EnableWindow(hWnd, m_bCorrect);
	hWnd = ::GetDlgItem(m_hWnd, IDC_LIGHT_INCANDESCENCE);
	::EnableWindow(hWnd, m_bCorrect);
}

int CDlgSetParameter::BayerToControl(HV_BAYER_LAYOUT layout)
{
	return (layout - 1);
}

HV_BAYER_LAYOUT CDlgSetParameter::ControlToBayer(int control)
{
	return (HV_BAYER_LAYOUT)(control + 1);
}

int CDlgSetParameter::ConvertToControl(HV_BAYER_CONVERT_TYPE type)
{
	switch( type ) 
	{
	case BAYER2RGB_NEIGHBOUR:
		return 0;
	case BAYER2RGB_BILINER:
		return 1;
	case BAYER2RGB_PIXELGROUPING:
		return 2;
	default:
		return 0;
	}
}

HV_BAYER_CONVERT_TYPE CDlgSetParameter::ControlToConvert(int control)
{
	switch( control ) 
	{
	case 0:
		return BAYER2RGB_NEIGHBOUR;
	case 1:
		return BAYER2RGB_BILINER;
	case 2:
		return BAYER2RGB_PIXELGROUPING;
	default:
		return BAYER2RGB_NEIGHBOUR;
	}
}

//////////////////////////////////////////////////////////////////////////
// Update image data

void CDlgSetParameter::UpdatePreView(void *pDst, void *pSrc, int nWid, int nHei)
{
	pDst = m_pParam.m_data.pCidDest;
	pSrc = m_pParam.m_data.pCidSrce;
	nWid = m_pParam.m_info.ciiDest.iV11Width;
	nHei = m_pParam.m_info.ciiDest.iV11Height;
	int nDepth = m_pParam.m_info.ciiDest.iV11DataDepth;

	if (m_bBalance == TRUE)
	{
		m_SliderRed.SetPos(m_EditRed);
		m_SliderBlue.SetPos(m_EditBlue);
		SetDlgItemInt(IDC_RATIO_R, m_EditRed);
		SetDlgItemInt(IDC_RATIO_B, m_EditBlue);
		m_bBalance = FALSE;
	}

	
	// Save temp data
	if (m_nDispSize < (UINT)(nWid * nHei* (nDepth/8))) 
	{
		delete        []m_pDispBuff;
		m_nDispSize = nWid * nHei * (nDepth/8);
		m_pDispBuff = new BYTE[m_nDispSize];
	}
	
 	memcpy(m_pDispBuff, pDst, nWid * nHei * (nDepth/8));

	m_ImageView.UpdateImage(m_pDispBuff, nWid, nHei);
	// Update information
	// Update image display

/*
	m_ImageView.GetRectInfo(&m_strInfo, pDst, nWid, nHei);
	m_ImageView.GetPointInfo(&m_strInfo1, pDst, nWid, nHei);
	SetDlgItemText(IDC_STATUS_RECT, m_strInfo);
	SetDlgItemText(IDC_STATUS_POINT, m_strInfo1);
*/

	
	m_ImageView.Invalidate();
	m_ImageView.UpdateWindow();
}

void CDlgSetParameter::WhiteBalance(void *pDst, void *pSrc, int nWid, int nHei)
{
	m_pParam.m_RatioRed  = 1.0;
	m_pParam.m_RatioBlue = 1.0;
	m_pParam.UpdateLutGama();

	ConvertBayer2Rgb(pDst, pSrc, nWid, nHei);

	double dRatioR = 1.0;
	double dRatioG = 1.0;
	double dRatioB = 1.0;

	CRect rect;
	m_ImageView.GetSelectRect(&rect, nWid, nHei);

	//选取rect范围内的pDest，拷入pRectBuf
	//begin by lm 20090709
	BYTE* pRectBuf = new BYTE[rect.Width() * rect.Height() * 3];
	BYTE* pTemp = (BYTE*)pDst;

	for (int i=rect.top; i<rect.bottom; i++)
	{
		for (int j=rect.left; j<rect.right; j++)
		{
			pRectBuf[((i-rect.top)*rect.Width() + (j-rect.left))*3]   = pTemp[(i*nWid + j)*3];
			pRectBuf[((i-rect.top)*rect.Width() + (j-rect.left))*3+1] = pTemp[(i*nWid + j)*3+1];
			pRectBuf[((i-rect.top)*rect.Width() + (j-rect.left))*3+2] = pTemp[(i*nWid + j)*3+2];
		}
	}
	
	CColorRender::PlxGetWBalanceRatio(pRectBuf,
		rect.Width(), rect.Height(), &dRatioR, &dRatioG, &dRatioB);

	delete[] pRectBuf;
	//end

	dRatioR = dRatioR < 0.1 ? 0.1 : (dRatioR > 5 ? 5 : dRatioR);
	dRatioB = dRatioB < 0.1 ? 0.1 : (dRatioB > 5 ? 5 : dRatioB);

	m_pParam.m_RatioRed  = dRatioR;
	m_pParam.m_RatioBlue = dRatioB;
	m_pParam.UpdateLutGama();

	m_EditRed  = RATIOTOINTEGER(dRatioR);
	m_EditBlue = RATIOTOINTEGER(dRatioB);
}

LRESULT CDlgSetParameter::OnShowDlg(WPARAM wParam, LPARAM lParam)
{
	if ( m_bShowDlg )
	{
		InitializeControl();
		UpdateData(false);
		EnableControl(m_pParam.m_bCorrect);
		EnableGammaControl(m_bEnableGamma);
		EnableBrightnessControl(m_bEnableBrightness);
		EnableContrastControl(m_bEnableContrast);
		EnableSharpenControl(m_bEnableSharpen);
        EnableFullControl(m_pParam.m_bDoColor);

//		m_ImageView.UpdateImage(NULL, 0, 0);
		m_bBalance = FALSE;		//by xupx 2009.02.12
		
		CheckMenuItem(m_hMenu, ID_COLOR_MENU, MF_CHECKED);
		m_bShowDlg = TRUE;//add by xupx 
		

		/***********add by xupx 2009.02.09 Init the list **********/
		int nDataDepth		=m_nSrcDepth;
		m_ShiftMax = nDataDepth - 8;
		
		strlist.push_back("0 --  7");
		strlist.push_back("1 --  8");
		strlist.push_back("2 --  9");
		strlist.push_back("3 -- 10");
		strlist.push_back("4 -- 11");
		strlist.push_back("5 -- 12");
		strlist.push_back("6 -- 13");
		strlist.push_back("7 -- 14");
		strlist.push_back("8 -- 15");
		
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		int i = 0;
		
		m_BitCombo.ResetContent();
		for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			m_BitCombo.InsertString(i, *pString);
			
			if(m_ShiftMax == i)
				break;
			
			i++;
			
			//list[i++] = (*pString);
		}

		m_BitCombo.SetCurSel(m_CurrentShift);
		
		/************ xupx end**************/
		
		ShowWindow(SW_SHOW);
		SetTimer(0, 1000, NULL);

		//cleared by xupx 2009.02.05
	}
	else
	{
		CheckMenuItem(m_hMenu, ID_COLOR_MENU, MF_UNCHECKED);
		ShowWindow(SW_HIDE);
		KillTimer(0);
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// Initialize dialog

BOOL CDlgSetParameter::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// initialize image rect
	m_ImageView.InitialRect(m_pBmpInfo, m_nNowDepth);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////
// CDlgSetParameter message handlers

// Radio control message handlers

void CDlgSetParameter::OnBayerChange(UINT nID)
{
	UpdateData(true);
	m_pParam.m_BayerLayout = ControlToBayer(m_Bayer);
}

void CDlgSetParameter::OnQualityChange(UINT nID)
{
	UpdateData(true);
	m_pParam.m_ConvertType = ControlToConvert(m_Quality);
}

void CDlgSetParameter::OnLightChange(UINT nID)
{
	UpdateData(true);
	m_pParam.m_Illuminant = (HV_ILLUMINANT)m_Light;
}

// Check control message handler

void CDlgSetParameter::OnCheckCorrect() 
{
	UpdateData(true);
	m_pParam.m_bCorrect = m_bCorrect;
	EnableControl(m_bCorrect);
}

// Slider control message handler

void CDlgSetParameter::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(m_bShowDlg == FALSE) return; // add by xupx 2008.08.06
									//if the Dlg isn't show, the MSG will be ignored 

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	if ( pScrollBar == (CWnd *)&m_SliderRed )
	{
		m_pParam.m_RatioRed = INTEGERTORATIO(m_SliderRed.GetPos());
		m_EditRed = m_SliderRed.GetPos();
		m_pParam.UpdateLutRed();
	}

	if ( pScrollBar == (CWnd *)&m_SliderBlue )
	{
		m_pParam.m_RatioBlue = INTEGERTORATIO(m_SliderBlue.GetPos());
		m_EditBlue = m_SliderBlue.GetPos();
		m_pParam.UpdateLutBlue();
	}

	if ( pScrollBar == (CWnd *)&m_SliderGama )
	{
		m_pParam.m_RatioGama = INTEGERTORATIO(m_SliderGama.GetPos());
		m_EditGama = m_SliderGama.GetPos();
		m_pParam.UpdateLutGama();
	}
	if ( pScrollBar == (CWnd *)&m_SliderBrightness )
	{
		m_pParam.m_Brightness = m_SliderBrightness.GetPos();
		m_EditBrightness = m_SliderBrightness.GetPos();
	}
	if ( pScrollBar == (CWnd *)&m_SliderContrast )
	{
		m_pParam.m_Contrast = m_SliderContrast.GetPos();
		m_EditContrast = m_SliderContrast.GetPos();
	}
	if ( pScrollBar == (CWnd *)&m_SliderSharpen )
	{
		m_pParam.m_Sharpen = m_SliderSharpen.GetPos() / 10.0;
		m_EditSharpen = m_SliderSharpen.GetPos() / 10.0;
	}

	UpdateData(false);
}

// Edit control message handler

void CDlgSetParameter::OnKillfocusRatioR() 
{
	if ( UpdateData(true) )
	{
		m_pParam.m_RatioRed = INTEGERTORATIO(m_EditRed);
		m_SliderRed.SetPos(m_EditRed);
		m_pParam.UpdateLutRed();
	}
	else
	{
		m_EditRed = RATIOTOINTEGER(m_pParam.m_RatioRed);
		UpdateData(false);
	}
}

void CDlgSetParameter::OnKillfocusRatioB() 
{
	if ( UpdateData(true) )
	{
		m_pParam.m_RatioBlue = INTEGERTORATIO(m_EditBlue);
		m_SliderBlue.SetPos(m_EditBlue);
		m_pParam.UpdateLutBlue();
	}
	else
	{
		m_EditBlue = RATIOTOINTEGER(m_pParam.m_RatioBlue);
		UpdateData(false);
	}
}

void CDlgSetParameter::OnKillfocusRatioGama() 
{
	if ( UpdateData (true) )
	{
		m_pParam.m_RatioGama = INTEGERTORATIO(m_EditGama);
		m_SliderGama.SetPos(m_EditGama);
		m_pParam.UpdateLutGama();
	}
	else
	{
		m_EditGama = RATIOTOINTEGER(m_pParam.m_RatioGama);
		UpdateData(false);
	}
}

// Buttons control message handler

void CDlgSetParameter::OnCancel() 
{
	m_pParam = m_sParam;
	m_bShowDlg = FALSE;
	OnShowDlg(0,0);
}

void CDlgSetParameter::OnOK() 
{
	m_pParam.SaveParam(m_KeyPath);
	m_sParam = m_pParam;
	
	m_bShowDlg = FALSE;	
	OnShowDlg(0,0);
}


void CDlgSetParameter::OnBalance() 
{
	m_bBalance = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// User message handler

LRESULT CDlgSetParameter::OnUpdateView(WPARAM wParam, LPARAM lParam)
{
	UpdatePreView(NULL, NULL, 0, 0);
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// PlugIn interface

void CDlgSetParameter::GetName(char * buffer)
{ 
	::LoadString(AfxGetResourceHandle(), IDS_PLUG_NAME, buffer, 200);	
}

void CDlgSetParameter::GetInfo(char * buffer)
{
	::LoadString(AfxGetResourceHandle(), IDS_PLUG_INFO, buffer, 200);
}

DWORD CDlgSetParameter::GetSpecVersion()
{
	return 0x00010001;
}

void CDlgSetParameter::Initialize(CCamera hCamera, CManager hManager)
{
	char buffer[100];
	int  nBuffSize;

	//add by xupx 2008.09.05
	m_bBalance = TRUE;
	m_bSavePara = TRUE;
	//init keypath
	m_KeyPath.Format("%s", hManager.pRegistryPath);
	
	// initial parameter
	HVGetDeviceInfo(hCamera.hDevice, DESC_DEVICE_TYPE, NULL, &nBuffSize);
	HVGetDeviceInfo(hCamera.hDevice, DESC_DEVICE_TYPE, buffer, &nBuffSize);	
	m_pParam.m_CameraType = (HVTYPE)*buffer;

	if (m_pParam.m_CameraType < 0)		// add by xupx 2009.09.17
	{
		m_pParam.m_CameraType = (HVTYPE)(int (SUPPORT_CAMERA));
	}

	m_pParam.ReadParam(m_KeyPath);

	HVSTATUS status;
	status = HVGetDeviceInfo(hCamera.hDevice, DESC_DEVICE_BAYER_LAYOUT, NULL, &nBuffSize);	
	if(status == STATUS_OK)
	{
		HVGetDeviceInfo(hCamera.hDevice, DESC_DEVICE_BAYER_LAYOUT, buffer, &nBuffSize);	
		m_pParam.m_BayerLayout = (HV_BAYER_LAYOUT)*buffer;
	}

	m_sParam = m_pParam;

	m_bDoColor   = m_pParam.m_bDoColor;
	m_nLastColor = m_pParam.m_bDoColor;


	m_pBmpInfo = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
	m_pBmpInfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biPlanes		= 1;
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;

	for (int i = 0; i < 256; i++){
		m_pBmpInfo->bmiColors[i].rgbBlue		= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbGreen		= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbRed			= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbReserved	= 0;
	}

	m_nNowDepth = m_pParam.m_info.ciiDest.iV11DataDepth;
	m_nLastDepth = m_pParam.m_info.ciiDest.iV11DataDepth;
	//end

	// initial menu
	m_hMenu = (HMENU)hManager.hMenu;
	::LoadString(AfxGetResourceHandle(), ID_COLOR_MENU, buffer, 100);
    ::InsertMenu((HMENU)m_hMenu, (UINT)-1, MF_BYPOSITION, ID_COLOR_MENU, buffer);

	// initial parameter dialog
	CWnd * pWnd = CWnd::FromHandle((HWND)hManager.hWndParent);
	Create(IDD_DLG_SETPARAMETER, pWnd);
	::LoadString(AfxGetResourceHandle(), IDS_DLG_NAME, buffer, 100);
	SetWindowText(/*CString(hCamera.pName) + */CString(buffer));
	m_Place.InitPlacement(this, m_KeyPath);
	m_bShowDlg = m_Place.m_Visible;	
	OnShowDlg(0,0);
	//initial bitList  add by xupx 2009.02.09
	m_CurrentShift = 0;
	
}

void CDlgSetParameter::Uninitialize()
{
	DeleteMenu(m_hMenu, ID_COLOR_MENU, MF_BYCOMMAND);
	m_Place.SavePlacement(this, m_KeyPath);
	DestroyWindow();

	if(m_pBmpInfo != NULL)
	{
		delete m_pBmpInfo;
	}
}

bool CDlgSetParameter::OnUserCommand(unsigned int id)
{
	switch( id )
	{
	case ID_COLOR_MENU:
		m_bShowDlg = !IsWindowVisible();
		OnShowDlg(0,0);
		break;

	default:
		return false;
	}

	return true;
}

void CDlgSetParameter::OnManagerNotify(CConvertImageInfo *pImageInfo)
{
	/*********add by xupx 2009.02.09**********/
	m_nSrcDepth = pImageInfo->ciiSrce.iV11DataDepth;
	m_nDestDepth = pImageInfo->ciiDest.iV11DataDepth;
	m_ShiftMax = m_nSrcDepth - 8;
	m_CurrentShift = m_ShiftMax;

	m_bShowDlg = IsWindowVisible();
	OnShowDlg(0,0);
	/**************** End ********************/
}

void CDlgSetParameter::OnStartGrabbing(CConvertImageInfo *pImageInfo)
{
	/*********add by xupx 2009.02.09**********/
	m_ShiftMax = m_nSrcDepth - 8;
//	m_CurrentShift = m_ShiftMax;
	/*****************************************/
	
	m_nSrcDepth = pImageInfo->ciiSrce.iV11DataDepth;
	m_nLastDepth = m_pParam.m_info.ciiDest.iV11DataDepth;
	ATLTRACE(" Start Snapping~~~~~~~~~~~~~~~~~~");
}

void CDlgSetParameter::OnStopGrabbing(CConvertImageInfo *pImageInfo, CConvertImageData *pImageData)
{
}
void CDlgSetParameter::OnShowFrame(CConvertImageInfo *pImageInfo, CConvertImageData *pImageData)//xdtest3
{ 
	m_pParam.m_info = *pImageInfo;
	m_pParam.m_data = *pImageData;
	 
	xdMakeDat2Show((BYTE*)m_pParam.m_data.pCidSrce, (BYTE*)m_pParam.m_data.pCidDest,1392,1040);
}

void CDlgSetParameter::RawDatato8(BYTE *pInRaw, BYTE *pOutRaw, int nWidth, int nHeigh, int n)	//10bit
{
	BYTE *pRaw = (BYTE *)pInRaw;
	WORD *wpBuf = (WORD *)pRaw;
	WORD nTemp;
	
	int nTopV[] = {255,511,1023,2047,4095};
	
	for(int i = 0; i <nWidth * nHeigh; i++)
	{
		nTemp = (wpBuf[i]) > nTopV[n] ? nTopV[n] :wpBuf[i] ;
		
		nTemp = (nTemp>> n) & 0xff;
		
		pOutRaw[i] = (BYTE)nTemp;
	}
}

void CDlgSetParameter::ConvertBayer2Rgb(void *pDst, void *pSrc, int nWid, int nHei)
{
	//
	// [!BUG] In the previous version, i user m_bDoColor to check whether should do color/gray
	// conversion, it's wrong, because sometime the m_bDoColor not have the right value.
	//
	unsigned long nDepth = m_pParam.m_info.ciiDest.iV11DataDepth;//为了使用方便增加了nDepth

	if (m_pParam.m_bDoColor) 
	{
		if ( (nDepth < 24) && (m_nTempSize < m_pParam.m_info.ciiDest.iV11SizeImage * 3) )
		{
			delete m_pTempBuff;
			m_nTempSize = m_pParam.m_info.ciiDest.iV11SizeImage * 3;
			m_pTempBuff = new BYTE[m_nTempSize];
		}

		BYTE * pDst1 = nDepth < 24 ? m_pTempBuff : (BYTE*)pDst;

		if ( m_pParam.m_bCorrect )
		{
			ColorRender.ConvertBayer2Rgb_Ex((BYTE*)pDst1, (BYTE*)pSrc, nWid, nHei, 
				m_pParam.m_ConvertType, m_pParam.m_LutR, m_pParam.m_LutG, m_pParam.m_LutB, 		
				true, m_pParam.m_BayerLayout, m_pParam.m_Illuminant, m_pParam.m_CameraType);
		}
		else 
		{
			ColorRender.PlxConvertBayer2Rgb((BYTE*)pDst1, (BYTE*)pSrc, nWid, nHei,
				m_pParam.m_ConvertType, 	m_pParam.m_LutR, m_pParam.m_LutG, m_pParam.m_LutB,	
				true, m_pParam.m_BayerLayout);
		}

		if ( nDepth == 8 )	// cleared by xupx 2009.02.05
		{
			ColorRender.Color2Gray((BYTE*)pDst, (BYTE*)pDst1, nWid, nHei);
		}
	}
	else
	{
		BYTE* pDst1 = (BYTE*)pDst;
		BYTE* pSrc1 = (BYTE*)pSrc;

		if ( nDepth == 24 )
		{
			pDst1 += nWid * nHei * 3;
			for(int i = 0; i<nHei; i++)
			{
				pDst1 -= 3 * nWid;
				for(int j = 0; j<nWid; j++)
				{
					pDst1[3*j]     = pSrc1[j];
					pDst1[3*j + 1] = pSrc1[j];
					pDst1[3*j + 2] = pSrc1[j];
				}
				pSrc1 += nWid;
			} 
		}
		else
		{
			pDst1 += nWid * nHei;
			for(int i = 0; i<nHei; i++)
			{
				pDst1 -= nWid;
				memcpy(pDst1, pSrc1, nWid);
				pSrc1 += nWid;
			} 
		}
	}
}


void CDlgSetParameter::OnWbReset() 
{
	m_pParam.m_RatioRed  = 1.0;
    m_pParam.m_RatioBlue = 1.0;

    m_EditRed = 100;
    m_EditBlue = 100;
	
    m_pParam.UpdateLutRed();
    m_pParam.UpdateLutBlue();
	
	m_SliderBlue.SetPos(m_EditBlue);
	m_SliderRed.SetPos(m_EditRed);
	
    UpdateData(0);
}



void CDlgSetParameter::EnableFullControl(BOOL bEnable)
{
    HWND hWnd;
	
    CHECK_WND(IDC_CHECK_CORRECT);
	
	// bayertype
	CHECK_WND(IDC_BAYER_TYPE);
    CHECK_WND(IDC_BAYER_GB);
    CHECK_WND(IDC_BAYER_GR);
    CHECK_WND(IDC_BAYER_BG);
    CHECK_WND(IDC_BAYER_RG);	
	
	//Gamma
	CHECK_WND(IDC_ENABLE_GAMMA);
	CHECK_WND(IDC_GROUP_GAMMA);
	if (!bEnable && m_bEnableGamma)
	{
		m_bEnableGamma = FALSE;
		UpdateData(FALSE);
		EnableGammaControl(m_bEnableGamma);
	}

	// White balance
	if(m_nDestDepth == 8 || bEnable==FALSE)
	{
		//W.B.
		ENABLE_WND(IDC_GROUP_WHB, FALSE);
		ENABLE_WND(IDC_SLIDER_R, FALSE);
		ENABLE_WND(IDC_SLIDER_B, FALSE);
		ENABLE_WND(IDC_STATIC_R_MIN, FALSE);
		ENABLE_WND(IDC_STATIC_R_MAX, FALSE);
		ENABLE_WND(IDC_STATIC_B_MIN, FALSE);
		ENABLE_WND(IDC_STATIC_B_MAX, FALSE);
		ENABLE_WND(IDC_RATIO_R, FALSE);
		ENABLE_WND(IDC_RATIO_B, FALSE);
		ENABLE_WND(IDC_STATIC_R, FALSE);
		ENABLE_WND(IDC_STATIC_B, FALSE);
		ENABLE_WND(IDBALANCE, FALSE);
		ENABLE_WND(IDC_DEFAULT_WHB, FALSE);

		//Sharpen
		ENABLE_WND(IDC_ENABLE_SHARPEN,FALSE);
		ENABLE_WND(IDC_GROUP_SHARPEN, FALSE);
		if (m_bEnableSharpen)
		{
			m_bEnableSharpen = FALSE;
			UpdateData(FALSE);
			EnableSharpenControl(m_bEnableSharpen);
		}
	}
	else
	{
		//W.B.
		CHECK_WND(IDC_GROUP_WHB);
		CHECK_WND(IDC_SLIDER_R);
		CHECK_WND(IDC_SLIDER_B);
		CHECK_WND(IDC_STATIC_R_MIN);
		CHECK_WND(IDC_STATIC_R_MAX);
		CHECK_WND(IDC_STATIC_B_MIN);
		CHECK_WND(IDC_STATIC_B_MAX);
		CHECK_WND(IDC_RATIO_R);
		CHECK_WND(IDC_RATIO_B);
		CHECK_WND(IDC_STATIC_R);
		CHECK_WND(IDC_STATIC_B);
		CHECK_WND(IDBALANCE);
		CHECK_WND(IDC_DEFAULT_WHB);

		CHECK_WND(IDC_GROUP_SHARPEN);
		CHECK_WND(IDC_ENABLE_SHARPEN);
	}

	//
    // Only if bEnable is TRUE and m_bCorrect is TRUE then,
    // enable windows of FOUR CORRECT RATIO BUTTONS.
    //
	CHECK_WND(IDC_GROUP_LIGHT);
	
    if (bEnable && m_bCorrect) {
        ENABLE_WND(IDC_LIGHT_DAYLIGHT, TRUE);
        ENABLE_WND(IDC_LIGHT_FLUENCE, TRUE);
        ENABLE_WND(IDC_LIGHT_INCANDESCENCE, TRUE);
		
    } else {
        ENABLE_WND(IDC_LIGHT_DAYLIGHT, FALSE);
        ENABLE_WND(IDC_LIGHT_FLUENCE, FALSE);
        ENABLE_WND(IDC_LIGHT_INCANDESCENCE, FALSE);
    }
}

void CDlgSetParameter::OnColor() 
{
	UpdateData(true);
	m_nLastColor = m_pParam.m_bDoColor;
    m_pParam.m_bDoColor = m_bDoColor;
    EnableFullControl(m_bDoColor);
}

void CDlgSetParameter::OnDefaultGamma() 
{
	m_EditGama = 100;
	m_pParam.m_RatioGama = 1.0;
	m_pParam.UpdateLutGama();
	m_SliderGama.SetPos(m_EditGama);
	UpdateData(0);
}


void CDlgSetParameter::OnSelchangeComboBit() 
{
	int n = m_BitCombo.GetCurSel();
	m_CurrentShift = n;
}


BOOL CDlgSetParameter::isSupportedColorCode(HV_COLOR_CODE code)
{
	BOOL b_isSuported = FALSE;
	switch ((unsigned long)code)
	{
		case 0:
		case 1:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:

		case 128:
		case 129:
		case 130:
		case 131:
		case 132:
		case 133:
		case 134:
		case 135:
		case 136:
		case 137:
		case 138:
		case 139:

		case 160:
		case 170:
			b_isSuported = TRUE;
			break;
		default:
			break;
	}
	return b_isSuported;
		
}

void CDlgSetParameter::OnKillfocusEditBrightness() 
{
	// TODO: Add your control notification handler code here
	if ( UpdateData (true) )
	{
		m_pParam.m_Brightness = m_EditBrightness;
		m_SliderBrightness.SetPos(m_EditBrightness);
	}
	else
	{
		m_EditBrightness = m_pParam.m_Brightness;
		UpdateData(false);
	}
}

void CDlgSetParameter::OnKillfocusEditContrast() 
{
	// TODO: Add your control notification handler code here
	if ( UpdateData (TRUE) )
	{
		m_pParam.m_Contrast = m_EditContrast;
		m_SliderContrast.SetPos(m_EditContrast);
	}
	else
	{
		m_EditContrast = m_pParam.m_Contrast;
		UpdateData(FALSE);
	}
}

void CDlgSetParameter::OnKillfocusEditSharpen() 
{
	// TODO: Add your control notification handler code here
	if ( UpdateData (TRUE) )
	{
		m_pParam.m_Sharpen = m_EditSharpen;
		m_SliderSharpen.SetPos(m_EditSharpen * 10);
	}
	else
	{
		m_EditSharpen = m_pParam.m_Sharpen;
		UpdateData(FALSE);
	}
}

void CDlgSetParameter::OnDefaultBrightness() 
{
	// TODO: Add your control notification handler code here
	m_EditBrightness = 0;
	m_pParam.m_Brightness = 0;
	m_SliderBrightness.SetPos(m_EditBrightness);
	UpdateData(FALSE);
}

void CDlgSetParameter::OnDefaultContrast() 
{
	// TODO: Add your control notification handler code here
	m_EditContrast = 0;
	m_pParam.m_Contrast = 0;
	m_SliderContrast.SetPos(m_EditContrast);
	UpdateData(FALSE);
}

void CDlgSetParameter::OnDefaultSharpen() 
{
	// TODO: Add your control notification handler code here
	m_EditSharpen = 0.0;
	m_pParam.m_Sharpen = 0.0;
	m_SliderSharpen.SetPos(m_EditSharpen * 10);
	UpdateData(FALSE);
}

void CDlgSetParameter::EnableGammaControl(BOOL bEnable)
{
	HWND hWnd;

	CHECK_WND(IDC_STATIC_GAMMA);
	CHECK_WND(IDC_SLIDER_GAMA);
	CHECK_WND(IDC_STATIC_GAMA_MIN);
	CHECK_WND(IDC_STATIC_GAMA_MAX);
	CHECK_WND(IDC_RATIO_GAMA);
	CHECK_WND(IDC_DEFAULT_GAMMA);
}

void CDlgSetParameter::EnableBrightnessControl(BOOL bEnable)
{
	HWND hWnd;

	CHECK_WND(IDC_SLIDER_BRIGHTNESS);
	CHECK_WND(IDC_STATIC_BRIGHTNESS_MIN);
	CHECK_WND(IDC_STATIC_BRIGHTNESS_MAX);
	CHECK_WND(IDC_FACTOR_BRIGHTNESS);
	CHECK_WND(IDC_DEFAULT_BRIGHTNESS);
}

void CDlgSetParameter::EnableContrastControl(BOOL bEnable)
{
	HWND hWnd;

	CHECK_WND(IDC_SLIDER_CONTRAST);
	CHECK_WND(IDC_STATIC_CONTRAST_MIN);
	CHECK_WND(IDC_STATIC_CONTRAST_MAX);
	CHECK_WND(IDC_FACTOR_CONTRAST);
	CHECK_WND(IDC_DEFAULT_CONTRAST);
}

void CDlgSetParameter::EnableSharpenControl(BOOL bEnable)
{
	HWND hWnd;

	CHECK_WND(IDC_SLIDER_SHARPEN);
	CHECK_WND(IDC_STATIC_SHARPEN_MAX);
	CHECK_WND(IDC_STATIC_SHARPEN_MIN);
	CHECK_WND(IDC_FACTOR_SHARPEN);
	CHECK_WND(IDC_DEFAULT_SHARPEN);		
}

void CDlgSetParameter::OnEnableBrightness() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	EnableBrightnessControl(m_bEnableBrightness);
}

void CDlgSetParameter::OnEnableGamma() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	EnableGammaControl(m_bEnableGamma);
}

void CDlgSetParameter::OnEnableContrast() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	EnableContrastControl(m_bEnableContrast);
}

void CDlgSetParameter::OnEnableSharpen() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	EnableSharpenControl(m_bEnableSharpen);
}

void CDlgSetParameter::OnTimer(UINT nIDEvent) 
{
	int nWid = m_pParam.m_info.ciiDest.iV11Width;
	int nHei = m_pParam.m_info.ciiDest.iV11Height;

	m_ImageView.GetRectInfo(&m_strInfo, m_pParam.m_data.pCidDest, nWid, nHei);
	m_ImageView.GetPointInfo(&m_strInfo1, m_pParam.m_data.pCidDest, nWid, nHei);
	SetDlgItemText(IDC_STATUS_RECT, m_strInfo);
	SetDlgItemText(IDC_STATUS_POINT, m_strInfo1);

	CDialog::OnTimer(nIDEvent);
}
