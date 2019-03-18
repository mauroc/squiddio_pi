///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "squiddioPrefsDialogBase.h"

///////////////////////////////////////////////////////////////////////////

SquiddioPrefsDialogBase::SquiddioPrefsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_fgMainSizer = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgMainSizer->SetFlexibleDirection( wxBOTH );
	m_fgMainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	m_fgSubSizer = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSubSizer->SetFlexibleDirection( wxBOTH );
	m_fgSubSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_fgSubSizer->SetMinSize( wxSize( -1,380 ) );
	wxStaticBoxSizer* sbDestSizer;
	sbDestSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("View Destinations") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 3, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxAll = new wxCheckBox( sbDestSizer->GetStaticBox(), ID_CB_ALL, _("All"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxAll, 0, wxALL, 5 );

	m_checkBoxMarinas = new wxCheckBox( sbDestSizer->GetStaticBox(), wxID_ANY, _("Marinas"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxMarinas, 0, wxALL, 5 );

	m_checkBoxAnchorages = new wxCheckBox( sbDestSizer->GetStaticBox(), wxID_ANY, _("Anchorages"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxAnchorages, 0, wxALL, 5 );

	m_checkBoxYachtClubs = new wxCheckBox( sbDestSizer->GetStaticBox(), wxID_ANY, _("Yacht clubs"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxYachtClubs, 0, wxALL, 5 );

	m_checkBoxDocks = new wxCheckBox( sbDestSizer->GetStaticBox(), wxID_ANY, _("Docks/piers"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxDocks, 0, wxALL, 5 );

	m_checkBoxRamps = new wxCheckBox( sbDestSizer->GetStaticBox(), wxID_ANY, _("Boat ramps"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxRamps, 0, wxALL, 5 );

	m_checkBoxFuelStations = new wxCheckBox( sbDestSizer->GetStaticBox(), wxID_ANY, _("Fuel Stations"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxFuelStations, 0, wxALL, 5 );

	m_checkBoxOthers = new wxCheckBox( sbDestSizer->GetStaticBox(), wxID_ANY, _("Other"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxOthers, 0, wxALL, 5 );

	m_checkBoxAIS = new wxCheckBox( sbDestSizer->GetStaticBox(), wxID_ANY, _("AIS marker"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_checkBoxAIS, 0, wxALL, 5 );


	sbDestSizer->Add( fgSizer5, 1, wxEXPAND, 5 );


	m_fgSubSizer->Add( sbDestSizer, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbLogSizer;
	sbLogSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Share logs with friends") ), wxVERTICAL );

	wxFlexGridSizer* fgSizerLogs;
	fgSizerLogs = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerLogs->SetFlexibleDirection( wxBOTH );
	fgSizerLogs->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText6 = new wxStaticText( sbLogSizer->GetStaticBox(), wxID_ANY, _("Send"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizerLogs->Add( m_staticText6, 0, wxALL, 5 );

	wxString m_choiceHowOftenChoices[] = { _("Never"), _("Once a day"), _("Twice a day"), _("Once an hour"), _("Twice an hour"), _("Once a minute"), _("Twice a minute"), _("Every 10 seconds") };
	int m_choiceHowOftenNChoices = sizeof( m_choiceHowOftenChoices ) / sizeof( wxString );
	m_choiceHowOften = new wxChoice( sbLogSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceHowOftenNChoices, m_choiceHowOftenChoices, 0 );
	m_choiceHowOften->SetSelection( 0 );
	m_choiceHowOften->SetMinSize( wxSize( 250,-1 ) );

	fgSizerLogs->Add( m_choiceHowOften, 0, wxALL, 5 );

	m_staticText4 = new wxStaticText( sbLogSizer->GetStaticBox(), wxID_ANY, _("Receive"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizerLogs->Add( m_staticText4, 0, wxALL, 5 );

	wxString m_choiceReceiveChoices[] = { _("Never"), _("Once a day"), _("Twice a day"), _("Once an hour"), _("Twice an hour"), _("Once a minute"), _("Twice a minute"), _("Every 10 seconds") };
	int m_choiceReceiveNChoices = sizeof( m_choiceReceiveChoices ) / sizeof( wxString );
	m_choiceReceive = new wxChoice( sbLogSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceReceiveNChoices, m_choiceReceiveChoices, 0 );
	m_choiceReceive->SetSelection( 0 );
	m_choiceReceive->SetMinSize( wxSize( 250,-1 ) );

	fgSizerLogs->Add( m_choiceReceive, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( sbLogSizer->GetStaticBox(), wxID_ANY, _("sQuiddio ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizerLogs->Add( m_staticText7, 0, wxALL, 5 );

	m_textSquiddioID = new wxTextCtrl( sbLogSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textSquiddioID->Enable( false );
	m_textSquiddioID->SetToolTip( _("The email address you use to log into sQuidd.io") );

	fgSizerLogs->Add( m_textSquiddioID, 0, wxALL|wxEXPAND, 5 );

	m_staticText8 = new wxStaticText( sbLogSizer->GetStaticBox(), wxID_ANY, _("API Key"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizerLogs->Add( m_staticText8, 0, wxALL, 5 );

	m_textApiKey = new wxTextCtrl( sbLogSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textApiKey->Enable( false );
	m_textApiKey->SetToolTip( _("Retrieve your API Key from your sQuidd.io profile. Log into http://squidd.io and click Edit Profile in the Dashboard, then select the Numbers and Keys tab. Click Help for more information. \n\nIMPORTANT! Do not use your sQuiddio Password as your API Key! ") );

	fgSizerLogs->Add( m_textApiKey, 0, wxALL|wxEXPAND, 5 );


	sbLogSizer->Add( fgSizerLogs, 1, wxEXPAND, 5 );


	m_fgSubSizer->Add( sbLogSizer, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerRenderer;
	sbSizerRenderer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Renderer") ), wxVERTICAL );

	wxString m_radioBoxOCPNorODChoices[] = { _("OpenCPN Waypoint"), _("ODraw TextPoint") };
	int m_radioBoxOCPNorODNChoices = sizeof( m_radioBoxOCPNorODChoices ) / sizeof( wxString );
	m_radioBoxOCPNorOD = new wxRadioBox( sbSizerRenderer->GetStaticBox(), wxID_ANY, _("OCPN or ODraw"), wxDefaultPosition, wxDefaultSize, m_radioBoxOCPNorODNChoices, m_radioBoxOCPNorODChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxOCPNorOD->SetSelection( 0 );
	sbSizerRenderer->Add( m_radioBoxOCPNorOD, 0, wxALL, 5 );


	m_fgSubSizer->Add( sbSizerRenderer, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbODSettings;
	sbODSettings = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Ocpn Draw settings") ), wxVERTICAL );

	m_fgSizerODSettings = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerODSettings->AddGrowableCol( 0 );
	m_fgSizerODSettings->SetFlexibleDirection( wxBOTH );
	m_fgSizerODSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxShowName = new wxCheckBox( sbODSettings->GetStaticBox(), wxID_ANY, _("Show Name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxShowName->SetValue(true);
	m_fgSizerODSettings->Add( m_checkBoxShowName, 0, wxALL, 5 );


	m_fgSizerODSettings->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText5 = new wxStaticText( sbODSettings->GetStaticBox(), wxID_ANY, _("Text position relative to point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	m_fgSizerODSettings->Add( m_staticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePositionChoices[] = { _("Top"), _("Top Centre"), _("Bottom"), _("Bottom Centre"), _("Centre"), _("Right"), _("Left") };
	int m_choicePositionNChoices = sizeof( m_choicePositionChoices ) / sizeof( wxString );
	m_choicePosition = new wxChoice( sbODSettings->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePositionNChoices, m_choicePositionChoices, 0 );
	m_choicePosition->SetSelection( 0 );
	m_fgSizerODSettings->Add( m_choicePosition, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextTextColour = new wxStaticText( sbODSettings->GetStaticBox(), wxID_ANY, _("Text Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTextColour->Wrap( -1 );
	m_fgSizerODSettings->Add( m_staticTextTextColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerText = new wxColourPickerCtrl( sbODSettings->GetStaticBox(), wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_fgSizerODSettings->Add( m_colourPickerText, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextBackgroundColour = new wxStaticText( sbODSettings->GetStaticBox(), wxID_ANY, _("Baackground Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundColour->Wrap( -1 );
	m_fgSizerODSettings->Add( m_staticTextBackgroundColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerBackgroundColour = new wxColourPickerCtrl( sbODSettings->GetStaticBox(), wxID_ANY, wxColour( 255, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_fgSizerODSettings->Add( m_colourPickerBackgroundColour, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextBackgroundTransparency = new wxStaticText( sbODSettings->GetStaticBox(), wxID_ANY, _("Background Transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundTransparency->Wrap( -1 );
	m_fgSizerODSettings->Add( m_staticTextBackgroundTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderBackgroundTransparency = new wxSlider( sbODSettings->GetStaticBox(), wxID_ANY, 100, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_fgSizerODSettings->Add( m_sliderBackgroundTransparency, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerFontFace;
	bSizerFontFace = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextFont = new wxStaticText( sbODSettings->GetStaticBox(), wxID_ANY, _("Text Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFont->Wrap( -1 );
	bSizerFontFace->Add( m_staticTextFont, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextFontFaceExample = new wxStaticText( sbODSettings->GetStaticBox(), wxID_ANY, _("Example"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFontFaceExample->Wrap( -1 );
	bSizerFontFace->Add( m_staticTextFontFaceExample, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	m_fgSizerODSettings->Add( bSizerFontFace, 1, wxEXPAND, 5 );

	m_buttonTextFont = new wxButton( sbODSettings->GetStaticBox(), wxID_ANY, _("Fonts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fgSizerODSettings->Add( m_buttonTextFont, 0, wxALIGN_RIGHT|wxALL, 5 );


	sbODSettings->Add( m_fgSizerODSettings, 1, wxEXPAND, 5 );

	wxString m_radioBoxShowDisplayTextChoices[] = { _("Always"), _("On Rollover Only"), _("Never") };
	int m_radioBoxShowDisplayTextNChoices = sizeof( m_radioBoxShowDisplayTextChoices ) / sizeof( wxString );
	m_radioBoxShowDisplayText = new wxRadioBox( sbODSettings->GetStaticBox(), wxID_ANY, _("Show Display Text"), wxDefaultPosition, wxDefaultSize, m_radioBoxShowDisplayTextNChoices, m_radioBoxShowDisplayTextChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxShowDisplayText->SetSelection( 0 );
	sbODSettings->Add( m_radioBoxShowDisplayText, 0, wxALL, 5 );


	m_fgSubSizer->Add( sbODSettings, 0, wxALL|wxEXPAND, 5 );


	m_fgMainSizer->Add( m_fgSubSizer, 0, wxEXPAND, 5 );

	m_sdbButtonSizer = new wxStdDialogButtonSizer();
	m_sdbButtonSizerOK = new wxButton( this, wxID_OK );
	m_sdbButtonSizer->AddButton( m_sdbButtonSizerOK );
	m_sdbButtonSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbButtonSizer->AddButton( m_sdbButtonSizerCancel );
	m_sdbButtonSizerHelp = new wxButton( this, wxID_HELP );
	m_sdbButtonSizer->AddButton( m_sdbButtonSizerHelp );
	m_sdbButtonSizer->Realize();

	m_fgMainSizer->Add( m_sdbButtonSizer, 1, wxEXPAND|wxFIXED_MINSIZE, 5 );


	this->SetSizer( m_fgMainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_checkBoxAll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnCheckBoxAll ), NULL, this );
	m_choiceHowOften->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnShareChoice ), NULL, this );
	m_choiceReceive->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnShareChoice ), NULL, this );
	m_buttonTextFont->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnButtonClickFonts ), NULL, this );
	m_sdbButtonSizerCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::m_sdbButtonSizerOnCancelButtonClick ), NULL, this );
	m_sdbButtonSizerHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::LaunchHelpPage ), NULL, this );
	m_sdbButtonSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::m_sdbButtonSizerOnOKButtonClick ), NULL, this );
}

SquiddioPrefsDialogBase::~SquiddioPrefsDialogBase()
{
	// Disconnect Events
	m_checkBoxAll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnCheckBoxAll ), NULL, this );
	m_choiceHowOften->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnShareChoice ), NULL, this );
	m_choiceReceive->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnShareChoice ), NULL, this );
	m_buttonTextFont->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::OnButtonClickFonts ), NULL, this );
	m_sdbButtonSizerCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::m_sdbButtonSizerOnCancelButtonClick ), NULL, this );
	m_sdbButtonSizerHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::LaunchHelpPage ), NULL, this );
	m_sdbButtonSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SquiddioPrefsDialogBase::m_sdbButtonSizerOnOKButtonClick ), NULL, this );

}
