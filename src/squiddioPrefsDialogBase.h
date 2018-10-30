///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_CB_ALL 1000

///////////////////////////////////////////////////////////////////////////////
/// Class SquiddioPrefsDialogBase
///////////////////////////////////////////////////////////////////////////////
class SquiddioPrefsDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxStdDialogButtonSizer* m_sdbButtonSizer;
		wxButton* m_sdbButtonSizerOK;
		wxButton* m_sdbButtonSizerCancel;
		wxButton* m_sdbButtonSizerHelp;

		// Virtual event handlers, overide them in your derived class
		virtual void OnCheckBoxAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShareChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_sdbButtonSizerOnCancelButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void LaunchHelpPage( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_sdbButtonSizerOnOKButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxCheckBox* m_checkBoxAll;
		wxCheckBox* m_checkBoxMarinas;
		wxCheckBox* m_checkBoxAnchorages;
		wxCheckBox* m_checkBoxYachtClubs;
		wxCheckBox* m_checkBoxDocks;
		wxCheckBox* m_checkBoxRamps;
		wxCheckBox* m_checkBoxFuelStations;
		wxCheckBox* m_checkBoxOthers;
		wxCheckBox* m_checkBoxAIS;
		wxChoice* m_choiceHowOften;
		wxChoice* m_choiceReceive;
		wxTextCtrl* m_textSquiddioID;
		wxTextCtrl* m_textApiKey;
		wxRadioBox* m_radioBoxOCPNorOD;

		SquiddioPrefsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("sQuiddio Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 367,460 ), long style = wxDEFAULT_DIALOG_STYLE );
		~SquiddioPrefsDialogBase();

};

