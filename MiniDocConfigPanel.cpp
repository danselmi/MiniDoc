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
#include <wx/xrc/xmlres.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/button.h>
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
	CheckBox1 = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX1"));
	CheckBox2 = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX2"));

	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MiniDocConfigPanel::OnColorSelButtonClick);
	//*)

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
    if(!cfg)
        return;


    wxColour backgroundColour = Manager::Get()->GetColourManager()->GetColour(wxT("minidoc_background"));
    XRCCTRL(*this, "ID_BUTTON1", wxButton)->SetBackgroundColour(backgroundColour);

	XRCCTRL(*this, "ID_CHECKBOX1", wxCheckBox)->SetValue(cfg->ReadBool(_T("/mini_doc/sync_to_main_doc"), true));
	XRCCTRL(*this, "ID_CHECKBOX2", wxCheckBox)->SetValue(cfg->ReadBool(_T("/mini_doc/inverse_designator"), false));
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


    cfg->Write(_T("/mini_doc/sync_to_main_doc"), XRCCTRL(*this, "ID_CHECKBOX1", wxCheckBox)->GetValue());
    cfg->Write(_T("/mini_doc/inverse_designator"), XRCCTRL(*this, "ID_CHECKBOX2", wxCheckBox)->GetValue());
    panel_->UpdateMiniStcBackground();
}

void MiniDocConfigPanel::OnCancel()
{
}

wxString MiniDocConfigPanel::GetTitle() const
{
    return _("MiniDoc");
}
wxString MiniDocConfigPanel::GetBitmapBaseName() const
{
    return _T("MiniDoc");
}

