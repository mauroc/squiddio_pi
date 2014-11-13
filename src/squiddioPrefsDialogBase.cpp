///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  6 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "squiddioPrefsDialogBase.h"

///////////////////////////////////////////////////////////////////////////

SquiddioPrefsDialogBase::SquiddioPrefsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgMainSizer;
	fgMainSizer = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgMainSizer->SetFlexibleDirection( wxBOTH );
	fgMainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSubSizer;
	fgSubSizer = new wxFlexGridSizer( 3, 1, 20, 0 );
	fgSubSizer->SetFlexibleDirection( wxBOTH );
	fgSubSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbDestSizer;
	sbDestSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("View Destinations") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 3, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_checkBoxAll = new wxCheckBox( this, ID_CB_ALL, wxT("All"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxAll->SetValue(true); 
	m_checkBoxAll->Enable( false );
	
	fgSizer5->Add( m_checkBoxAll, 0, wxALL, 5 );
	
	m_checkBoxMarinas = new wxCheckBox( this, wxID_ANY, wxT("Marinas"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxMarinas, 0, wxALL, 5 );
	
	m_checkBoxAnchorages = new wxCheckBox( this, wxID_ANY, wxT("Anchorages"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxAnchorages, 0, wxALL, 5 );
	
	m_checkBoxYachtClubs = new wxCheckBox( this, wxID_ANY, wxT("Yacht clubs"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxYachtClubs, 0, wxALL, 5 );
	
	m_checkBoxDocks = new wxCheckBox( this, wxID_ANY, wxT("Docks/piers"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxDocks, 0, wxALL, 5 );
	
	m_checkBoxRamps = new wxCheckBox( this, wxID_ANY, wxT("Boat ramps"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxRamps, 0, wxALL, 5 );
	
	m_checkBoxFuelStations = new wxCheckBox( this, wxID_ANY, wxT("Fuel Stations"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxFuelStations, 0, wxALL, 5 );
	
	m_checkBoxOthers = new wxCheckBox( this, wxID_ANY, wxT("Other"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxOthers, 0, wxALL, 5 );
	
	
	sbDestSizer->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	
	fgSubSizer->Add( sbDestSizer, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbLogSizer;
	sbLogSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Share logs with friends") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizerLogs;
	fgSizerLogs = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerLogs->SetFlexibleDirection( wxBOTH );
	fgSizerLogs->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Send"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizerLogs->Add( m_staticText6, 0, wxALL, 5 );
	
	wxString m_choiceHowOftenChoices[] = { wxT("Never"), wxT("Once a day"), wxT("Twice a day"), wxT("Once an hour"), wxT("Twice an hour"), wxT("Once a minute"), wxT("Twice a minute"), wxT("Every 10 seconds") };
	int m_choiceHowOftenNChoices = sizeof( m_choiceHowOftenChoices ) / sizeof( wxString );
	m_choiceHowOften = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceHowOftenNChoices, m_choiceHowOftenChoices, 0 );
	m_choiceHowOften->SetSelection( 0 );
	m_choiceHowOften->SetMinSize( wxSize( 250,-1 ) );
	
	fgSizerLogs->Add( m_choiceHowOften, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Receive"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizerLogs->Add( m_staticText4, 0, wxALL, 5 );
	
	wxString m_choiceReceiveChoices[] = { wxT("Never"), wxT("Once a day"), wxT("Twice a day"), wxT("Once an hour"), wxT("Twice an hour"), wxT("Once a minute"), wxT("Twice a minute"), wxT("Every 10 seconds") };
	int m_choiceReceiveNChoices = sizeof( m_choiceReceiveChoices ) / sizeof( wxString );
	m_choiceReceive = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceReceiveNChoices, m_choiceReceiveChoices, 0 );
	m_choiceReceive->SetSelection( 0 );
	m_choiceReceive->SetMinSize( wxSize( 250,-1 ) );
	
	fgSizerLogs->Add( m_choiceReceive, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("sQuiddio ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizerLogs->Add( m_staticText7, 0, wxALL, 5 );
	
	m_textSquiddioID = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerLogs->Add( m_textSquiddioID, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("API Key"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizerLogs->Add( m_staticText8, 0, wxALL, 5 );
	
	m_textApiKey = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerLogs->Add( m_textApiKey, 0, wxALL|wxEXPAND, 5 );
	
	
	sbLogSizer->Add( fgSizerLogs, 1, wxEXPAND, 5 );
	
	
	fgSubSizer->Add( sbLogSizer, 1, wxEXPAND, 5 );
	
	m_sdbButtonSizer = new wxStdDialogButtonSizer();
	m_sdbButtonSizerOK = new wxButton( this, wxID_OK );
	m_sdbButtonSizer->AddButton( m_sdbButtonSizerOK );
	m_sdbButtonSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbButtonSizer->AddButton( m_sdbButtonSizerCancel );
	m_sdbButtonSizerHelp = new wxButton( this, wxID_HELP );
	m_sdbButtonSizer->AddButton( m_sdbButtonSizerHelp );
	m_sdbButtonSizer->Realize();
	
	fgSubSizer->Add( m_sdbButtonSizer, 1, wxEXPAND, 5 );
	
	
	fgMainSizer->Add( fgSubSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgMainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_checkBoxAll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnCheckBoxAll ), NULL, this );
	m_sdbButtonSizerHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::LaunchHelpPage ), NULL, this );
}

SquiddioPrefsDialogBase::~SquiddioPrefsDialogBase()
{
	// Disconnect Events
	m_checkBoxAll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnCheckBoxAll ), NULL, this );
	m_sdbButtonSizerHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::LaunchHelpPage ), NULL, this );
	
}
