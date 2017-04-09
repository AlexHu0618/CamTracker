/***************************************************************
 * Name:      CamTrackerApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Alex (jthu4alex@163.com)
 * Created:   2016-06-27
 * Copyright: Alex (www.gzrobot.net)
 * License:
 **************************************************************/

#include "CamTrackerApp.h"

//(*AppHeaders
#include "CamTrackerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(CamTrackerApp);

bool CamTrackerApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	CamTrackerFrame* Frame = new CamTrackerFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
