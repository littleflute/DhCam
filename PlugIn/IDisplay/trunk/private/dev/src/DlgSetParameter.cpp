// DlgSetParameter.cpp : implementation file
//

#include "stdafx.h"
#include "HVColor.h"
#include "DlgSetParameter.h"
#include "..\inc\HVDAILT.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_nBitDepth;
int nDepth;
int g_nColor;//用来记录上次是否色彩转换
BITMAPINFO g_BmpInfo;
HGLOBAL g_hBmpInfo;
BITMAPINFO *g_pBmpInfo;

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
	//}}AFX_DATA_INIT

	m_hMenu     = NULL;
	m_pTempBuff = NULL;
	m_nTempSize = 0;
	m_pDispBuff = NULL;
	m_nDispSize = 0;
	m_KeyPath   = COLOR_KEY;
	m_bShowDlg  = FALSE; 
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
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BAYER_GB, IDC_BAYER_RG, OnBayerChange)
	ON_COMMAND_RANGE(IDC_QUALITY_LOW, IDC_QUALITY_HIGH, OnQualityChange)
	ON_COMMAND_RANGE(IDC_LIGHT_DAYLIGHT, IDC_LIGHT_INCANDESCENCE, OnLightChange)
	ON_MESSAGE(WM_SETPARAMDLG_UPDATE, OnUpdateView)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// CDlgSetParameter implement

void CDlgSetParameter::InitializeControl()
{
	m_Light      = m_pParam.m_Illuminant;
	m_bCorrect   = m_pParam.m_bCorrect;
    m_bDoColor   = m_pParam.m_bDoColor;
	g_nColor	 = m_pParam.m_bDoColor;
	m_Quality    = ConvertToControl(m_pParam.m_ConvertType);
	m_Bayer      = BayerToControl(m_pParam.m_BayerLayout);
	m_EditRed    = RATIOTOINTEGER(m_pParam.m_RatioRed);
	m_EditGama   = RATIOTOINTEGER(m_pParam.m_RatioGama);
	m_EditBlue   = RATIOTOINTEGER(m_pParam.m_RatioBlue);

	m_SliderRed.SetRange(10, 500);
	m_SliderBlue.SetRange(10, 500);
	m_SliderGama.SetRange(10, 500);

	m_SliderBlue.SetPos(m_EditBlue);
	m_SliderGama.SetPos(m_EditGama);
	m_SliderRed.SetPos(m_EditRed);

	m_bBalance = FALSE;
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
	// White balance
	if (m_bBalance == TRUE)
	{
		WhiteBalance(pDst, pSrc, nWid, nHei);
		m_bBalance = FALSE;
	}
	
	// Color render
//	ConvertBayer2Rgb(pDst, pSrc, nWid, nHei, &m_pParam);

	// Save temp data
	if (m_nDispSize < (UINT)(nWid * nHei* (m_pParam.m_info.ImageBitDepth/8))) 
	{
		delete        m_pDispBuff;
		m_nDispSize = nWid * nHei * (m_pParam.m_info.ImageBitDepth/8);
		m_pDispBuff = new BYTE[m_nDispSize];
	}

	memcpy(m_pDispBuff, pDst, nWid * nHei * (m_pParam.m_info.ImageBitDepth/8));

	// Update image display
	m_ImageView.UpdateImage(m_pDispBuff, nWid, nHei);
	m_ImageView.Invalidate();
	m_ImageView.UpdateWindow();

	// Update information
	m_ImageView.GetRectInfo(&m_strInfo, pDst, nWid, nHei);
	SetDlgItemText(IDC_STATUS_RECT, m_strInfo);
	m_ImageView.GetPointInfo(&m_strInfo, pDst, nWid, nHei);	
	SetDlgItemText(IDC_STATUS_POINT, m_strInfo);

	nDepth = m_pParam.m_info.ImageBitDepth;

}

void CDlgSetParameter::WhiteBalance(void *pDst, void *pSrc, int nWid, int nHei)
{
	m_pParam.m_RatioRed  = 1.0;
	m_pParam.m_RatioBlue = 1.0;
	m_pParam.UpdateLutGama();

//	ConvertBayer2Rgb(pDst, pSrc, nWid, nHei, &m_pParam);

	CRect                      rect;
	m_ImageView.GetSelectRect(&rect, nWid, nHei);

	double dRatioR = 1.0;
	double dRatioG = 1.0;
	double dRatioB = 1.0;
	
	CColorRender::PlxGetWBalanceRatio((BYTE*)pDst + (rect.top * nWid + rect.left) * 3,
		rect.Width(), rect.Height(), &dRatioR, &dRatioG, &dRatioB);
	
	dRatioR = dRatioR < 0.1 ? 0.1 : (dRatioR > 5 ? 5 : dRatioR);
	dRatioB = dRatioB < 0.1 ? 0.1 : (dRatioB > 5 ? 5 : dRatioB);

	m_pParam.m_RatioRed  = dRatioR;
	m_pParam.m_RatioBlue = dRatioB;
	m_pParam.UpdateLutGama();

	m_EditRed  = RATIOTOINTEGER(dRatioR);
	m_EditBlue = RATIOTOINTEGER(dRatioB);
	m_SliderRed.SetPos(m_EditRed);
	m_SliderBlue.SetPos(m_EditBlue);	
	SetDlgItemInt(IDC_RATIO_R, m_EditRed);
	SetDlgItemInt(IDC_RATIO_B, m_EditBlue);
}

void CDlgSetParameter::ShowDialog(BOOL bShow)
{
	if ( bShow )
	{
		InitializeControl();
		UpdateData(false);
		EnableControl(m_pParam.m_bCorrect);
        EnableFullControl(m_pParam.m_bDoColor);
		
		m_ImageView.UpdateImage(NULL, 0, 0);
		m_bBalance = FALSE;
		
		CheckMenuItem(m_hMenu, ID_COLOR_MENU, MF_CHECKED);
		m_bShowDlg = TRUE;//add by xupx 
		
		ShowWindow(SW_SHOW);

		//hj 2007.9.19
		if ( m_pParam.m_info.ImageBitDepth == 8 )
		{
			BOOL bEnable;
			HWND hWnd = NULL;
			
			bEnable = FALSE;
			if (hWnd != NULL) 
			{
				CHECK_WND(IDBALANCE);
			}
		}
		//end
	}
	else
	{
		CheckMenuItem(m_hMenu, ID_COLOR_MENU, MF_UNCHECKED);
		ShowWindow(SW_HIDE);
	}
}

//////////////////////////////////////////////////////////////////////////
// Initialize dialog

BOOL CDlgSetParameter::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// initialize image rect
	m_ImageView.InitialRect();
	
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
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_pParam.m_bCorrect = m_bCorrect;
	EnableControl(m_bCorrect);
}

// Slider control message handler

void CDlgSetParameter::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
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

	UpdateData(false);
}

// Edit control message handler

void CDlgSetParameter::OnKillfocusRatioR() 
{
	// TODO: Add your control notification handler code here
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
	// TODO: Add your control notification handler code here
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
	// TODO: Add your control notification handler code here
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
	// TODO: Add extra cleanup here
	m_pParam = m_sParam;
	
	ShowDialog(FALSE);
}

void CDlgSetParameter::OnOK() 
{
	// TODO: Add extra validation here
	m_pParam.SaveParam(m_KeyPath);
	m_sParam = m_pParam;

	ShowDialog(FALSE);
}


void CDlgSetParameter::OnBalance() 
{
	// TODO: Add your control notification handler code here
	m_bBalance = TRUE;
}

//////////////////////////////////////////////////////////////////////////
// User message handler

LRESULT CDlgSetParameter::OnUpdateView(WPARAM wParam, LPARAM lParam)
{
	IMAGE * image = (IMAGE *)wParam;
	UpdatePreView(image->pDst, image->pSrc, image->nWid, image->nHei);
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
	return 0x00010000;
}

void CDlgSetParameter::Initialize(CCamera hCamera, CManager hManager)
{
	char buffer[100];
	int  nBuffSize;

	//init keypath
	m_KeyPath.Format("%s", hManager.pRegistryPath);
	
	// initial parameter
	HVGetDeviceInfo(hCamera.hDevice, DESC_DEVICE_TYPE, NULL, &nBuffSize);	
	HVGetDeviceInfo(hCamera.hDevice, DESC_DEVICE_TYPE, buffer, &nBuffSize);	
	m_pParam.m_CameraType = (HVTYPE)*buffer;
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
	g_nColor	 = m_pParam.m_bDoColor;
	//hj 2007.9.5
	g_hBmpInfo = GlobalAlloc(GPTR, sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD));
	g_pBmpInfo = (BITMAPINFO *)GlobalLock(g_hBmpInfo);
	
	g_pBmpInfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	g_pBmpInfo->bmiHeader.biPlanes		= 1;
	g_pBmpInfo->bmiHeader.biCompression = BI_RGB;

	for (int i = 0; i < 256; i++){
		g_pBmpInfo->bmiColors[i].rgbBlue		= (BYTE)i;
		g_pBmpInfo->bmiColors[i].rgbGreen		= (BYTE)i;
		g_pBmpInfo->bmiColors[i].rgbRed			= (BYTE)i;
		g_pBmpInfo->bmiColors[i].rgbReserved	= 0;
	}

	g_nBitDepth = m_pParam.m_info.ImageBitDepth;
	nDepth = m_pParam.m_info.ImageBitDepth;
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
	ShowDialog(m_Place.m_Visible);
	
}

void CDlgSetParameter::Uninitialize()
{
	DeleteMenu(m_hMenu, ID_COLOR_MENU, MF_BYCOMMAND);
	m_Place.SavePlacement(this, m_KeyPath);
	DestroyWindow();

	//hj 2007.9.19
	if (g_hBmpInfo != NULL) {
		GlobalUnlock(g_hBmpInfo);
		GlobalFree(g_hBmpInfo);
	}
	//end
}

bool CDlgSetParameter::OnUserCommand(unsigned int id)
{
	switch( id )
	{
	case ID_COLOR_MENU:
		ShowDialog(!IsWindowVisible());
		break;

	default:
		return false;
	}

	return true;
}

void CDlgSetParameter::OnManagerNotify(CImageInfo info)
{
}

void CDlgSetParameter::OnStartGrabbing(CImageInfo info)
{
}
	
void CDlgSetParameter::OnStopGrabbing(CImageInfo info, CImageData data)
{
}

void CDlgSetParameter::OnShowFrame(CImageInfo info, CImageData data)
{
	m_pParam.m_info = info;
	m_pParam.m_data = data;
	g_nBitDepth = info.ImageBitDepth;

	BYTE * pDst = (BYTE*)data.pImageBuffer;

	if ( IsWindowVisible() )
	{
		//IMAGE image = {data.pRawBuffer, pDst, info.ImageWidth, info.ImageHeight};
		//SendMessage(WM_SETPARAMDLG_UPDATE, (WPARAM)&image, NULL);
		UpdatePreView(pDst, data.pRawBuffer, info.ImageWidth, info.ImageHeight);
	}
	else
	{
	
	//	ConvertBayer2Rgb(pDst, data.pRawBuffer, info.ImageWidth, info.ImageHeight, &m_pParam);	
	}

	//if ( info.ImageBitDepth == 8 )
	//{
	//	ColorRender.Color2Gray((BYTE*)data.pImageBuffer, pDst, info.ImageWidth, info.ImageHeight);
	//}
}


void CDlgSetParameter::ConvertBayer2Rgb(void *pDst, void *pSrc, int nWid, int nHei, CParameter * pParam)
{

	//
	// [!BUG] In the previous version, i user m_bDoColor to check whether should do color/gray
	// conversion, it's wrong, because sometime the m_bDoColor not have the right value.
	//

	if (pParam->m_bDoColor) 
	{
	
		if (m_pParam.m_info.ImageBitDepth < 24 && m_nTempSize < m_pParam.m_info.ImageSizeBytes * 3)
		{
			delete	      m_pTempBuff;
			m_nTempSize = m_pParam.m_info.ImageSizeBytes * 3;
			m_pTempBuff = new BYTE[m_nTempSize];
		}

		BYTE * pDst = m_pParam.m_info.ImageBitDepth < 24 ? m_pTempBuff : (BYTE*)m_pParam.m_data.pImageBuffer;

		if ( pParam->m_bCorrect )
		{
			ColorRender.ConvertBayer2Rgb_Ex((BYTE*)pDst, (BYTE*)pSrc, nWid, nHei, 
				pParam->m_ConvertType, pParam->m_LutR, pParam->m_LutG, pParam->m_LutB, 		
				true, pParam->m_BayerLayout, pParam->m_Illuminant, pParam->m_CameraType);
		}
		else
		{
			ColorRender.PlxConvertBayer2Rgb((BYTE*)pDst, (BYTE*)pSrc, nWid, nHei,
				pParam->m_ConvertType, 	pParam->m_LutR, pParam->m_LutG, pParam->m_LutB,	
				true, pParam->m_BayerLayout);
		}

		if ( m_pParam.m_info.ImageBitDepth == 8 )
		{
			if( (nDepth != g_nBitDepth) ||(m_bDoColor != g_nColor) )
			{
				BOOL bEnable;
				HWND hWnd = NULL;
				
				bEnable = FALSE;
				if (hWnd != NULL) 
				{
					CHECK_WND(IDBALANCE);					
				}
			}
			ColorRender.Color2Gray((BYTE*)m_pParam.m_data.pImageBuffer, (BYTE*)pDst, m_pParam.m_info.ImageWidth, m_pParam.m_info.ImageHeight);
		}
		else
		{
			if( (nDepth != g_nBitDepth) ||(m_bDoColor != g_nColor))
			{
				BOOL bEnable;
				HWND hWnd = NULL;
				
				bEnable = TRUE;
				if (hWnd != NULL) 
				{
					CHECK_WND(IDBALANCE);					
				}
			}
		}
	}
	else
	{
		//
		// Do raw image
		//
		int i;
		BYTE* pDst1 = (BYTE*)pDst;
		BYTE* pSrc1 = (BYTE*)pSrc;

		if ( m_pParam.m_info.ImageBitDepth == 24 )
		{
			pDst1 += nWid * nHei * 3;
			for(i = 0; i<nHei; i++)
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
			for(i = 0; i<nHei; i++)
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

    CHECK_WND(IDC_BAYER_TYPE);
    CHECK_WND(IDC_GROUP_WHB);
	CHECK_WND(IDC_GROUP_GAMMA);
    CHECK_WND(IDC_GROUP_LIGHT);

    CHECK_WND(IDC_CHECK_CORRECT);
    CHECK_WND(IDBALANCE);
    CHECK_WND(IDC_STATIC_R);
    CHECK_WND(IDC_STATIC_B);
	CHECK_WND(IDC_STATIC_GAMMA);

	// Slider bar and label
	CHECK_WND(IDC_SLIDER_R);
	CHECK_WND(IDC_SLIDER_B);
	CHECK_WND(IDC_SLIDER_GAMA);
	CHECK_WND(IDC_STATIC_R_MIN);
	CHECK_WND(IDC_STATIC_R_MAX);
	CHECK_WND(IDC_STATIC_B_MIN);
	CHECK_WND(IDC_STATIC_B_MAX);
	CHECK_WND(IDC_STATIC_GAMA_MIN);
	CHECK_WND(IDC_STATIC_GAMA_MAX);
	// White balance edit box
	CHECK_WND(IDC_RATIO_R);
	CHECK_WND(IDC_RATIO_B);
	CHECK_WND(IDC_RATIO_GAMA);
    
    CHECK_WND(IDC_BAYER_GB);
    CHECK_WND(IDC_BAYER_GR);
    CHECK_WND(IDC_BAYER_BG);
    CHECK_WND(IDC_BAYER_RG);

    CHECK_WND(IDBALANCE);
    CHECK_WND(IDC_DEFAULT_WHB);
	CHECK_WND(IDC_DEFAULT_GAMMA);

    //
    // Only if bEnable is TRUE and m_bCorrect is TRUE then,
    // enable windows of FOUR CORRECT RATIO BUTTONS.
    //

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
	g_nColor = m_pParam.m_bDoColor;
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

