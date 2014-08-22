#ifndef MINI_STYLED_TEXT_CTRL_H
#define MINI_STYLED_TEXT_CTRL_H

#include "cbstyledtextctrl.h"

class MiniStyledTextCtrl: public cbStyledTextCtrl
{
    public:
        MiniStyledTextCtrl(wxWindow* pParent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
        virtual ~MiniStyledTextCtrl();

        void UpdateMiniature(cbStyledTextCtrl *stc, bool force = false);
        void UpdateConfig();
        void Init();
        void PrepareMainView(cbStyledTextCtrl *stc);
    protected:
    private:
        void SetMarker();
        void SetVisibleRange(int from, int to, bool force = false);
        void MakeVisible(int fromLine, int length);

        void SyncFoldState(cbStyledTextCtrl *stc);

        void OnKey(wxKeyEvent& event);
        void OnMousWheel(wxMouseEvent& event);
        void OnMouseUpOrDown(wxMouseEvent& event);
        void OnMouseDClick(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseEnterOrLeave(wxMouseEvent& event);

        int GetLineFromPosition(const wxPoint &pt);

        int visibleFrom, visibleLength;

        const int GetOurMarkerNumber()const;
    private:
        DECLARE_EVENT_TABLE();
};

#endif // MINI_STYLED_TEXT_CTRL_H
