// ColourPickerComboBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ColourPickerComboBox.h"
#include "ColourPickerComboBoxDlg.h"
#include "PickAColour.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace UF;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

// Dialog Data
  enum { IDD = IDD_ABOUTBOX };

  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CColourPickerComboBoxDlg dialog



CColourPickerComboBoxDlg::CColourPickerComboBoxDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CColourPickerComboBoxDlg::IDD, pParent)
  , cmp(0)
  , cmf(0)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CColourPickerComboBoxDlg::~CColourPickerComboBoxDlg()
{
  if ( cmp )
    delete cmp;
  if ( cmf )
    delete cmf;

}

void CColourPickerComboBoxDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO1, m_colourCB);
  DDX_Control(pDX, IDC_EDIT1, m_red);
  DDX_Control(pDX, IDC_EDIT2, m_green);
  DDX_Control(pDX, IDC_EDIT3, m_blue);
  DDX_Control(pDX, IDC_EDIT4, m_alpha);
  DDX_Control(pDX, IDC_EDIT5, m_hue1);
  DDX_Control(pDX, IDC_EDIT6, m_lightness);
  DDX_Control(pDX, IDC_EDIT7, m_saturation1);
  DDX_Control(pDX, IDC_EDIT8, m_hue2);
  DDX_Control(pDX, IDC_EDIT9, m_saturation2);
  DDX_Control(pDX, IDC_EDIT10, m_value1);
  DDX_Control(pDX, IDC_EDIT11, m_B);
  DDX_Control(pDX, IDC_EDIT12, m_L);
  DDX_Control(pDX, IDC_EDIT13, m_V);
  DDX_Control(pDX, IDC_EDIT14, m_Y);
  DDX_Control(pDX, IDC_EDIT15, m_Y709);
  DDX_Control(pDX, IDC_EDIT16, m_HTML);
  DDX_Control(pDX, IDC_EDIT17, m_HTMLc);
}

BEGIN_MESSAGE_MAP(CColourPickerComboBoxDlg, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  //}}AFX_MSG_MAP
  ON_CBN_SELENDOK(IDC_COMBO1, OnCbnSelendokCombo1)
  ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CColourPickerComboBoxDlg message handlers

BOOL CColourPickerComboBoxDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);      // Set big icon
  SetIcon(m_hIcon, FALSE);    // Set small icon

  // TODO: Add extra initialization here
  this->cmp = new RGBColour::Colour_Map(true);
  this->cmf = new RGBColour::Colour_Map();

  RGBColour::TColour_Map_CItr p = cmf->colour_map.find("alice blue");
  if ( p != cmf->colour_map.end() )
  {
    this->m_colour.colour_name(p->first);
    this->m_colour.colour_value(p->second);
  }
  SetEditControls(this->m_colour);

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CColourPickerComboBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CColourPickerComboBoxDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CColourPickerComboBoxDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void CColourPickerComboBoxDlg::OnCbnSelendokCombo1()
{
  this->m_colourCB.SelectChangeColour();
  this->UpdateData(TRUE);
  this->m_colour = this->m_colourCB.Colour();
  SetEditControls(this->m_colour);
}

void CColourPickerComboBoxDlg::OnBnClickedButton1()
{
  CPickAColour dlg;

  dlg.m_colour = this->m_colour;

  if ( dlg.DoModal() == IDOK )
  {
      this->m_colour = dlg.m_colour;
      this->m_colourCB.Colour(this->m_colour);
      SetEditControls(this->m_colour);
  }
}

void CColourPickerComboBoxDlg::OnBnClickedButton2()
{
  CColorDialog dlg;

  unsigned char r, g, b, a;
  this->m_colour.get_colour_value(r,g,b,a);

  dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
  dlg.m_cc.rgbResult = RGB(r, g, b);
  if ( dlg.DoModal() == IDOK )
  {
    int rgb = dlg.m_cc.rgbResult;
    b = (unsigned char)((rgb & 0x00ff0000) >> 16);
    g = (unsigned char)((rgb & 0x0000ff00) >> 8);
    r = (unsigned char)(rgb & 0x000000ff);
    // Colour
    RGBColour::Colour<unsigned char> colour;
    colour.set_colour_value(r,g,b,255);
    MatchColour(colour);
    this->m_colour = colour;
    this->m_colourCB.Colour(this->m_colour);
    this->SetEditControls(this->m_colour);
  }
}

void CColourPickerComboBoxDlg::SetEditControls(RGBColour::Colour<unsigned char> const & colour)
{
  unsigned char r,g,b,a;
  colour.get_colour_value(r,g,b,a);
  CString txt;
  txt.Format("%u",r);
  this->m_red.SetWindowText(txt);
  txt.Format("%u",g);
  this->m_green.SetWindowText(txt);
  txt.Format("%u",b);
  this->m_blue.SetWindowText(txt);
  txt.Format("%u",a);
  this->m_alpha.SetWindowText(txt);

  double red, green, blue, alpha;
  unsigned char ur, ug, ub, ua;
  colour.get_colour_value(ur,ug,ub,ua);
  red = ur / 255.0;
  green = ug / 255.0;
  blue = ub / 255.0;
  alpha = ua / 255.0;

  double h,l,s,v;

  RGBColour::RGB_HLS()(red,green,blue,h,l,s);
  txt.Format("%lg",h);
  this->m_hue1.SetWindowText(txt);
  txt.Format("%lg",l);
  this->m_lightness.SetWindowText(txt);
  txt.Format("%lg",s);
  this->m_saturation1.SetWindowText(txt);

  RGBColour::RGB_HSV()(red,green,blue,h,s,v);
  txt.Format("%lg",h);
  this->m_hue2.SetWindowText(txt);
  txt.Format("%lg",s);
  this->m_saturation2.SetWindowText(txt);
  txt.Format("%lg",v);
  this->m_value1.SetWindowText(txt);

  txt.Format("%lg",RGBColour::B<double>()(red,green,blue));
  this->m_B.SetWindowText(txt);

  txt.Format("%lg",RGBColour::L<double>()(red,green,blue));
  this->m_L.SetWindowText(txt);

  txt.Format("%lg",RGBColour::V<double>()(red,green,blue));
  this->m_V.SetWindowText(txt);

  txt.Format("%lg",RGBColour::Y<double>()(red,green,blue));
  this->m_Y.SetWindowText(txt);

  txt.Format("%lg",RGBColour::Y709<double>()(red,green,blue));
  this->m_Y709.SetWindowText(txt);

  //this->m_HTML.SetWindowText(colour.colour_value().hex().c_str());
  //this->m_HTMLc.SetWindowText(colour.colour_value().hex_complement().c_str());

  COLORREF rgb_comp = RGB(255-colour.colour_value().red(),255-colour.colour_value().green(),255-colour.colour_value().blue());
  COLORREF rgb = RGB(colour.colour_value().red(),colour.colour_value().green(),colour.colour_value().blue());

  this->m_HTML.m_text = colour.colour_value().hex().c_str();
  this->m_HTML.m_txtColour = rgb_comp;
  this->m_HTML.m_bkColour = rgb;
  this->m_HTML.SetWindowTextA(this->m_HTML.m_text);

  this->m_HTMLc.m_text = colour.colour_value().hex_complement().c_str();
  this->m_HTMLc.m_txtColour = rgb;
  this->m_HTMLc.m_bkColour = rgb_comp;
  this->m_HTMLc.SetWindowTextA(this->m_HTMLc.m_text);
}

void CColourPickerComboBoxDlg::MatchColour( RGBColour::Colour<unsigned char> & colour )
{

  RGBColour::Colour<> c;
  RGBColour::ConvertColourType<unsigned char>()(colour,c,255.0);
  if (!cmp->match_value(c))
    if ( !cmf->match_value(c) )
      c.colour_name("User defined...");
  colour.colour_name(c.colour_name());
}
