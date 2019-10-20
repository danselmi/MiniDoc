/***************************************************************
 * Name:      MiniDoc
 * Purpose:   Code::Blocks plugin
 * Author:     ()
 * Created:   2014-07-29
 * Copyright:
 * License:   GPL
 **************************************************************/

#ifndef MiniDoc_H_INCLUDED
#define MiniDoc_H_INCLUDED

// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cbplugin.h> // for "class cbPlugin"

class MiniDocPanel;
class MiniDocConfigPanel;
class wxMenu;

class MiniDoc : public cbPlugin
{
    public:
        MiniDoc();
        virtual ~MiniDoc(){}

        /** Invoke configuration dialog. */
        virtual int Configure();
        virtual int GetConfigurationPriority() const { return 50; }
        virtual int GetConfigurationGroup() const { return cgEditor; }
        virtual cbConfigurationPanel *GetConfigurationPanel(wxWindow *parent);
        virtual cbConfigurationPanel *GetProjectConfigurationPanel(wxWindow *parent, cbProject *project){ return 0; }
        virtual void BuildMenu(wxMenuBar *menuBar);

        virtual void BuildModuleMenu(const ModuleType type, wxMenu *menu, const FileTreeData *data = 0){}
        virtual bool BuildToolBar(wxToolBar *toolBar){return false;}

    protected:
        virtual void OnAttach();
        virtual void OnRelease(bool appShutDown);

    private:
        void OnEditorClose(CodeBlocksEvent &event);
        void OnEditorActivated(CodeBlocksEvent &event);
        void OnEditorDeactivated(CodeBlocksEvent &event);
        void OnEditorSplit(CodeBlocksEvent &event);
        void OnEditorUnsplit(CodeBlocksEvent &event);
        void OnResize(wxSizeEvent &event);

        void OnViewMiniDocPanel(wxCommandEvent &event);
        void OnUpdateViewMenu(wxUpdateUIEvent &event);

        void OnEditorHook(cbEditor *editor, wxScintillaEvent &event);

        int m_FunctorId;
        MiniDocPanel *m_pPanel;
        EditorBase *currentEb_;
        wxMenu *m_pViewMenu;
    private:
        DECLARE_EVENT_TABLE();
};

#endif
