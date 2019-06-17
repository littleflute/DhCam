// ImageRectTracker.cpp: implementation of the CImageRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hvcolor.h"
#include "ImageRectTracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//
extern HCURSOR	_afxCursors[10];
extern HBRUSH	_afxHatchBrush;
extern HPEN		_afxBlackDottedPen;
extern int		_afxHandleSize;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CImageRectTracker::CImageRectTracker(CRect rect, int nStyle) 
	 : CRectTracker(rect, nStyle)
{
	InitializeCursor();
}

CImageRectTracker::~CImageRectTracker()
{

}

//////////////////////////////////////////////////////////////////////////
//

void CImageRectTracker::InitializeCursor()
{
	HINSTANCE hInst = AfxGetResourceHandle();

	_afxCursors[0] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNWSE));
    _afxCursors[1] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNESW));
    _afxCursors[2] = _afxCursors[0];
    _afxCursors[3] = _afxCursors[1];
    _afxCursors[4] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNS));
    _afxCursors[5] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKWE));
    _afxCursors[6] = _afxCursors[4];
    _afxCursors[7] = _afxCursors[5];
    _afxCursors[8] = ::LoadCursor(NULL, IDC_ARROW);
    _afxCursors[9] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACK4WAY));
}

void CImageRectTracker::Draw(CDC* pDC) const
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	CGdiObject* pTemp;
	int nOldROP;

	// draw lines
	if ((m_nStyle & (dottedLine|solidLine)) != 0)
	{
		if (m_nStyle & dottedLine)
			pOldPen = pDC->SelectObject(CPen::FromHandle(_afxBlackDottedPen));
		else
			pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nOldROP = pDC->SetROP2(R2_COPYPEN);
		rect.InflateRect(+1, +1);   // borders are one pixel outside
		pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// if hatchBrush is going to be used, need to unrealize it
	if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
		UnrealizeObject(_afxHatchBrush);

	// hatch inside
	if ((m_nStyle & hatchInside) != 0)
	{
		pTemp = pDC->SelectStockObject(NULL_PEN);
		if (pOldPen == NULL)
			pOldPen = (CPen*)pTemp;
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(TRANSPARENT);
		nOldROP = pDC->SetROP2(R2_MASKNOTPEN);
		pDC->Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// draw hatched border
	if ((m_nStyle & hatchedBorder) != 0)
	{
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;
		pDC->SetBkMode(OPAQUE);
		CRect rectTrue;
		GetTrueRect(&rectTrue);
		pDC->PatBlt(rectTrue.left, rectTrue.top, rectTrue.Width(),
			rect.top-rectTrue.top, PATINVERT    /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.bottom,
			rectTrue.Width(), rectTrue.bottom-rect.bottom, PATINVERT    /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.top, rect.left-rectTrue.left,
			rect.Height(), PATINVERT    /* Pn */);
		pDC->PatBlt(rect.right, rect.top, rectTrue.right-rect.right,
			rect.Height(), PATINVERT    /* Pn */);
	}

	// draw resize handles
	if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
	{
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				GetHandleRect((TrackerHit)i, &rect);
//				pDC->FillSolidRect(rect, RGB(0, 0, 0));
			}
		}
	}

	// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	VERIFY(pDC->RestoreDC(-1));
}