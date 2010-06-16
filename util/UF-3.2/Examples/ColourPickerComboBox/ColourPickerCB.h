#pragma once

#include "ufColour.h"

// ColourPickerCB
    //! A colour picker combo box.
    /*!
    CLASS
      ColourPickerCB

      This class provides the interface for a combo box that allows picking of colours by name.
      It also allows the user to specify a user defined colour and use a colour picker
      dialog to specify a colour by its colour patch or by its RGB or HSV value.
      To use it, just replace the CComboBox variable type with ColourPickerCB
      and ensure that the combobox that is subclassed has the properties outlined below.



    USAGE

      In the dialog box, create a CComboBox.

      Ensure that the combobox that is subclassed has the following styles:

      - Owner Draw: Fixed
      - Has Strings: true
      - Sort: false
      - Type: Drop List

      Add a variable of type CComboBox using the Add Member Variable Wizard and call it,
      for example, m_colourCB.

      In the declaration for the dialog class, do the following:
      Include this header:( #include "ColourPickerCB.h" ) and change
      the type from CComboBox to ColourPickerCB in the header file
      of the dialog:

      Change:
      - CComboBox m_colourCB;

      To:
      - RGBColour::ColourPickerCB m_colourCB;


      Add in a variable to hold the colour.
      - RGBColour::Colour<unsigned int> colour;

      What you have should be something like this:
      \code
        public:
          RGBColour::ColourPickerCB m_colourCB;
          RGBColour::Colour<unsigned char> m_colour;
        \endcode


      <p>In the implementation of the class, do the following.</p>
      In the DoDataExchange() procedure of the Dialog make sure that
      DDX control for the colour picker is called.
      \code
        void CVtkDLGDlg::DoDataExchange(CDataExchange* pDX)
        {
          ...
          DDX_Control(pDX, IDC_COMBO1, m_colourCB);
          m_colourCB.DDX_Control(pDX, IDC_COMBO1, m_colour);
        }
        \endcode

      Use the Event Handler Wizard and override the Windoes Message handler for
          CBN_SELENDOK

      In the Dialog, override the Windows Message Handler for CBN_SELENDOK to call
      SelectChangeColour() for the variable and then UpdateData(TRUE). This ensures
      that DDX_ColourPickerCB will update the value of the variable (  m_bkgCol in this case).
        \code
          void CColourPickerComboBoxDlg::OnCbnSelendokCombo1()
          {
            this->m_colourCB.SelectChangeColour();
            this->UpdateData(TRUE);
          }
        \endcode

      To call the dialog containing this combo box, do something like this ( in this example
      a button is pressed to invoke a dialog containing the control):
          \code
            void CColourPickerComboBoxDlg::OnBnClickedButton1()
            {
              CPickAColour dlg;

              dlg.m_colour = this->m_colour;

              if ( dlg.DoModal() == IDOK )
              {
                this->m_colour = dlg.m_colour;
                SetEditControls(this->m_colour);
                this->UpdateData(FALSE);
              }
            }
          \endcode


    \par license
    This software is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    YOU, as the USER, take SOLE RESPONSIBILITY for, and are responsible for
    determining the FITNESS and APPLICABILITY of this software for a particular
    purpose. The author or any other person or entity are not liable in any way
    for the use or misuse of any program or software using this software.

    \todo

    \bug

    */

    class ColourPickerCB : public CComboBox
    {
      DECLARE_DYNAMIC(ColourPickerCB)

    public:
      ColourPickerCB();
      virtual ~ColourPickerCB();

    //! Exchange data between the control and a variable.
    /*!
    * Perform a data exchange.
    *
    * @param *pDX : the pointer to the data exchange structure
    * @param iIDC : the ID of the control
    * @param &colour : The colour
    *
    * @return void  :
    */
      void ColourPickerCB::DDX_Control(CDataExchange *pDX, int iIDC, UF::RGBColour::Colour<unsigned char> &colour);
      void ColourPickerCB::DDX_Control(CDataExchange *pDX, int iIDC, UF::RGBColour::Colour<double,double> &colour);

    protected:
      DECLARE_MESSAGE_MAP()
    private:

      //! Load the combo box with colours.
      /*!
      * Load the combo box with the colours. A user defined colour is also added.
      *
      * @return void  :
      */
      void init(void);

      // Add a colour to the combo box.
      /*!
      * Add the colours to the display list for the control.
      *
      * @param name : then name of the colour
      * @param colour : the colour
      *
      * @return void  :
      */
      void AddColour( UF::RGBColour::Colour<unsigned char> const & colour );

      //! A name for the user defined colour name.
      std::string m_UserDefinedColourName;

      //! A map of colour names and values
      UF::RGBColour::Colour_Map m_colourMap;

    public:
    protected:
      virtual void DoDataExchange(CDataExchange* pDX);
      virtual void PreSubclassWindow();
    public:

      virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
      //! Select or change a colour.
      /*!
      * If the user selects the user defined colour, invoke a colour picker dialog
      * and update the user defined colour with the new colour. If the colour
      * selected matches an existing colour in the map, then the existing
      * colour will be selected. Note: <i>If the colour value exists in the map,
      * it will select the first colour that matches the value. e.g If the colour
      * value matches yellow green then olive drab 3 will be selected because it has
      * the same value as yellow green and it preceeds yellow green in the ordering
      * specified in the map of colours.</i>
      *
      *
      * @return void  :
      */
      void SelectChangeColour(void);

      //! Select a colour in the display list of the control.
      /*!
      * Select a colour in the display list of the control.
      *
      * @param &colourName : the name of the colour.
      *
      * @return void  :
      */
      void SelectColour(std::string const & colourName);

      //! Return the colour selected.
      /*!
      *
      * @return RGBColour::Colour<unsigned char>  : the colour.
      */
      UF::RGBColour::Colour<unsigned char> Colour ( void ) const ;

      //! Select a colour by name or value.
      /*!
      * - If the colour is not a user defined colour, find the colour by name
      * and if it exists, select it. <b> The value of the parameter <i>colour</i>
      * will be changed to reflect the existing value in the map of colours.</b>
      *
      * - Otherwise, find the colour by value, and if it exists, select it.
      * <b> The name of the parameter <i>colour</i> will be changed to reflect the
      * existing name in the map of colours.</b> <i>If the colour value exists in the map,
      * it will select the first colour that matches the value. e.g If the colour
      * value matches "yellow green" then "olive drab 3" will be selected because it has
      * the same value as "yellow green" and it preceeds "yellow green" in the ordering
      * specified in the map of colours.</i>
      *
      * - If the above two cases are not satisfied, then search for it by name and either
      * insert it as a new colour or change the red, green, and blue values of an existing
      * colour of the same name. Generally this means that a colour corresponding to the
      * user defined name is either entered or updated. Note, that in this case, even if
      * the user defined colour exists, it is <b>not</b> used to update <i>colour</i>.
      *
      * This mechanism ensures that the pre-defined colours in the map remain read-only
      * and also allows the user defined colour to be read-write.
      *
      * @param &colour : the colour, consisting of a name and its red, green and blue values.
      *
      * @return void  :
      */
      void Colour ( UF::RGBColour::Colour<unsigned char> const & colour );
    };


