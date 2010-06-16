// Colour Picker Combo Box.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"    // main symbols


// CColourPickerComboBoxApp:
// See Colour Picker Combo Box.cpp for the implementation of this class
//

class CColourPickerComboBoxApp : public CWinApp
{
public:
  CColourPickerComboBoxApp();

// Overrides
  public:
  virtual BOOL InitInstance();

// Implementation

  DECLARE_MESSAGE_MAP()
};

extern CColourPickerComboBoxApp theApp;
