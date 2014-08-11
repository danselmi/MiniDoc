#ifndef MINIDOCCONFIGPANEL_H
#define MINIDOCCONFIGPANEL_H

//(*Headers(MiniDocConfigPanel)
#include <wx/panel.h>
class wxCheckBox;
class wxStaticText;
class wxButton;
//*)

#include <configurationpanel.h>

class MiniDocPanel;

class MiniDocConfigPanel: public cbConfigurationPanel
{
	public:

		MiniDocConfigPanel(wxWindow* parent, MiniDocPanel *panel);
		virtual ~MiniDocConfigPanel();

		//(*Declarations(MiniDocConfigPanel)
		wxButton* ColorSelButton;
		wxCheckBox* CheckBox2;
		wxStaticText* StaticText1;
		wxCheckBox* CheckBox1;
		//*)

        /// @return the panel's title.
        virtual wxString GetTitle() const;
        /// @return the panel's bitmap base name. You must supply two bitmaps: \<basename\>.png and \<basename\>-off.png...
        virtual wxString GetBitmapBaseName() const;
        /// Called when the user chooses to apply the configuration.
        virtual void OnApply();
        /// Called when the user chooses to cancel the configuration.
        virtual void OnCancel();


	protected:

		//(*Identifiers(MiniDocConfigPanel)
		//*)

	private:
        MiniDocPanel *panel_;
		//(*Handlers(MiniDocConfigPanel)
		void OnColorSelButtonClick(wxCommandEvent& event);
		//*)

	protected:

		void BuildContent(wxWindow* parent);

		DECLARE_EVENT_TABLE()
};

#endif
