#ifndef MINI_STYLED_TEXT_CTRL_H
#define MINI_STYLED_TEXT_CTRL_H

#include "cbstyledtextctrl.h"
#include <wx/overlay.h>


class MiniStyledTextCtrl: public cbStyledTextCtrl
{
    public:
        MiniStyledTextCtrl(wxWindow* pParent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
        virtual ~MiniStyledTextCtrl();

        void UpdateMiniature(cbStyledTextCtrl *stc, bool force = false);

    protected:
    private:
        void SetMarker();
        void SetVisibleRange(int from, int to, bool force = false);
        void MakeVisible(int fromLine, int length);

        void SyncFoldState(cbStyledTextCtrl *stc);

        void OnKey(wxKeyEvent& event);
        void OnMousWheel(wxMouseEvent& event);
        void OnMouseUpOrDown(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseEnterOrLeave(wxMouseEvent& event);

        int GetLineFromPosition(const wxPoint &pt);

        int visibleFrom, visibleLength;

        const int GetOurMarkerNumber()const;
    private:
        DECLARE_EVENT_TABLE();
};

extern const wxEventType MiniStyledTextCtrlCommandEvent;

// A custom event that transports a whole wxString.
class MiniStyledTextCtrlLineClickedEvent: public wxCommandEvent
{
public:
	MiniStyledTextCtrlLineClickedEvent( wxEventType commandType = MiniStyledTextCtrlCommandEvent, int id = 0 )
	:  wxCommandEvent(commandType, id) { }

	// You *must* copy here the data to be transported
	MiniStyledTextCtrlLineClickedEvent( const MiniStyledTextCtrlLineClickedEvent &event )
	:  wxCommandEvent(event) { this->SetLine( event.GetLine() ); }

	// Required for sending with wxPostEvent()
	wxEvent* Clone() const { return new MiniStyledTextCtrlLineClickedEvent(*this); }

	int GetLine() const { return line; }
	void SetLine( const int& li ) { line = li; }

private:
	int line;
};

typedef void (wxEvtHandler::*MiniStyledTextCtrlLineClickedEventFunction)(MiniStyledTextCtrlLineClickedEvent &);



// This #define simplifies the one below, and makes the syntax less
// ugly if you want to use Connect() instead of an event table.
#define MiniStyledTextCtrlLineClickedEventHandler(func) \
	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) \
	wxStaticCastEvent(MiniStyledTextCtrlLineClickedEventFunction, &fn)

// Define the event table entry. Yes, it really *does* end in a comma.
#define EVT_MINI_STYLED_TEXT_CTRL_COMMAND(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY( MiniStyledTextCtrlCommandEvent, id, wxID_ANY, \
	(wxObjectEventFunction)(wxEventFunction) \
	(wxCommandEventFunction) wxStaticCastEvent( \
	MiniStyledTextCtrlLineClickedEventFunction, &fn ), (wxObject*) NULL ),

// Define the event table entry. Yes, it really *does* end in a comma.
#define EVT_MINI_STYLED_TEXT_CTRL_COMMAND_RANGE(id1, id2, fn) \
	DECLARE_EVENT_TABLE_ENTRY( MiniStyledTextCtrlCommandEvent, id1, id2, \
	(wxObjectEventFunction)(wxEventFunction) \
	(wxCommandEventFunction) wxStaticCastEvent( \
	MiniStyledTextCtrlLineClickedEventFunction, &fn ), (wxObject*) NULL ),




#endif // MINI_STYLED_TEXT_CTRL_H
