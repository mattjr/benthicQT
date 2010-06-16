#pragma once

#include "ColourPickerCB.h"
#include "afxwin.h"

// CPickAColour dialog

class CPickAColour : public CDialog
{
  DECLARE_DYNAMIC(CPickAColour)

public:
  CPickAColour(CWnd* pParent = NULL);   // standard constructor
  virtual ~CPickAColour();

// Dialog Data
  enum { IDD = IDD_COLOUR_PICKER };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  ColourPickerCB m_colourCB;
  UF::RGBColour::Colour<unsigned char> m_colour;
  afx_msg void OnCbnSelendokCombo1();
  virtual BOOL OnInitDialog();
};
