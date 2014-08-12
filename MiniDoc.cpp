#include <sdk.h> // Code::Blocks SDK
#include "configurationpanel.h"
#include "editor_hooks.h"
#include "cbcolourmanager.h"
#include "editormanager.h"
#include "cbeditor.h"

#include "MiniDoc.h"
#include "MiniDocPanel.h"
#include "MiniDocConfigPanel.h"

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<MiniDoc> reg(_T("MiniDoc"));

    const long idViewMiniDocPanel = wxNewId();
}


// events handling
BEGIN_EVENT_TABLE(MiniDoc, cbPlugin)
    // add any events you want to handle here

    EVT_MENU(idViewMiniDocPanel,      MiniDoc::OnViewMiniDocPanel)
    EVT_UPDATE_UI(idViewMiniDocPanel, MiniDoc::OnUpdateViewMenu)


END_EVENT_TABLE()

// constructor
MiniDoc::MiniDoc():
    m_pPanel(NULL)
{
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("MiniDoc.zip")))
        NotifyMissingFile(_T("MiniDoc.zip"));
}

// destructor
MiniDoc::~MiniDoc()
{
}

void MiniDoc::OnAttach()
{
    // do whatever initialization you need for your plugin
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...

    ColourManager* cm = Manager::Get()->GetColourManager();
    cm->RegisterColour(_("MiniDoc"), _("Background"), wxT("minidoc_background"), *wxLIGHT_GREY  );


    m_pPanel = new MiniDocPanel(Manager::Get()->GetAppWindow());

    EditorHooks::HookFunctorBase *editor_hook = new EditorHooks::HookFunctor<MiniDoc>(this, &MiniDoc::OnEditorHook);
    m_FunctorId = EditorHooks::RegisterHook( editor_hook );


    // add the MiniDoc-panel to the docking system
    CodeBlocksDockEvent dockevt(cbEVT_ADD_DOCK_WINDOW);
    dockevt.name = _T("MiniDoc");
    dockevt.title = _("MiniDoc");
    dockevt.pWindow = m_pPanel;
    dockevt.minimumSize.Set(50, 50);
    dockevt.desiredSize.Set(150, 100);
    dockevt.floatingSize.Set(100, 150);
    dockevt.dockSide = CodeBlocksDockEvent::dsRight;
    dockevt.stretch = true;
    Manager::Get()->ProcessEvent(dockevt);


    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_CLOSE,       new cbEventFunctor<MiniDoc, CodeBlocksEvent>(this, &MiniDoc::OnEditorClose));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_OPEN,        new cbEventFunctor<MiniDoc, CodeBlocksEvent>(this, &MiniDoc::OnEditorOpen));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_ACTIVATED,   new cbEventFunctor<MiniDoc, CodeBlocksEvent>(this, &MiniDoc::OnEditorActivated));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_DEACTIVATED, new cbEventFunctor<MiniDoc, CodeBlocksEvent>(this, &MiniDoc::OnEditorDeactivated));
}

void MiniDoc::OnRelease(bool appShutDown)
{
    EditorHooks::UnregisterHook(m_FunctorId);

    if ( m_pPanel && !appShutDown )
    {
        CodeBlocksDockEvent docevt(cbEVT_REMOVE_DOCK_WINDOW);
        docevt.pWindow = m_pPanel;
        Manager::Get()->ProcessEvent(docevt);
        // and destroy the panel
        m_pPanel->Destroy();
        m_pPanel = NULL;
    }
}

void MiniDoc::OnEditorClose(CodeBlocksEvent& event)
{
    if (m_pPanel && IsAttached())
        m_pPanel->ShowMiniatureOf(Manager::Get()->GetEditorManager()->GetActiveEditor());
}
void MiniDoc::OnEditorOpen(CodeBlocksEvent& event)
{
}
void MiniDoc::OnEditorActivated(CodeBlocksEvent& event)
{
    if (m_pPanel && IsAttached())
    {
        EditorBase *eb = event.GetEditor();
        m_pPanel->ShowMiniatureOf(eb);
        if(eb && eb->IsBuiltinEditor())
            eb->Connect(wxEVT_SIZE, (wxObjectEventFunction)&MiniDoc::OnResize, NULL, this);
    }
}
void MiniDoc::OnEditorDeactivated(CodeBlocksEvent& event)
{
    if (m_pPanel && IsAttached())
    {
        EditorBase *eb = event.GetEditor();
        if(eb && eb->IsBuiltinEditor())
            eb->Disconnect(wxEVT_SIZE, (wxObjectEventFunction)&MiniDoc::OnResize, NULL, this);
        m_pPanel->ShowMiniatureOf(NULL);
    }
}

void MiniDoc::OnResize(wxSizeEvent &event)
{
    EditorBase *eb = Manager::Get()->GetEditorManager()->GetActiveEditor();
    m_pPanel->UpdateMiniStc(eb, true);
    event.Skip();
}

void MiniDoc::OnEditorHook(cbEditor* editor, wxScintillaEvent& event)
{
    if(event.GetEventType() == wxEVT_SCI_CHARADDED)
    {
        cbStyledTextCtrl *stc = editor->GetControl();
        wxChar ch = event.GetKey();
        if ( (ch == wxT('\n')) || ( (stc->GetEOLMode() == wxSCI_EOL_CR) && (ch == wxT('\r')) ) )
            m_pPanel->UpdateMiniStc(editor, true);
    }
    else if(event.GetEventType() == wxEVT_SCI_UPDATEUI)
        m_pPanel->UpdateMiniStc(editor);
//    else if(event.GetEventType() == wxEVT_SCI_MODIFIED)
//    {
//        if ( event.GetModificationType() & wxSCI_MOD_CHANGEFOLD)
//            m_pPanel->UpdateMiniStc(editor, true);
//    }
}
int MiniDoc::Configure()
{
    //create and display the configuration dialog for your plugin
    cbConfigurationDialog dlg(Manager::Get()->GetAppWindow(), wxID_ANY, _("Your dialog title"));
    cbConfigurationPanel* panel = GetConfigurationPanel(&dlg);
    if (panel)
    {
        dlg.AttachConfigurationPanel(panel);
        PlaceWindow(&dlg);
        return dlg.ShowModal() == wxID_OK ? 0 : -1;
    }
    return -1;
}

void MiniDoc::BuildMenu(wxMenuBar* menuBar)
{
    // insert entry in the View menu
    int viewPos = menuBar->FindMenu(_("&View"));
    if (viewPos != wxNOT_FOUND)
    {
        m_pViewMenu = menuBar->GetMenu(viewPos);
        wxMenuItemList& items = m_pViewMenu->GetMenuItems();
        // find the first separator and insert before it
        for (size_t i = 0; i < items.GetCount(); ++i)
        {
            if (items[i]->IsSeparator())
            {
                m_pViewMenu->InsertCheckItem(i, idViewMiniDocPanel, _("&MiniDoc"), _("Toggle displaying the MiniDoc panel"));
                return;
            }
        }
        // not found so just append
        m_pViewMenu->AppendCheckItem(idViewMiniDocPanel, _("&MiniDoc"), _("Toggle displaying the MiniDoc panel"));
    }
}

void MiniDoc::BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data)
{
    //Some library module is ready to display a pop-up menu.
    //Check the parameter \"type\" and see which module it is
    //and append any items you need in the menu...
    //TIP: for consistency, add a separator as the first item...
}

bool MiniDoc::BuildToolBar(wxToolBar* toolBar)
{
    // return true if you add toolbar items
    return false;
}


void MiniDoc::OnViewMiniDocPanel(wxCommandEvent& event)
{
    CodeBlocksDockEvent evt(event.IsChecked() ? cbEVT_SHOW_DOCK_WINDOW : cbEVT_HIDE_DOCK_WINDOW);
    evt.pWindow = m_pPanel;
    Manager::Get()->ProcessEvent(evt);
}
void MiniDoc::OnUpdateViewMenu(wxUpdateUIEvent &event)
{
    if (m_pViewMenu)
    {
        bool isVis = IsWindowReallyShown((wxWindow*)m_pPanel);
        m_pViewMenu->Check(idViewMiniDocPanel, isVis);
    }
    // must do...
    event.Skip();
}


cbConfigurationPanel* MiniDoc::GetConfigurationPanel(wxWindow* parent)
{
    return new MiniDocConfigPanel(parent, m_pPanel);
}
