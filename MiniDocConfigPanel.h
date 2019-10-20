#ifndef MINIDOCCONFIGPANEL_H
#define MINIDOCCONFIGPANEL_H

//(*Headers(MiniDocConfigPanel)
#include <wx/panel.h>
class wxButton;
class wxCheckBox;
class wxChoice;
class wxStaticText;
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
		wxCheckBox* ShowScrollbar;
		wxCheckBox* inverseDesignatorCheckBox;
		wxCheckBox* syncPositionCheckBox;
		wxChoice* adjustPositionChoice;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		//*)

        /// @return the panel's title.
        virtual wxString GetTitle() const;
        /// @return the panel's bitmap base name. You must supply two bitmaps: \<basename\>.png and \<basename\>-off.png...
        virtual wxString GetBitmapBaseName() const;
        /// Called when the user chooses to apply the configuration.
        virtual void OnApply();
        /// Called when the user chooses to cancel the configuration.
        virtual void OnCancel(){}


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
