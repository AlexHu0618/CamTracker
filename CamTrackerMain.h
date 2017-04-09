/***************************************************************
 * Name:      CamTrackerMain.h
 * Purpose:   Defines Application Frame
 * Author:    Alex (jthu4alex@163.com)
 * Created:   2016-06-27
 * Copyright: Alex (www.gzrobot.net)
 * License:
 **************************************************************/

#ifndef CAMTRACKERMAIN_H
#define CAMTRACKERMAIN_H

//(*Headers(CamTrackerFrame)
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/imaglist.h>
#include <wx/frame.h>
#include <wx/timer.h>
//*)
#include "ObjectTracking.h"
#include <wx/dcclient.h>

class CamTrackerFrame: public wxFrame
{
    public:

        CamTrackerFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~CamTrackerFrame();

    private:

        //(*Handlers(CamTrackerFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnTimer4refreshTrigger(wxTimerEvent& event);
        void OnPanelShowImgResize(wxSizeEvent& event);
        void OnTreeCtrl1ItemActivated(wxTreeEvent& event);
        void OnPanelShowImgResize1(wxSizeEvent& event);
        void OnTreeCtrl1ItemMenu(wxTreeEvent& event);
        void OnMenuItem1Selected(wxCommandEvent& event);
        //*)

        //(*Identifiers(CamTrackerFrame)
        static const long ID_PANEL1;
        static const long ID_TREECTRL1;
        static const long ID_TIMER1;
        static const long TreeMenu_delete;
        static const long ID_MESSAGEDIALOG1;
        //*)

        //(*Declarations(CamTrackerFrame)
        wxMenu Menu1;
        wxMenuItem* MenuItem1;
        wxTimer Timer4refresh;
        wxImageList* ImageList1;
        wxTreeCtrl* TreeCtrl1;
        wxMessageDialog* MessageDialog1;
        wxPanel* PanelShowImg;
        //*)
        wxClientDC* dc;
        ObjectTracking* ObjTrack;

        void AddItem2Tree(wxTreeItemId TreeCtrl1_Item1);

        DECLARE_EVENT_TABLE()
};

#endif // CAMTRACKERMAIN_H
