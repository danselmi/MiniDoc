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
#include <wx/radiobut.h>
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
	CheckBox1 = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX1"));
	CheckBox2 = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX2"));
	CheckBox3 = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX3"));
	RadioButton1 = (wxRadioButton*)FindWindow(XRCID("ID_RADIOBUTTON1"));
	RadioButton2 = (wxRadioButton*)FindWindow(XRCID("ID_RADIOBUTTON2"));
	RadioButton3 = (wxRadioButton*)FindWindow(XRCID("ID_RADIOBUTTON3"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));
	Choice1 = (wxChoice*)FindWindow(XRCID("ID_CHOICE1"));

	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MiniDocConfigPanel::OnColorSelButtonClick);
	//*)

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
    if(!cfg)
        return;


    wxColour backgroundColour = Manager::Get()->GetColourManager()->GetColour(wxT("minidoc_background"));
    ColorSelButton->SetBackgroundColour(backgroundColour);

	CheckBox1->SetValue(cfg->ReadBool(_T("/mini_doc/sync_to_main_doc"), true));
	CheckBox2->SetValue(cfg->ReadBool(_T("/mini_doc/inverse_designator"), false));
	CheckBox3->SetValue(cfg->ReadBool(_T("/mini_doc/show_vertical_scrollbar"), true));
	Choice1->SetSelection(cfg->ReadInt(_T("/mini_doc/pos_of_main"), 2));

    bool showMini = cfg->ReadBool(_T("/mini_doc/show_mini"), true);
    bool showMicro = cfg->ReadBool(_T("/mini_doc/show_micro"), true);

    RadioButton1->SetValue(showMini && !showMicro);
    RadioButton2->SetValue(!showMini && showMicro);
    RadioButton3->SetValue(showMini && showMicro);

    RadioButton1->Hide();
    RadioButton2->Hide();
    RadioButton3->Hide();

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


    cfg->Write(_T("/mini_doc/sync_to_main_doc"), CheckBox1->GetValue());
    cfg->Write(_T("/mini_doc/inverse_designator"), CheckBox2->GetValue());
    cfg->Write(_T("/mini_doc/show_vertical_scrollbar"), CheckBox3->GetValue());
    cfg->Write(_T("/mini_doc/pos_of_main"), Choice1->GetSelection());

    panel_->UpdateConfig();

    //takes place after restart:
    bool showMini = RadioButton1->GetValue() || RadioButton3->GetValue();
    cfg->Write(_T("/mini_doc/show_mini"), showMini);
    bool showMicro =  RadioButton2->GetValue() || RadioButton3->GetValue();
    cfg->Write(_T("/mini_doc/show_micro"), showMicro);

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

