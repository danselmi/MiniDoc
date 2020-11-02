#ifndef MINI_STYLED_TEXT_CTRL_EVENT_H
#define MINI_STYLED_TEXT_CTRL_EVENT_H

#include <wx/event.h>

extern const wxEventType MiniStyledTextCtrlCommandEvent;

class MiniStyledTextCtrlLineClickedEvent: public wxCommandEvent
{
public:
	explicit MiniStyledTextCtrlLineClickedEvent(wxEventType commandType = MiniStyledTextCtrlCommandEvent, int id = 0):
        wxCommandEvent(commandType, id),
        line(0)
    {}

	// You *must* copy here the data to be transported
	explicit MiniStyledTextCtrlLineClickedEvent(const MiniStyledTextCtrlLineClickedEvent &event):
        wxCommandEvent(event)
    {
        this->SetLine( event.GetLine() );
    }

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



#endif
