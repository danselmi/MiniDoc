#ifndef MICRO_STYLED_TEXT_CTRL_H
#define MICRO_STYLED_TEXT_CTRL_H

#include "cbstyledtextctrl.h"
#include "wx/panel.h"
#include "vector"

class MicroStyledTextCtrl: public wxPanel
{
    public:
        MicroStyledTextCtrl(wxWindow* pParent, int id);
        virtual ~MicroStyledTextCtrl();

        void UpdateMiniature(cbStyledTextCtrl *stc);
        void UpdateConfig();
        void AssociateNoDoc();
    private:
        void Init();

        void OnMouseDown(wxMouseEvent& event);
        void OnPaint(wxPaintEvent &evt);
        void OnEraseBackground(wxEraseEvent &evt);
        int GetLineFromPosition(const wxPoint &pt);
        void GetVisibles(cbStyledTextCtrl *stc);
        void GetvisibleIndicators(cbStyledTextCtrl *stc, int theIndicator, const wxColor &color);

        struct visible
        {
            int line;
            char colStart, colEnd;
            wxColor color;
        };
        int linesInDoc_;
        int getHeight();
        std::vector<visible> visibles;
        wxColor bgColor;
        int firstVisibleLine;
        int lastVisibleLine;
    private:
        bool inverseDesignator_;
        wxColor markColor_;
        bool highlightOccurrencesEnabled_;
    private:
        DECLARE_EVENT_TABLE();
};



#endif
