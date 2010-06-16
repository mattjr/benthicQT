// ColourEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ColourPickerComboBox.h"
#include "ColourEditCtrl.h"


// ColourEditCtrl

IMPLEMENT_DYNAMIC(ColourEditCtrl, CEdit)

ColourEditCtrl::ColourEditCtrl()
{
   this->m_bkColour = RGB(255,0,0);
   this->m_txtColour = RGB(0,255,255);
   this->m_Brush.CreateSolidBrush(this->m_bkColour);
   this->m_text="FIIK";
}

ColourEditCtrl::~ColourEditCtrl()
{
  this->m_Brush.DeleteObject();
}


BEGIN_MESSAGE_MAP(ColourEditCtrl, CEdit)
  ON_WM_PAINT()
  ON_WM_SETFOCUS()
  ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// ColourEditCtrl message handlers



BOOL ColourEditCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
  if ( message != WM_CTLCOLOREDIT )
  {
    return CEdit::OnChildNotify(message, wParam, lParam, pLResult);
  }
  // We are only handling the WM_CTLCOLOREDIT message.
  HDC hdcChild = (HDC)wParam;
  SetTextColor(hdcChild,this->m_txtColour);
  SetBkColor(hdcChild,this->m_bkColour);

  return CEdit::OnChildNotify(message, wParam, lParam, pLResult);
}

void ColourEditCtrl::OnPaint()
{
  CPaintDC dc(this); // device context for painting
  // TODO: Add your message handler code here
  // Do not call CEdit::OnPaint() for painting messages
  this->GetWindowText(this->m_text);
  this->SetBkGrndColour();
}

void ColourEditCtrl::SetBkGrndColour()
{
  this->m_Brush.DeleteObject();
  this->m_Brush.CreateSolidBrush(this->m_bkColour);
  CDC* pDC = GetDC();
  pDC->SetBkMode(OPAQUE);
  pDC->SetBkColor(this->m_bkColour);
  pDC->SelectObject(&this->m_Brush);
  CRect rc;
  GetClientRect(&rc);
  ScreenToClient(&rc);
  pDC->Rectangle(0,0,rc.Width(),rc.Height());
  pDC->SetTextColor(this->m_txtColour);
  pDC->TextOut(2,2,this->m_text.GetBuffer(m_text.GetLength()));
}
void ColourEditCtrl::OnSetFocus(CWnd* pOldWnd)
{
  CEdit::OnSetFocus(pOldWnd);

  SetBkGrndColour();
}

void ColourEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
  CEdit::OnKillFocus(pNewWnd);

  this->GetWindowText(this->m_text);

  SetBkGrndColour();
}
