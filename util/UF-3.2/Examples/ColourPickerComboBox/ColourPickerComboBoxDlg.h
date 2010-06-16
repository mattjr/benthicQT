// Colour Picker Combo BoxDlg.h : header file
//

#pragma once

//#include "afxwin.h"
#include "ColourPickerCB.h"
#include "ColourEditCtrl.h"

// CColourPickerComboBoxDlg dialog
class CColourPickerComboBoxDlg : public CDialog
{
// Construction
public:
  CColourPickerComboBoxDlg(CWnd* pParent = NULL);  // standard constructor

  virtual ~CColourPickerComboBoxDlg();  // standard destructor


// Dialog Data
  enum { IDD = IDD_COLOURPICKERCOMBOBOX_DIALOG };

  protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support


// Implementation
protected:
  HICON m_hIcon;

  // Generated message map functions
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()
public:
  ColourPickerCB m_colourCB;
  UF::RGBColour::Colour<unsigned char> m_colour;
  afx_msg void OnCbnSelendokCombo1();
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  CEdit m_red;
  CEdit m_green;
  CEdit m_blue;
  CEdit m_alpha;
  void SetEditControls(UF::RGBColour::Colour<unsigned char> const & colour);
  CEdit m_hue1;
  CEdit m_lightness;
  CEdit m_saturation1;
  CEdit m_hue2;
  CEdit m_saturation2;
  CEdit m_value1;
  CEdit m_B;
  CEdit m_L;
  CEdit m_V;
  CEdit m_Y;
  CEdit m_Y709;
  ColourEditCtrl m_HTML;
  ColourEditCtrl m_HTMLc;
  UF::RGBColour::Colour_Map *cmp; // Primary colour map.
  UF::RGBColour::Colour_Map *cmf; // The full colour map.
  //! Matches a name to the colour value.
  /*!
      It requires both the primary and full colour map.
  */
  void MatchColour( UF::RGBColour::Colour<unsigned char> & colour );

};
