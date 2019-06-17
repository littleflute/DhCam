// Placement.cpp: implementation of the CPlacement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hvcolor.h"
#include "Placement.h"
#include "PropertyBag.h"
#include "Exception.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlacement::CPlacement()
{
	memset(&m_Placement, 0, sizeof(WINDOWPLACEMENT));
	m_Visible = FALSE;
}

CPlacement::~CPlacement()
{

}

//////////////////////////////////////////////////////////////////////////
//

void CPlacement::InitPlacement(CWnd * pWnd, CString keyPath)
{
	try
	{
		CPropertyBagPtr ptrBag = CRegistryPropertyBag::Open(keyPath);
		m_Placement.rcNormalPosition.left   = ptrBag->ReadLong("left");
		m_Placement.rcNormalPosition.top    = ptrBag->ReadLong("top");
        
        RECT ClientRect;
        pWnd->GetWindowRect(&ClientRect);
        m_Placement.rcNormalPosition.right = ClientRect.right - ClientRect.left +
            m_Placement.rcNormalPosition.left;
        m_Placement.rcNormalPosition.bottom = ClientRect.bottom - ClientRect.top +
            m_Placement.rcNormalPosition.top;
    
        // Because we cannot set windows width and height, so doesn't read
        // from register.
		//m_Placement.rcNormalPosition.right  = ptrBag->ReadLong("right");
		//m_Placement.rcNormalPosition.bottom = ptrBag->ReadLong("bottom");
		m_Visible = ptrBag->ReadBool("visible");
		pWnd->SetWindowPlacement(&m_Placement);
	}
	catch (CPropertyBagException)
	{
		memset(&m_Placement, 0, sizeof(WINDOWPLACEMENT));
		m_Visible = FALSE;
	}	
}


void CPlacement::SavePlacement(CWnd * pWnd, CString keyPath)
{
	pWnd->GetWindowPlacement(&m_Placement);
	
	try
	{
		CPropertyBagPtr ptrBag = CRegistryPropertyBag::Create(keyPath);
		ptrBag->WriteLong("left", m_Placement.rcNormalPosition.left);
		ptrBag->WriteLong("top", m_Placement.rcNormalPosition.top);
		ptrBag->WriteLong("right", m_Placement.rcNormalPosition.right);
		ptrBag->WriteLong("bottom", m_Placement.rcNormalPosition.bottom);
		ptrBag->WriteBool("visible", pWnd->IsWindowVisible());
	}
	catch (CPropertyBagException) {
	}
}
