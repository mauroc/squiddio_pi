#include "SquiddioPrefsDialog.h"

SquiddioPrefsDialog::SquiddioPrefsDialog( wxWindow* parent )
:
SquiddioPrefsDialogBase( parent )
{

}

void SquiddioPrefsDialog::OnCheckBoxAll( wxCommandEvent& event )
{
// TODO: Implement onAllSelect
::wxBell();
/*
wxCheckBox *checkbox = (wxCheckBox*)event.GetEventObject();
if (checkbox.IsChecked())
m_checkBoxMarinas->Enable(false);
m_checkBoxAnchorages->Enable(false);
m_checkBoxYachtClubs->Enable(false);
m_checkBoxDocks->Enable(false);
m_checkBoxRamps->Enable(false);
m_checkBoxOthers->Enable(false);
else
m_checkBoxMarinas->Enable(true);
m_checkBoxAnchorages->Enable(true);
m_checkBoxYachtClubs->Enable(true);
m_checkBoxDocks->Enable(true);
m_checkBoxRamps->Enable(true);
m_checkBoxOthers->Enable(true);
*/
}

void SquiddioPrefsDialog::LaunchHelpPage( wxCommandEvent& event )
{
// TODO: Implement launchBrowser
    if (!wxLaunchDefaultBrowser(_T("http://squidd.io/squiddio_pi")))
        wxMessageBox(_("Could not launch default browser. Check your Internet connection"));
}
