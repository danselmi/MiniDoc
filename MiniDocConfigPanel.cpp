#include "MiniDocConfigPanel.h"

#include <sdk.h>

#ifndef CB_PRECOMP
    #include <algorithm>
    #include <wx/xrc/xmlres.h>
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/colordlg.h>
    #include <wx/stattext.h>
    #include <wx/spinctrl.h>

    #include <configmanager.h>
    #include <editormanager.h>
    #include <logmanager.h>
#endif

#include "cbcolourmanager.h"
#include "MiniDocPanel.h"

//(*InternalHeaders(MiniDocConfigPanel)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(MiniDocConfigPanel)
//*)

BEGIN_EVENT_TABLE(MiniDocConfigPanel,wxPanel)
	//(*EventTable(MiniDocConfigPanel)
	//*)
END_EVENT_TABLE()

MiniDocConfigPanel::MiniDocConfigPanel(wxWindow* parent, MiniDocPanel *panel)
{
    panel_ = panel;
	BuildContent(parent);
}

void MiniDocConfigPanel::BuildContent(wxWindow* parent)
{
	//(*Initialize(MiniDocConfigPanel)
	wxXmlResource::Get()->LoadObject(this,parent,_T("MiniDocConfigPanel"),_T("wxPanel"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	ColorSelButton = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	syncPositionCheckBox = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX_SYNC_POS"));
	showDesignatorCheckBox = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX_SHOW_DESIGNATOR"));
	inverseDesignatorCheckBox = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX_INVERSE_DES"));
	showScrollbarCheckBox = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX_SHOW_SCROLLBAR"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	adjustPositionChoice = (wxChoice*)FindWindow(XRCID("ID_CHOICE1"));

	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MiniDocConfigPanel::OnColorSelButtonClick);
	Connect(XRCID("ID_CHECKBOX_SHOW_DESIGNATOR"),wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MiniDocConfigPanel::OnShowDesignator);
	//*)

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
    if(!cfg) return;

    wxColour backgroundColour = Manager::Get()->GetColourManager()->GetColour(wxT("minidoc_background"));
    ColorSelButton->SetBackgroundColour(backgroundColour);

    const bool enableDesignator = cfg->ReadBool(_T("/mini_doc/show_designator"), false);
	syncPositionCheckBox->SetValue(cfg->ReadBool(_T("/mini_doc/sync_to_main_doc"), true));
	showDesignatorCheckBox->SetValue(enableDesignator);
	inverseDesignatorCheckBox->SetValue(cfg->ReadBool(_T("/mini_doc/inverse_designator"), false));
	inverseDesignatorCheckBox->Enable(enableDesignator);
	showScrollbarCheckBox->SetValue(cfg->ReadBool(_T("/mini_doc/show_vertical_scrollbar"), true));
	adjustPositionChoice->SetSelection(cfg->ReadInt(_T("/mini_doc/pos_of_main"), 2));
}

MiniDocConfigPanel::~MiniDocConfigPanel()
{
	//(*Destroy(MiniDocConfigPanel)
	//*)
}

void MiniDocConfigPanel::OnColorSelButtonClick(wxCommandEvent& event)
{
    wxColourData data;
    wxWindow* sender = FindWindowById(event.GetId());
    data.SetColour(sender->GetBackgroundColour());

    wxColourDialog dlg(this, &data);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        wxColour colour = dlg.GetColourData().GetColour();
        sender->SetBackgroundColour(colour);
        sender->SetLabel(wxEmptyString);
    }
}

void MiniDocConfigPanel::OnApply()
{
    // save any changes
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));

    wxColour backgroundColour = XRCCTRL(*this, "ID_BUTTON1", wxButton)->GetBackgroundColour();
    Manager::Get()->GetColourManager()->SetColour(wxT("minidoc_background"), backgroundColour);

    cfg->Write(_T("/mini_doc/sync_to_main_doc"), syncPositionCheckBox->GetValue());
    cfg->Write(_T("/mini_doc/show_designator"), showDesignatorCheckBox->GetValue());
    cfg->Write(_T("/mini_doc/inverse_designator"), inverseDesignatorCheckBox->GetValue());
    cfg->Write(_T("/mini_doc/show_vertical_scrollbar"), showScrollbarCheckBox->GetValue());
    cfg->Write(_T("/mini_doc/pos_of_main"), adjustPositionChoice->GetSelection());

    panel_->UpdateConfig();
}

wxString MiniDocConfigPanel::GetTitle() const
{
    return _("MiniDoc");
}

wxString MiniDocConfigPanel::GetBitmapBaseName() const
{
    return _T("MiniDoc");
}

void MiniDocConfigPanel::OnShowDesignator(wxCommandEvent& event)
{
    inverseDesignatorCheckBox->Enable(showDesignatorCheckBox->GetValue());
}
