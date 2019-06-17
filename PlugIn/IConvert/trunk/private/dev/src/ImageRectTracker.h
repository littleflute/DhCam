// ImageRectTracker.h: interface for the CImageRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGERECTTRACKER_H__259DE54C_4F20_4887_8DBF_0F6E2C833D19__INCLUDED_)
#define AFX_IMAGERECTTRACKER_H__259DE54C_4F20_4887_8DBF_0F6E2C833D19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImageRectTracker : public CRectTracker  
{
public:
	CImageRectTracker(CRect rect = CRect(0, 0, 10, 10), int nStyle = solidLine);
	virtual ~CImageRectTracker();
	void Draw(CDC* pDC) const;	

protected:
	void InitializeCursor();
};

#endif // !defined(AFX_IMAGERECTTRACKER_H__259DE54C_4F20_4887_8DBF_0F6E2C833D19__INCLUDED_)
