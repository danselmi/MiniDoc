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

#include <wx/intl.h>
#include <wx/string.h>

BEGIN_EVENT_TABLE(MiniDocPanel, wxPanel)
    EVT_MINI_STYLED_TEXT_CTRL_COMMAND(wxID_ANY, MiniDocPanel::OnMiniStcLineClick)
END_EVENT_TABLE()

MiniDocPanel::MiniDocPanel(wxWindow* parent,wxWindowID id)
{
    currentEb_ = NULL;
    wxBoxSizer* boxSizer;

    Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));

    boxSizer = new wxBoxSizer(wxVERTICAL);
    miniStc_ = new MiniStyledTextCtrl(this, wxID_ANY);
    boxSizer->Add(miniStc_, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    SetSizer(boxSizer);
    boxSizer->Fit(this);
    boxSizer->SetSizeHints(this);

}

MiniDocPanel::~MiniDocPanel()
{
}

void MiniDocPanel::UpdateMiniStc(EditorBase *eb, bool force)
{
    if(!eb)
        return;

    cbEditor *editor = dynamic_cast<cbEditor *>(eb);
    if(!editor)
        return;
    cbStyledTextCtrl *stc = editor->GetControl();
    if(!stc)
        return;
    miniStc_->UpdateMiniature(stc, force);
}
void MiniDocPanel::ChangeMiniStcDoc(cbEditor *ed)
{
    if(ed)
    {
        miniStc_->SetDocPointer(ed->GetControl()->GetDocPointer());
        EditorColourSet *ecs = Manager::Get()->GetEditorManager()->GetColourSet();
        ecs->Apply(ed->GetLanguage(), miniStc_);
        miniStc_->UpdateMiniature(ed->GetControl(),true);
    }
    else
    {
        miniStc_->SetDocPointer(NULL);
    }
}


void MiniDocPanel::ShowMiniatureOf(EditorBase *eb)
{
    wxString str;

    currentEb_ = eb;
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

    if ( currentEb_ )
    {
        cbEditor *ed = dynamic_cast<cbEditor*>(currentEb_);
        if(ed)
        {
            cbStyledTextCtrl *stc = ed->GetControl();
            if (stc)
                stc->ScrollToLine(line);
        }
    }

}
