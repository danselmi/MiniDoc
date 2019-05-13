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
#include <configmanager.h>

#include <wx/intl.h>
#include <wx/string.h>

BEGIN_EVENT_TABLE(MiniDocPanel, wxPanel)
    EVT_MINI_STYLED_TEXT_CTRL_COMMAND(wxID_ANY, MiniDocPanel::OnMiniStcLineClick)
END_EVENT_TABLE()

MiniDocPanel::MiniDocPanel(wxWindow* parent,wxWindowID id)
{
    miniStc_ = NULL;
    microStc_ = NULL;
    wxBoxSizer* boxSizer;

    Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));

    boxSizer = new wxBoxSizer(wxHORIZONTAL);

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
    bool showMini = cfg->ReadBool(_T("/mini_doc/show_mini"), true);
    bool showMicro = cfg->ReadBool(_T("/mini_doc/show_micro"), true);
    // TODO (danselmi#1#): First improve the MiniStyledTextCtrl, later implement the MicroStyledTextCtrl
    showMicro = false;
    showMini = true;
    if(showMini)
    {
        miniStc_ = new MiniStyledTextCtrl(this, wxID_ANY);
        boxSizer->Add(miniStc_, 1, wxALL|wxEXPAND, 5);
    }

    if(showMicro)
    {
        microStc_ = new MicroStyledTextCtrl(this, wxID_ANY);
        boxSizer->Add(microStc_, showMini?0:1, wxALL|wxEXPAND, 5);
    }

    SetSizer(boxSizer);
    boxSizer->Fit(this);
    boxSizer->SetSizeHints(this);
}

MiniDocPanel::~MiniDocPanel()
{
}
void MiniDocPanel::UpdateConfig()
{
    if(miniStc_)
        miniStc_->UpdateConfig();
    if(microStc_)
        microStc_->UpdateConfig();
}
//void MiniDocPanel::UpdateMiniStc(cbStyledTextCtrl *stc)
void MiniDocPanel::UpdateMiniStc(EditorBase *eb)
{
    if(!eb)
        return;
    if(eb && eb->IsBuiltinEditor())
    {
        cbStyledTextCtrl *stc = static_cast<cbEditor *>(eb)->GetControl();

        if(miniStc_)
            miniStc_->UpdateMiniature(stc);
        if(microStc_)
            microStc_->UpdateMiniature(stc);
    }
}
void MiniDocPanel::ChangeMiniStcDoc(cbEditor *ed)
{
    if(ed)
    {
        if(miniStc_)
        {
            miniStc_->SetDocPointer(ed->GetControl()->GetDocPointer());
            EditorColourSet *ecs = Manager::Get()->GetEditorManager()->GetColourSet();
            const wxString lang = ed->GetLanguage();
            const bool isC = lang == "C/C++";
            ecs->Apply(lang, miniStc_, isC, true);

            miniStc_->UpdateMiniature(ed->GetControl());
        }
        if(microStc_)
            microStc_->UpdateMiniature(ed->GetControl());
    }
    else
    {
        if(miniStc_)
            miniStc_->SetDocPointer(NULL);
        if(microStc_)
            microStc_->AssociateNoDoc();
    }
}


void MiniDocPanel::ShowMiniatureOf(EditorBase *eb)
{
    wxString str;

    if(eb)
    {
        str = _T("active: ");
        str << eb->GetFilename();
        cbEditor *ed = dynamic_cast<cbEditor*>(eb);
        if(ed)
        {
            str << eb->GetFilename();
            ChangeMiniStcDoc(ed);
        }
        else
        {
            str << _T("don't know how to draw miniature of ");
            str << eb->GetFilename();
            ChangeMiniStcDoc(NULL);
        }
    }
    else
    {
        str = _T("No Doc open");
        ChangeMiniStcDoc(NULL);
    }

    //Manager::Get()->GetLogManager()->Log(_T("MiniDoc: ") + str);
}

void MiniDocPanel::OnMiniStcLineClick(MiniStyledTextCtrlLineClickedEvent &event)
{
    int line = event.GetLine();

    cbEditor *ed = dynamic_cast<cbEditor*>(Manager::Get()->GetEditorManager()->GetActiveEditor());
    if(ed)
    {
        cbStyledTextCtrl *stc = ed->GetControl();
        if (stc)
        {
            ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
            int k;
                /*k = 0; //no adjustment of position
                k = 3; //selected line 1/3 into screen
                k = 4; //selected line 1/4 into screen
                k = 2; //selected line on center
                k = 1; //selected line on top*/
            k = cfg->ReadInt(_T("mini_doc/pos_of_main"), 2);
            switch (k)
            {
            case 4:
            case 3:
            case 2:
                stc->ScrollToLine(line - stc->LinesOnScreen()/k);
                break;
            case 1:
                stc->ScrollToLine(line);
                break;
            case 0:
            default:
                break;
            }

        }
    }
}

