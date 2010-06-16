#pragma once


// ColourEditCtrl

class ColourEditCtrl : public CEdit
{
	DECLARE_DYNAMIC(ColourEditCtrl)

public:
	ColourEditCtrl();
	virtual ~ColourEditCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
public:
  afx_msg void OnPaint();

  void SetBkGrndColour();

  COLORREF m_bkColour;
  COLORREF m_txtColour;
  CString m_text;

protected:
  CBrush m_Brush;
public:
  afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
  afx_msg void OnKillFocus(CWnd* pNewWnd);
};


