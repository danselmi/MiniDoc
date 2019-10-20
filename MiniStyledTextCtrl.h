#ifndef MINI_STYLED_TEXT_CTRL_H
#define MINI_STYLED_TEXT_CTRL_H

#include "cbstyledtextctrl.h"

class ConfigManager;

class MiniStyledTextCtrl: public cbStyledTextCtrl
{
    public:
        MiniStyledTextCtrl(wxWindow* pParent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
        virtual ~MiniStyledTextCtrl(){}

        void UpdateMiniature(cbStyledTextCtrl *stc);
        void UpdateConfig();
        void Init();
    protected:
    private:
        void SetMarker();
        void DesignateVisibleRange(int from, int to);
        void MakePositionVisible(int fromLine, int length);

        void OnKey(wxKeyEvent& event);
        void OnMousWheel(wxMouseEvent& event);
        void OnMouseUpOrDown(wxMouseEvent& event);
        void OnMouseDClick(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseEnterOrLeave(wxMouseEvent& event);

        int GetLineFromPosition(const wxPoint &pt);

        int visibleFrom_, visibleLength_;

        const int GetOurMarkerNumber()const;
    private:
        wxColor backgroundColour_;
        bool showDesignator_;
        bool inverseMarker_;
        bool doScrollToPosition_;
    private:
        DECLARE_EVENT_TABLE();
};

#endif
