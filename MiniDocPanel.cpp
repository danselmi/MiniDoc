/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 9264 $
 * $Id: MiniDocPanel.cpp 9264 2013-08-17 09:35:46Z mortenmacfly $
 * $HeadURL: $
 */

#include "MiniDocPanel.h"

#include <sdk.h> // Code::Blocks SDK
#include <cbcolourmanager.h>
#include <cbeditor.h>
#include <editormanager.h>
#include <editorcolourset.h>
#include <configmanager.h>

#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/string.h>

BEGIN_EVENT_TABLE(MiniDocPanel, wxPanel)
    EVT_MINI_STYLED_TEXT_CTRL_COMMAND(wxID_ANY, MiniDocPanel::OnMiniStcLineClick)
END_EVENT_TABLE()

MiniDocPanel::MiniDocPanel(wxWindow* parent, wxWindowID id):
    wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id")),
    miniStc_(new MiniStyledTextCtrl(this, wxID_ANY))
{
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    miniStc_->StyleSetBackground(wxSCI_STYLE_DEFAULT, wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    wxBoxSizer* boxSizer = new wxBoxSizer(wxHORIZONTAL);
    boxSizer->Add(miniStc_, 1, wxALL|wxEXPAND, 5);
    SetSizer(boxSizer);
    boxSizer->Fit(this);
    boxSizer->SetSizeHints(this);
}

void MiniDocPanel::UpdateConfig()
{
    if(miniStc_)
    {
        miniStc_->UpdateConfig();
        miniStc_->StyleSetBackground(wxSCI_STYLE_DEFAULT, wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    }

    EditorManager *edMan = Manager::Get()->GetEditorManager();
    if(!edMan) return;
    if(!Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/mini_doc/show_designator"), false))
    {
        for(size_t i = 0 ; i < edMan->GetEditorsCount() ; ++i)
        {
            cbEditor *ed = edMan->GetBuiltinEditor(i);
            if(!ed) continue;

            MiniStyledTextCtrl::RemoveMarkers(ed->GetControl());
        }
    }
}

void MiniDocPanel::Update(cbEditor *ed)
{
    if(ed && miniStc_)
    {
        if(cbStyledTextCtrl *stc = ed->GetControl())
            miniStc_->UpdateMiniature(stc);
    }
}

void MiniDocPanel::ShowMiniatureOf(cbEditor *ed)
{
    if(!miniStc_)
        return;

    if(ed)
    {
        miniStc_->SetDocPointer(ed->GetControl()->GetDocPointer());
        EditorColourSet *ecs = Manager::Get()->GetEditorManager()->GetColourSet();
        const wxString lang = ed->GetLanguage();
        const bool isC = lang == "C/C++";
        ecs->Apply(lang, miniStc_, isC, true);

        miniStc_->UpdateMiniature(ed->GetControl());
    }
    else
    {
        miniStc_->SetDocPointer(NULL);
        miniStc_->StyleSetBackground(wxSCI_STYLE_DEFAULT, wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    }
}

void MiniDocPanel::OnMiniStcLineClick(MiniStyledTextCtrlLineClickedEvent &event)
{
    int line = event.GetLine();

    cbEditor *ed = dynamic_cast<cbEditor*>(Manager::Get()->GetEditorManager()->GetActiveEditor());
    if(!ed) return;

    if(cbStyledTextCtrl *stc = ed->GetControl())
    {
        ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
        int k;
            /*k = 0; //no adjustment of position
            k = 3; //selected line 1/3 into screen
            k = 4; //selected line 1/4 into screen
            k = 2; //selected line on center
            k = 1; //selected line on top*/
        k = cfg->ReadInt(_T("mini_doc/pos_of_main"), 2);
        if(k>1 && k<5)
            stc->ScrollToLine(line - stc->LinesOnScreen()/k);
        else if(k==1)
            stc->ScrollToLine(line);
    }
}

