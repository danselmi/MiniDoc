/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MINI_DOC_PANEL_H
#define MINI_DOC_PANEL_H

#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "MiniStyledTextCtrl.h"
#include "MiniStyledTextCtrlEvent.h"

class EditorBase;
class cbEditor;

class MiniDocPanel: public wxPanel
{
    public:
        MiniDocPanel(wxWindow* parent,wxWindowID id=wxID_ANY);
        virtual ~MiniDocPanel(){}

        void ShowMiniatureOf(cbEditor *ed);
        void Update(cbEditor *ed);
        void UpdateConfig();

    protected:
        void OnMiniStcLineClick(MiniStyledTextCtrlLineClickedEvent &event);

        MiniStyledTextCtrl *miniStc_;

    private:
        DECLARE_EVENT_TABLE()
};

#endif
