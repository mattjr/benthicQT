// PickAColour.cpp : implementation file
//

#include "stdafx.h"
#include "ColourPickerComboBox.h"
#include "PickAColour.h"

using namespace UF;

// CPickAColour dialog

IMPLEMENT_DYNAMIC(CPickAColour, CDialog)
CPickAColour::CPickAColour(CWnd* pParent /*=NULL*/)
  : CDialog(CPickAColour::IDD, pParent)
{
}

CPickAColour::~CPickAColour()
{
}

void CPickAColour::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO1, m_colourCB);
}


BEGIN_MESSAGE_MAP(CPickAColour, CDialog)
  ON_CBN_SELENDOK(IDC_COMBO1, OnCbnSelendokCombo1)
END_MESSAGE_MAP()


// CPickAColour message handlers

void CPickAColour::OnCbnSelendokCombo1()
{
  this->m_colourCB.SelectChangeColour();
  this->UpdateData(TRUE);
  this->m_colour = this->m_colourCB.Colour();
}

BOOL CPickAColour::OnInitDialog()
{
  CDialog::OnInitDialog();

  this->m_colourCB.Colour(this->m_colour);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
