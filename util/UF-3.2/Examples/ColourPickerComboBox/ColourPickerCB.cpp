// ColourPickerCB.cpp : implementation file
//

#include "stdafx.h"
#include "ColourPickerCB.h"

using namespace UF::RGBColour;

IMPLEMENT_DYNAMIC(ColourPickerCB, CComboBox)
ColourPickerCB::ColourPickerCB() :
  m_UserDefinedColourName("User defined...")
{
}

ColourPickerCB::~ColourPickerCB()
{
}


BEGIN_MESSAGE_MAP(ColourPickerCB, CComboBox)
END_MESSAGE_MAP()



// CColourPickerCB message handlers


void ColourPickerCB::init(void)
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a = 255;
  UF::RGBColour::Colour<unsigned char> c;
  for(TColour_Map_CItr p = m_colourMap.colour_map.begin(); p != m_colourMap.colour_map.end(); ++p)
  {
    r = p->second.red();
    g = p->second.green();
    b = p->second.blue();
    c.set_colour(p->first.c_str(),r,g,b,a);
    AddColour(c);
  }

  // Add an item on the end to allow defining of a new colour
  // not in the named list of colours.
  // We will allow the user to change this colour.
  r = g = b = a = 255;
  c.set_colour(m_UserDefinedColourName,r,g,b,a);
  AddColour(c);
  m_colourMap.colour_map[c.colour_name()] = c.colour_value();
  // Select the first colour in the list.
  SetCurSel( 0 );
}

void ColourPickerCB::AddColour( UF::RGBColour::Colour<unsigned char> const & colour )
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
  colour.get_colour_value(r,g,b,a);
  int idx = AddString( colour.colour_name().c_str() );
  if( idx != CB_ERR && idx != CB_ERRSPACE )
    SetItemData( idx, RGB(r,g,b) );
}


void ColourPickerCB::DoDataExchange(CDataExchange* pDX)
{
  // TODO: Add your specialized code here and/or call the base class

  CComboBox::DoDataExchange(pDX);
}

void ColourPickerCB::DDX_Control(CDataExchange *pDX, int iIDC, UF::RGBColour::Colour<unsigned char> &colour)
{
  HWND hWndCtrl = pDX->PrepareCtrl( iIDC );
  ASSERT( hWndCtrl );

  // Get the actual control
  ColourPickerCB* pPicker = (ColourPickerCB*)CWnd::FromHandle( hWndCtrl );
  ASSERT( pPicker );

  if( pDX->m_bSaveAndValidate )
    colour = pPicker->Colour();
  else
  //{
  //  if ( !colour.colour_name().empty() )
    pPicker->Colour( colour );
  //}
}


void ColourPickerCB::DDX_Control(CDataExchange *pDX, int iIDC, UF::RGBColour::Colour<double,double> &colour)
{
  HWND hWndCtrl = pDX->PrepareCtrl( iIDC );
  ASSERT( hWndCtrl );

  // Get the actual control
  ColourPickerCB* pPicker = (ColourPickerCB*)CWnd::FromHandle( hWndCtrl );
  ASSERT( pPicker );

  if( pDX->m_bSaveAndValidate )
    ConvertColourType<unsigned char,unsigned char,double,double>()(pPicker->Colour(),colour,1.0/255.0);
    //colour = pPicker->Colour();
  else
  //{
  //  if ( !colour.colour_name().empty() )
  {
    UF::RGBColour::Colour<unsigned char> c;
    ConvertColourType<double,double,unsigned char,unsigned char>()(colour,c,255.0);
    pPicker->Colour( c );
  }
}


void ColourPickerCB::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  CString strColour;
  CDC dcContext;
  CRect rItemRect( lpDrawItemStruct->rcItem );
  CRect rBlockRect( rItemRect );
  CRect rTextRect( rBlockRect );
  CBrush brFrameBrush;
  int iFourthWidth = 0;
  int iItem = lpDrawItemStruct->itemID;
//  int iAction = lpDrawItemStruct->itemAction;
  int iState = lpDrawItemStruct->itemState;
  COLORREF crColour = NULL;
  COLORREF crNormal = GetSysColor( COLOR_WINDOW );
  COLORREF crSelected = GetSysColor( COLOR_HIGHLIGHT );
  COLORREF crText = GetSysColor( COLOR_WINDOWTEXT );

  if( !dcContext.Attach( lpDrawItemStruct->hDC ) )
    return;

  iFourthWidth = ( rBlockRect.Width() / 4 );
  brFrameBrush.CreateStockObject( BLACK_BRUSH );

  if( iState & ODS_SELECTED )
  {
    dcContext.SetTextColor(  ( 0x00FFFFFF & ~( crText ) ) );
    dcContext.SetBkColor( crSelected );
    dcContext.FillSolidRect( &rBlockRect, crSelected );
  }
  else
  {
    dcContext.SetTextColor( crText );
    dcContext.SetBkColor( crNormal );
    dcContext.FillSolidRect( &rBlockRect, crNormal );
  }

  if( iState & ODS_FOCUS )
    dcContext.DrawFocusRect( &rItemRect );

  // calculate text area.
  rTextRect.left += ( iFourthWidth + 2 );
  rTextRect.top += 2;

  // calculate colour block area.
  rBlockRect.DeflateRect( CSize( 2, 2 ) );
  rBlockRect.right = iFourthWidth;

  // draw colour text and block.
  if( iItem != -1 )
  {
    GetLBText( iItem, strColour );

    if( iState & ODS_DISABLED )
    {
      crColour = GetSysColor( COLOR_INACTIVECAPTIONTEXT );
      dcContext.SetTextColor( crColour );
    }
    else
      crColour = (COLORREF)GetItemData( iItem );

    dcContext.SetBkMode( TRANSPARENT );
    dcContext.TextOut( rTextRect.left, rTextRect.top,  strColour );
    dcContext.FillSolidRect( &rBlockRect, crColour );
    dcContext.FrameRect( &rBlockRect, &brFrameBrush );
  }

  dcContext.Detach();
}

void ColourPickerCB::SelectChangeColour(void)
{
  CString colourName;
  GetLBText( GetCurSel(), colourName );

  if( colourName == m_UserDefinedColourName.c_str() )
  {
    CColorDialog dlg;

    if( dlg.DoModal() == IDOK )
    {
      // Set the user defined colour patch.
      SetItemData( GetCurSel(), dlg.GetColor() );

      // Look for the colour in the colour map.
      COLORREF cr = dlg.GetColor();
      colour_base_<unsigned char> cv(GetRValue( cr ), GetGValue( cr ), GetBValue( cr ));
      UF::RGBColour::Colour<unsigned char,unsigned char> colour(m_UserDefinedColourName,cv);
      if ( this->m_colourMap.match_value(colour) )
      {
        SelectColour(colour.colour_name());
        return;
      }
      this->m_colourMap.change(colour);

      // Find it
      int idx = FindStringExact( -1, m_UserDefinedColourName.c_str() );
      if ( idx != CB_ERR)
      {
        // Update the colour patch
        unsigned char r = cv.red();
        unsigned char g = cv.green();
        unsigned char b = cv.blue();
        SetItemData( idx, RGB(r,g,b) );
        // Select it
        SetCurSel(idx);
      }
      return;
    }
  }
}

void ColourPickerCB::SelectColour(std::string const & colourName)
{
   int idx = FindStringExact( -1, colourName.c_str() );
  if ( idx != CB_ERR)
    SetCurSel(idx);
}


Colour<unsigned char> ColourPickerCB::Colour ( void ) const
{
  UF::RGBColour::Colour<unsigned char> colour;
  CString name;

  int idx = GetCurSel();
  if ( idx != CB_ERR )
  {
    GetLBText(idx,name);
    TColour_Map_CItr p = this->m_colourMap.colour_map.find( (LPCSTR) name);
    if ( p != this->m_colourMap.colour_map.end() )
    {
      colour.colour_name(p->first);
      colour.colour_value(p->second);
    }
  }

  return colour;
}

void ColourPickerCB::Colour ( UF::RGBColour::Colour<unsigned char> const & colour )
{
  // Find the colour by name (if it is not user defined) and, if it exists, select it.
  if ( colour.colour_name() != m_UserDefinedColourName )
  {
    TColour_Map_CItr p = this->m_colourMap.colour_map.find( colour.colour_name() );
    if ( p != this->m_colourMap.colour_map.end() )
    {
      SelectColour(colour.colour_name());
      return;
    }
  }

  // If a user defined colour or the colour value already exists, match on value.
  UF::RGBColour::Colour<unsigned char> c(colour);
  if ( this->m_colourMap.match_value(c) )
  {
    SelectColour(c.colour_name());
    return;
  }

  // We have a totally new colour so insert it.
  this->m_colourMap.colour_map[colour.colour_name()] = colour.colour_value();

  // Find it
  int idx = FindStringExact( -1, colour.colour_name().c_str() );
  if ( idx != CB_ERR)
  {
    // Update the colour patch
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    colour.get_colour_value(r,g,b,a);
    SetItemData( idx, RGB(r,g,b) );
    // Select it
    SetCurSel(idx);
  }

}

void ColourPickerCB::PreSubclassWindow()
{
  init();
  CComboBox::PreSubclassWindow();
}

