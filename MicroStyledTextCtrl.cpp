#include "MicroStyledTextCtrl.h"
#include "MiniStyledTextCtrlEvent.h"

#include <wx/dcclient.h>
#include <wx/settings.h>
#include "configmanager.h"
#include "cbcolourmanager.h"
#include "logmanager.h"

const unsigned int COLUMNS_PER_LINE = 4;

BEGIN_EVENT_TABLE(MicroStyledTextCtrl, wxPanel)
    EVT_LEFT_DOWN       (MicroStyledTextCtrl::OnMouseDown)
    EVT_PAINT           (MicroStyledTextCtrl::OnPaint)
    EVT_ERASE_BACKGROUND(MicroStyledTextCtrl::OnEraseBackground)
END_EVENT_TABLE()

MicroStyledTextCtrl::MicroStyledTextCtrl(wxWindow* pParent, int id):
    wxPanel(pParent, id),
    linesInDoc_(0)
{
    Init();
}

void MicroStyledTextCtrl::Init()
{
    bgColor = *wxWHITE;
}

MicroStyledTextCtrl::~MicroStyledTextCtrl()
{

}

void MicroStyledTextCtrl::OnPaint(wxPaintEvent &evt)
{
    wxPaintDC dc(this);

    wxSize sz = GetSize();
    dc.SetPen(wxPen(bgColor));
    dc.SetBrush(wxBrush(bgColor));
    dc.DrawRectangle(wxPoint(0,0), sz);
    if ( getHeight() >= linesInDoc_ )
    {
        //wxColor wxCtrlBgColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);	//Window background colour.
        wxColor wxCtrlBgColor = wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE );
        dc.SetPen(wxPen(wxCtrlBgColor));
        dc.SetBrush(wxBrush(wxCtrlBgColor));
        dc.DrawRectangle(wxPoint(0, linesInDoc_), wxSize(sz.x, sz.y-linesInDoc_));
    }
    if(linesInDoc_ == 0)
        return;

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
    bool inverse = cfg->ReadBool(_T("/mini_doc/inverse_designator"), false);
    wxColor markColor = Manager::Get()->GetColourManager()->GetColour(wxT("minidoc_background"));
    wxColor brushColor(markColor.Red(), markColor.Green(), markColor.Blue(), 20);
    dc.SetPen(wxPen(brushColor));
    dc.SetBrush(wxBrush(brushColor));
    int y1 = firstVisibleLine, y2 = lastVisibleLine, y3 = linesInDoc_;
    if ( getHeight() < linesInDoc_ )
    {
        y1 = y1 * sz.y/linesInDoc_;
        y2 = y2 * sz.y/linesInDoc_;
        y3 = y3 * sz.y/linesInDoc_;
    }
    if (inverse)
    {
        dc.DrawRectangle(0, y1, sz.x, y2-y1);
    }
    else
    {
        dc.DrawRectangle(0, 0, sz.x, y1);
        dc.DrawRectangle(0, y2, sz.x, y3-y2);
    }

    for ( unsigned k = 0; k < visibles.size() ; ++k )
    {
        dc.SetPen(wxPen(visibles[k].color));
        int y = visibles[k].line;
        if ( getHeight() < linesInDoc_ )
            y = y * sz.y/linesInDoc_;
        int x1 = visibles[k].colStart*sz.x/COLUMNS_PER_LINE;
        int x2 = (visibles[k].colEnd+1)*sz.x/COLUMNS_PER_LINE;
        dc.DrawLine(wxPoint(x1,y), wxPoint(x2, y));
    }
}
void MicroStyledTextCtrl::OnEraseBackground(wxEraseEvent &evt){}
void MicroStyledTextCtrl::UpdateMiniature(cbStyledTextCtrl *stc, bool force)
{
    linesInDoc_ = stc->GetLineCount();
    bgColor = stc->StyleGetBackground(wxSCI_STYLE_DEFAULT);

    int oldFirstVisibleLine = firstVisibleLine;
    int oldLastVisibleLine = lastVisibleLine;

    firstVisibleLine = stc->DocLineFromVisible(stc->GetFirstVisibleLine());
    lastVisibleLine = stc->DocLineFromVisible(stc->GetFirstVisibleLine()+stc->LinesOnScreen());

    if ((oldFirstVisibleLine != firstVisibleLine) || (oldLastVisibleLine != lastVisibleLine))
        force = true;

    visibles.clear();
    GetVisibles(stc);

    //if (force)
        Refresh();
}
void MicroStyledTextCtrl::GetVisibles(cbStyledTextCtrl *stc)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
    if (cfg->ReadBool(_T("/highlight_occurrence/enabled"), true))
    {
        const int theIndicator = 10;
        wxColor color = Manager::Get()->GetColourManager()->GetColour(wxT("editor_highlight_occurrence"));
        GetvisibleIndicators(stc, theIndicator, color);
    }
    const int thePermIndicator = 12;
    GetvisibleIndicators(stc, thePermIndicator, wxColour(Manager::Get()->GetColourManager()->GetColour(wxT("editor_highlight_occurrence_permanently"))));

    const int theFindFoudIndicator = 21;
    GetvisibleIndicators(stc, theFindFoudIndicator, wxColour(cfg->ReadColour(_T("/incremental_search/text_found_colour"), wxColour(160, 32, 240))));

    const int theFindHighlightIndicator = 22;
    GetvisibleIndicators(stc, theFindHighlightIndicator, wxColour(cfg->ReadColour(_T("/incremental_search/highlight_colour"), wxColour(255, 165, 0))));
}
void MicroStyledTextCtrl::GetvisibleIndicators(cbStyledTextCtrl *stc, int theIndicator, const wxColor &color)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
    const int charsPerLine = cfg->ReadInt(_T("/gutter/column"), 80);
    int pos = 0;
    const int COLUMNS = COLUMNS_PER_LINE;
    for(pos = 0 ; pos < stc->GetLength() ; ++pos)
    {
        int indVal = stc->IndicatorValueAt(theIndicator,pos);
        if (indVal)
        {
            int end = stc->IndicatorEnd(theIndicator, pos);

            visible vis;
            vis.color = color;
            vis.line = stc->LineFromPosition(pos);
            vis.colStart = (pos-stc->PositionFromLine(vis.line))*COLUMNS/charsPerLine;
            if (vis.colStart > 3)
                vis.colStart = 3;
            char columns = (end-pos)*COLUMNS/charsPerLine;
            vis.colEnd = vis.colStart + columns;
            if ( vis.colEnd > 3 )
                vis.colEnd = 3;
            visibles.push_back(vis);
            pos = end;
        }
    }
    return;
}
void MicroStyledTextCtrl::AssociateNoDoc()
{
    bool doRefresh = linesInDoc_ != 0;
    linesInDoc_ = 0;
    visibles.clear();
    if (doRefresh)
        Refresh();
}
void MicroStyledTextCtrl::UpdateConfig()
{
    Init();
}

void MicroStyledTextCtrl::OnMouseDown(wxMouseEvent& event)
{
    //SetFocus();
    int line = GetLineFromPosition(event.GetPosition());
    MiniStyledTextCtrlLineClickedEvent evt(MiniStyledTextCtrlCommandEvent, GetId());
    evt.SetLine(line);
    wxPostEvent(this, evt);
}

int MicroStyledTextCtrl::getHeight()
{
    GetSize().GetY();
}

int MicroStyledTextCtrl::GetLineFromPosition(const wxPoint &pt)
{
    int pos = pt.y;
    if ( getHeight() >= linesInDoc_ )
    {
        if ( pos < linesInDoc_)
            return pos;
        return linesInDoc_;
    }
    else
    {
        return (pt.y * linesInDoc_ )/ getHeight();
    }
}

