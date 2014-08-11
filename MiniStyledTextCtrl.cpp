#include "MiniStyledTextCtrl.h"

#include <wx/event.h>
#include "configmanager.h"
#include "cbcolourmanager.h"

const wxEventType MiniStyledTextCtrlCommandEvent = wxNewEventType();

BEGIN_EVENT_TABLE(MiniStyledTextCtrl, cbStyledTextCtrl)
    EVT_KEY_DOWN        (MiniStyledTextCtrl::OnKey)
    EVT_KEY_UP          (MiniStyledTextCtrl::OnKey)

    EVT_MOUSEWHEEL      (MiniStyledTextCtrl::OnMousWheel)
    EVT_ENTER_WINDOW    (MiniStyledTextCtrl::OnMouseEnterOrLeave)
    EVT_LEAVE_WINDOW    (MiniStyledTextCtrl::OnMouseEnterOrLeave)
    EVT_LEFT_DOWN       (MiniStyledTextCtrl::OnMouseUpOrDown)
    EVT_MIDDLE_DOWN     (MiniStyledTextCtrl::OnMouseUpOrDown)
    EVT_RIGHT_DOWN      (MiniStyledTextCtrl::OnMouseUpOrDown)
    EVT_LEFT_UP         (MiniStyledTextCtrl::OnMouseUpOrDown)
    EVT_MIDDLE_UP       (MiniStyledTextCtrl::OnMouseUpOrDown)
    EVT_RIGHT_UP        (MiniStyledTextCtrl::OnMouseUpOrDown)
    EVT_MOTION          (MiniStyledTextCtrl::OnMouseMove)
//    EVT_LEFT_DCLICK
//    EVT_MIDDLE_DCLICK
//    EVT_RIGHT_DCLICK
    EVT_PAINT           (MiniStyledTextCtrl::OnPaint)
    EVT_ERASE_BACKGROUND(MiniStyledTextCtrl::OnEraseBackground)
END_EVENT_TABLE()


MiniStyledTextCtrl::MiniStyledTextCtrl(wxWindow* pParent, int id, const wxPoint& pos, const wxSize& size, long style):
    cbStyledTextCtrl(pParent, id, pos, size, style)
{
    SetMargins(0,0);
    for (unsigned int i = 0 ; i < wxSCI_MAX_MARGIN ; ++i)
        SetMarginWidth(i,0);
    SetZoom(-10); // smallest allowed zoom factor
    SetUseHorizontalScrollBar(false);

    wxColor color = Manager::Get()->GetColourManager()->GetColour(wxT("minidoc_background"));

    MarkerDeleteAll(GetOurMarkerNumber());
    MarkerSetBackground(GetOurMarkerNumber(), color);
    const int alpha = 100;
    MarkerSetAlpha(GetOurMarkerNumber(), alpha);


    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));// check if the feature is enabled
    //if (cfg->ReadBool(_T("/highlight_occurrence/enabled"), true)){
    const int theIndicator = 10;
    wxColour highlightColour(Manager::Get()->GetColourManager()->GetColour(wxT("editor_highlight_occurrence")));
    IndicatorSetStyle(theIndicator, wxSCI_INDIC_HIGHLIGHT);
    IndicatorSetForeground(theIndicator, highlightColour );
#ifndef wxHAVE_RAW_BITMAP
    IndicatorSetUnder(theIndicator,true);
#endif
    //}

    const int thePermIndicator = 12;
    IndicatorSetStyle(thePermIndicator, wxSCI_INDIC_HIGHLIGHT);
    IndicatorSetForeground(thePermIndicator, wxColour(Manager::Get()->GetColourManager()->GetColour(wxT("editor_highlight_occurrence_permanently"))) );
#ifndef wxHAVE_RAW_BITMAP
    IndicatorSetUnder(thePermIndicator,true);
#endif

    const int theFindFoudIndicator = 21;
    IndicatorSetStyle(theFindFoudIndicator, wxSCI_INDIC_HIGHLIGHT);
    IndicatorSetForeground(theFindFoudIndicator, wxColour(cfg->ReadColour(_T("/incremental_search/text_found_colour"), wxColour(160, 32, 240))) );
#ifndef wxHAVE_RAW_BITMAP
    IndicatorSetUnder(theFindFoudIndicator,true);
#endif

    const int theFindHighlightIndicator = 22;
    IndicatorSetStyle(theFindHighlightIndicator, wxSCI_INDIC_HIGHLIGHT);
    IndicatorSetForeground(theFindHighlightIndicator, wxColour(cfg->ReadColour(_T("/incremental_search/highlight_colour"), wxColour(255, 165, 0))) );
#ifndef wxHAVE_RAW_BITMAP
    IndicatorSetUnder(theFindHighlightIndicator,true);
#endif

}




MiniStyledTextCtrl::~MiniStyledTextCtrl()
{
    //dtor
}

void MiniStyledTextCtrl::OnKey(wxKeyEvent& event)
{   // ignore key press events
    event.Skip(false);
}
void MiniStyledTextCtrl::OnMousWheel(wxMouseEvent& event)
{   //disable zoom changes
    event.Skip(!event.m_controlDown);
}
void MiniStyledTextCtrl::OnMouseUpOrDown(wxMouseEvent& event)
{
    if( event.ButtonDown(wxMOUSE_BTN_LEFT) )
    {
        SetFocus();
        int line = GetLineFromPosition(event.GetPosition());
        MiniStyledTextCtrlLineClickedEvent evt(MiniStyledTextCtrlCommandEvent, GetId());
        evt.SetLine(line);
        wxPostEvent(this, evt);
    }
    if ( event.ButtonDown(wxMOUSE_BTN_RIGHT) )
    {
        SetFocus();
    }
    event.Skip(false);
}
void MiniStyledTextCtrl::OnMouseMove(wxMouseEvent& event)
{
    if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
    {
        int line = GetLineFromPosition(event.GetPosition());
        MiniStyledTextCtrlLineClickedEvent evt(MiniStyledTextCtrlCommandEvent, GetId());
        evt.SetLine(line);
        wxPostEvent(this, evt);
    }
}
int MiniStyledTextCtrl::GetLineFromPosition(const wxPoint &pt)
{
    int line = GetFirstVisibleLine();

    int lh = TextHeight(line);
    if (lh>0)
        line += pt.y/lh;
    return line;
}
void MiniStyledTextCtrl::OnMouseEnterOrLeave(wxMouseEvent& event)
{
    event.Skip(false);
}
void MiniStyledTextCtrl::SetVisibleRange(int from, int length, bool force)
{
    int oldVisibleFrom = visibleFrom;
    int oldVisibleLength = visibleLength;

    visibleFrom = from;
    visibleLength = length;

    if (force || (oldVisibleFrom != visibleFrom) || (oldVisibleLength != visibleLength))
    {
        SetMarker();
        MakeVisible(from, length);
    }
}
void MiniStyledTextCtrl::MakeVisible(int from, int length)
{
// TODO (danselmi#1#): Check configuration if needed to follow
    ScrollToLine(from - (LinesOnScreen()-length)/2);
}
void MiniStyledTextCtrl::SetMarker()
{
// TODO (danselmi#1#): Check configuration if we should mark the visible or the non-visible area
    bool inverse = false;
    Freeze();
    MarkerDeleteAll(GetOurMarkerNumber());
    if (inverse)
    {
        for (int l = visibleFrom; l < visibleFrom+visibleLength ; ++l)
            MarkerAdd(l, GetOurMarkerNumber());
    }
    else
    {
        for (int l = 0; l < visibleFrom ; ++l)
            MarkerAdd(l, GetOurMarkerNumber());
        for (int l = visibleFrom+visibleLength; l < GetLineCount() ; ++l)
            MarkerAdd(l, GetOurMarkerNumber());
    }
    Thaw();
}
void MiniStyledTextCtrl::UpdateMiniature(cbStyledTextCtrl *stc, bool force)
{
    SyncFoldState(stc);

    int firstVisibleLine = stc->DocLineFromVisible(stc->GetFirstVisibleLine());
    int lastVisibleLine = stc->DocLineFromVisible(stc->GetFirstVisibleLine()+stc->LinesOnScreen());

    int totalVisibleLines = lastVisibleLine-firstVisibleLine;

    SetVisibleRange(firstVisibleLine, totalVisibleLines, force);
}
void MiniStyledTextCtrl::SyncFoldState(cbStyledTextCtrl *stc)
{
    /// should we do sync the folding into the miniature?
}
const int MiniStyledTextCtrl::GetOurMarkerNumber()const
{
    return 8;
}


