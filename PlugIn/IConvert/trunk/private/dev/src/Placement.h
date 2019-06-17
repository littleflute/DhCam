// Placement.h: interface for the CPlacement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLACEMENT_H__94BAA870_36E4_442C_829E_1E47442F00B5__INCLUDED_)
#define AFX_PLACEMENT_H__94BAA870_36E4_442C_829E_1E47442F00B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlacement  
{
public:
	CPlacement();
	virtual ~CPlacement();

public:
	WINDOWPLACEMENT m_Placement;
	BOOL            m_Visible;

public:
	void InitPlacement(CWnd *, CString);
	void SavePlacement(CWnd *, CString);

};

#endif // !defined(AFX_PLACEMENT_H__94BAA870_36E4_442C_829E_1E47442F00B5__INCLUDED_)
