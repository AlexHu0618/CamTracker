/***************************************************************
 * Name:      CamTrackerMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Alex (jthu4alex@163.com)
 * Created:   2016-06-27
 * Copyright: Alex (www.gzrobot.net)
 * License:
 **************************************************************/

#include "CamTrackerMain.h"
#include <wx/msgdlg.h>
#include <wx/image.h>
#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/arrstr.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>

//(*InternalHeaders(CamTrackerFrame)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(CamTrackerFrame)
const long CamTrackerFrame::ID_PANEL1 = wxNewId();
const long CamTrackerFrame::ID_TREECTRL1 = wxNewId();
const long CamTrackerFrame::ID_TIMER1 = wxNewId();
const long CamTrackerFrame::TreeMenu_delete = wxNewId();
const long CamTrackerFrame::ID_MESSAGEDIALOG1 = wxNewId();
//*)

cv::VideoCapture cap;
extern int FRAME_WIDTH;
extern int FRAME_HEIGHT;
bool delSavedFile = true;
bool menuItemSelected = false;

BEGIN_EVENT_TABLE(CamTrackerFrame,wxFrame)
    //(*EventTable(CamTrackerFrame)
    //*)
END_EVENT_TABLE()

CamTrackerFrame::CamTrackerFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(CamTrackerFrame)
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer3;

    Create(parent, wxID_ANY, _("CamTracker"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FIND")),wxART_FRAME_ICON));
    	SetIcon(FrameIcon);
    }
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    PanelShowImg = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(640,480), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    PanelShowImg->SetBackgroundColour(wxColour(0,0,0));
    BoxSizer3->Add(PanelShowImg, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    TreeCtrl1 = new wxTreeCtrl(this, ID_TREECTRL1, wxDefaultPosition, wxSize(190,480), wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRL1"));
    wxTreeItemId TreeCtrl1_Item1 = TreeCtrl1->AddRoot(_T("D:\\CamTrackerVideo"));
    TreeCtrl1->ExpandAll();
    BoxSizer2->Add(TreeCtrl1, 1, wxTOP|wxBOTTOM|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    Timer4refresh.SetOwner(this, ID_TIMER1);
    Timer4refresh.Start(100, false);
    static const char *ImageList1_0_XPM[] = {
    "16 16 23 1",
    "5 c Black",
    ", c #9BACC2",
    ". c #547897",
    "2 c #7F99B4",
    "X c #D1D9E5",
    "1 c #EAEDF3",
    "+ c #CAD2DC",
    "4 c #718BA7",
    "O c #BECAD9",
    "$ c #E1E6EE",
    "= c #F5F6F7",
    "< c #8DA0B9",
    "# c #D6DFE7",
    "@ c #D2D9E0",
    "; c #FAFCFE",
    ": c #ADBACE",
    "_ c None",
    "* c #EEF1F3",
    "- c #F8F9FA",
    "o c #B3BFD1",
    "3 c #7A90AC",
    "> c #A2B3C5",
    "& c #E5EAF1",
    "________________",
    "_.....__________",
    "_.XXXX._________",
    "_.............__",
    "_.oO+@#$&*=-;.__",
    "_.oO+@#$&*=-;.__",
    "_.:oO+X#$&*=-.__",
    "_.>:oO+X#$&*=.__",
    "_.,>:oO+X#$&*.__",
    "_.<,>:oO+X#$1.__",
    "_.2<,>:oO+X#$.__",
    "_.32<,>:oO+X#.__",
    "_.432<,>:oO+X.__",
    "_.............__",
    "________________",
    "5555555555555555"
    };
    static const char *ImageList1_1_XPM[] = {
    "16 16 32 1",
    "r c Black",
    "7 c #9BACC2",
    "e c #547B99",
    "6 c #94A5BD",
    ". c #376485",
    ": c #F1F4F7",
    "o c #7F99B4",
    "3 c #D1D9E5",
    "; c #EAEDF3",
    "O c #718BA7",
    "q c #65839D",
    "= c #DCE2EA",
    "> c #F5F6F7",
    "8 c #597B9A",
    "X c #8DA0B9",
    "+ c #467291",
    "w c #305F81",
    "* c #D6DFE7",
    "4 c #6A89A2",
    "2 c #A8B6CA",
    "- c #E4E9ED",
    "_ c None",
    ", c #F8F9FA",
    "< c #FDFDFE",
    "0 c #215579",
    "9 c #7F97B0",
    "@ c #B3BFD1",
    "1 c #7A90AC",
    "$ c #C2CBDB",
    "5 c #A2B3C5",
    "& c #CAD6E1",
    "# c #BBC4D6",
    "________________",
    ".....___________",
    ".XXXo.__________",
    ".XXXXO........__",
    ".XXXXXXXXXXXX.__",
    ".XXXXXXXXXXXX.__",
    ".X++++++++++++++",
    ".X+@#$&*=-;:>,<+",
    ".1.2@#$&3=-;:>34",
    "..X52@#$&3=-;:4_",
    "..X672@#$&3=-;4_",
    ".89X672@#$&3=&4_",
    "0q19X672@#$&34__",
    "w++++++++++++e__",
    "________________",
    "rrrrrrrrrrrrrrrr"
    };
    static const char *ImageList1_2_XPM[] = {
    "16 16 32 1",
    "- c #97C4E7",
    "> c #72A8D2",
    "2 c #FFFFFF",
    "e c #839CB5",
    "X c #6B98B8",
    ". c #5A89A6",
    "@ c #3A749C",
    "< c #D1E5F5",
    "1 c #67A1CF",
    ", c #F1F4F7",
    "r c #85A7BC",
    "& c #C3DDF1",
    "q c #749BB4",
    "3 c #7EA6C0",
    ": c #5F9BC8",
    "O c #538DB3",
    "; c #85BBE2",
    "$ c #D6DFE7",
    "0 c #EFF6FC",
    "o c #6591AE",
    "5 c #F7FBFD",
    "9 c #FAFCFE",
    "7 c #DAEAF7",
    "8 c #E9F3FA",
    "_ c None",
    "w c #FDFDFE",
    "4 c #E2EFF8",
    "# c #8EA9BC",
    "* c #B6D5EE",
    "= c #A5CCEA",
    "6 c #F4F9FD",
    "+ c #4581AA",
    "__..XooO+@#$____",
    "__.&&*=-;:>:,___",
    "__.<<&*=-12-X,__",
    "__#&&&&*=322-X__",
    "__#4;;;;;1oXoO__",
    "__#4567<&*=-;O__",
    "__#4;;;;;;;;-O__",
    "__#466847<&*=o__",
    "__#4;;;;;;;;*o__",
    "__#49560847<&X__",
    "__#4;;;;;;;;<q__",
    "__#42w9560847q__",
    "__#4;;;;;;;;4e__",
    "__#4222w95608r__",
    "__##########r#__",
    "________________"
    };
    static const char *ImageList1_3_XPM[] = {
    "16 16 32 1",
    "- c #97C4E7",
    "> c #72A8D2",
    "2 c #FFFFFF",
    "e c #839CB5",
    "X c #6B98B8",
    ". c #5A89A6",
    "@ c #3A749C",
    "< c #D1E5F5",
    "1 c #67A1CF",
    ", c #F1F4F7",
    "r c #85A7BC",
    "& c #C3DDF1",
    "q c #749BB4",
    "3 c #7EA6C0",
    ": c #5F9BC8",
    "O c #538DB3",
    "; c #85BBE2",
    "$ c #D6DFE7",
    "0 c #EFF6FC",
    "o c #6591AE",
    "5 c #F7FBFD",
    "9 c #FAFCFE",
    "7 c #DAEAF7",
    "8 c #E9F3FA",
    "_ c #010202",
    "w c #FDFDFE",
    "4 c #E2EFF8",
    "# c #8EA9BC",
    "* c #B6D5EE",
    "= c #A5CCEA",
    "6 c #F4F9FD",
    "+ c #4581AA",
    "__..XooO+@#$____",
    "__.&&*=-;:>:,___",
    "__.<<&*=-12-X,__",
    "__#&&&&*=322-X__",
    "__#4;;;;;1oXoO__",
    "__#4567<&*=-;O__",
    "__#4;;;;;;;;-O__",
    "__#466847<&*=o__",
    "__#4;;;;;;;;*o__",
    "__#49560847<&X__",
    "__#4;;;;;;;;<q__",
    "__#42w9560847q__",
    "__#4;;;;;;;;4e__",
    "__#4222w95608r__",
    "__##########r#__",
    "________________"
    };
    ImageList1 = new wxImageList(16, 16, 5);
    ImageList1->Add(wxBitmap(ImageList1_0_XPM));
    ImageList1->Add(wxBitmap(ImageList1_1_XPM));
    ImageList1->Add(wxBitmap(ImageList1_2_XPM));
    ImageList1->Add(wxBitmap(ImageList1_3_XPM));
    MenuItem1 = new wxMenuItem((&Menu1), TreeMenu_delete, _("&Delete"), wxEmptyString, wxITEM_NORMAL);
    Menu1.Append(MenuItem1);
    MessageDialog1 = new wxMessageDialog(this, _("是否同步删除D盘中保存的相应视频文件？"), _("Notice"), wxYES_NO|wxYES_DEFAULT|wxICON_QUESTION, wxDefaultPosition);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    PanelShowImg->Connect(wxEVT_SIZE,(wxObjectEventFunction)&CamTrackerFrame::OnPanelShowImgResize1,0,this);
    // Set the images for TreeCtrl1.
    TreeCtrl1->SetImageList(ImageList1);
    TreeCtrl1->SetItemImage(TreeCtrl1_Item1, 0, wxTreeItemIcon_Normal);
    TreeCtrl1->SetItemImage(TreeCtrl1_Item1, 0, wxTreeItemIcon_Selected);
    TreeCtrl1->SetItemImage(TreeCtrl1_Item1, 1, wxTreeItemIcon_Expanded);
    TreeCtrl1->SetItemImage(TreeCtrl1_Item1, 1, wxTreeItemIcon_SelectedExpanded);
    Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_ITEM_ACTIVATED,(wxObjectEventFunction)&CamTrackerFrame::OnTreeCtrl1ItemActivated);
    Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_ITEM_MENU,(wxObjectEventFunction)&CamTrackerFrame::OnTreeCtrl1ItemMenu);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&CamTrackerFrame::OnTimer4refreshTrigger);
    Connect(TreeMenu_delete,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&CamTrackerFrame::OnMenuItem1Selected);
    //*)

    dc = new wxClientDC(PanelShowImg);
    AddItem2Tree(TreeCtrl1_Item1);
    //initial OpenCV
    cap.open(0);
    if(!cap.isOpened())
    {
        wxMessageBox(_T("Fail to open camera0!"), _T("Error"));
    }
    ObjTrack = new ObjectTracking(cap);
}

CamTrackerFrame::~CamTrackerFrame()
{
    //(*Destroy(CamTrackerFrame)
    //*)
    delete ObjTrack;
}

void CamTrackerFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void CamTrackerFrame::OnAbout(wxCommandEvent& event)
{
    //wxString msg = wxbuildinfo(long_f);
    wxString msg = _T("This is the software CamTracker!\n\n Version 1.1");
    wxMessageBox(msg, _T("About me"));
}

void CamTrackerFrame::OnTimer4refreshTrigger(wxTimerEvent& event)
{
    cv::Mat img1,img2;
    // First, read the current frame from the cam
    /*cap.read(img1); // m_vCap correspond to the opened capture device*/
    ObjTrack->tracking(img1);
    // Get the width and height of the captured image
    int iWdth=cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int iHght=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    cvtColor(img1,img2,CV_BGR2RGB);
    // Create a wxImage from the captured datas
    wxImage Image( (int)iWdth, (int)iHght, img2.data, TRUE );
    wxBitmap bitmap = wxBitmap(Image);

    dc->DrawBitmap(bitmap,0,0);
    if(ObjTrack->shouldAddItem)
    {
        ObjTrack->shouldAddItem = false;
        wxString str4dirs = _T("D:\\CamTrackerVideo\\");
        wxDir dir4show;

        dir4show.Open(str4dirs);
        if(!dir4show.IsOpened())
        {
            wxMessageBox(_T("Fail to open the directorys!"),_T("Error"));
        }
        wxArrayString arrayItemsDirs;
        if(dir4show.GetAllFiles(str4dirs,&arrayItemsDirs)>1)
        {
            wxString lastDir = arrayItemsDirs[arrayItemsDirs.GetCount()-2].BeforeLast('\\').AfterLast('\\');
            wxString curDir = arrayItemsDirs.Last().BeforeLast('\\').AfterLast('\\');
            if( curDir == lastDir )
            {
                wxTreeItemId TreeCtrl1_Item3 = this->TreeCtrl1->AppendItem(this->TreeCtrl1->GetLastChild(this->TreeCtrl1->GetFirstVisibleItem()), arrayItemsDirs.Last().AfterLast('\\'));
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item3, 2, wxTreeItemIcon_Normal);
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item3, 3, wxTreeItemIcon_Selected);
            }
            else
            {
                wxTreeItemId TreeCtrl1_Item2 = this->TreeCtrl1->AppendItem(this->TreeCtrl1->GetRootItem(), curDir);
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item2, 0, wxTreeItemIcon_Normal);
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item2, 0, wxTreeItemIcon_Selected);
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item2, 1, wxTreeItemIcon_Expanded);
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item2, 1, wxTreeItemIcon_SelectedExpanded);

                wxTreeItemId TreeCtrl1_Item3 = this->TreeCtrl1->AppendItem(TreeCtrl1_Item2, arrayItemsDirs.Last().AfterLast('\\'));
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item3, 2, wxTreeItemIcon_Normal);
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item3, 3, wxTreeItemIcon_Selected);
            }
        }
        else
        {
            //wxMessageBox(_T("Fail to GetAllFiles!"),_T("Error"));
            AddItem2Tree(this->TreeCtrl1->GetRootItem());
        }
    }
}

void CamTrackerFrame::AddItem2Tree(wxTreeItemId TreeCtrl1_Item1)
{
    wxString dirName = wxEmptyString;
    wxString str4dirs = _T("D:\\CamTrackerVideo\\");
    wxDir dir4show;

    if(!wxFileName::DirExists(str4dirs))
    {
        if(!wxFileName::Mkdir(str4dirs))
        {
            wxMessageBox(_T("Fail to build directory CamTrackerVideo!"),_T("Error"));
        }
    }

    dir4show.Open(str4dirs);
    if(!dir4show.IsOpened())
    {
        wxMessageBox(_T("Fail to open the directorys!"),_T("Error"));
    }
    wxArrayString arrayItemsDirs,arrayItems;
    if(dir4show.GetAllFiles(str4dirs,&arrayItemsDirs))
    {
        wxTreeItemId TreeCtrl1_Item2;
        for(int i=0;i<(int)arrayItemsDirs.GetCount();++i)
        {
            if(arrayItemsDirs[i].BeforeLast('\\').AfterLast('\\') != dirName){

            TreeCtrl1_Item2 = this->TreeCtrl1->AppendItem(TreeCtrl1_Item1, arrayItemsDirs[i].BeforeLast('\\').AfterLast('\\'));
            this->TreeCtrl1->SetItemImage(TreeCtrl1_Item2, 0, wxTreeItemIcon_Normal);
            this->TreeCtrl1->SetItemImage(TreeCtrl1_Item2, 0, wxTreeItemIcon_Selected);
            this->TreeCtrl1->SetItemImage(TreeCtrl1_Item2, 1, wxTreeItemIcon_Expanded);
            this->TreeCtrl1->SetItemImage(TreeCtrl1_Item2, 1, wxTreeItemIcon_SelectedExpanded);
            if(dirName == wxEmptyString)
            {
                wxTreeItemId TreeCtrl1_Item3 = this->TreeCtrl1->AppendItem(TreeCtrl1_Item2, arrayItemsDirs[i].AfterLast('\\'));
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item3, 2, wxTreeItemIcon_Normal);
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item3, 3, wxTreeItemIcon_Selected);
            }
            dirName = arrayItemsDirs[i].BeforeLast('\\').AfterLast('\\');}

            else{
                wxTreeItemId TreeCtrl1_Item3 = this->TreeCtrl1->AppendItem(TreeCtrl1_Item2, arrayItemsDirs[i].AfterLast('\\'));
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item3, 2, wxTreeItemIcon_Normal);
                this->TreeCtrl1->SetItemImage(TreeCtrl1_Item3, 3, wxTreeItemIcon_Selected);}
        }
    }
    /*else
    {
        wxMessageBox(_T("Dir is empty!"),_T("Error"));
    }*/

}

void CamTrackerFrame::OnTreeCtrl1ItemActivated(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    wxString itemText = this->TreeCtrl1->GetItemText(itemId);
    if(itemText.AfterLast('.') == _T("avi"))
    {
        itemId = this->TreeCtrl1->GetItemParent(itemId);
        wxString parentItemText = this->TreeCtrl1->GetItemText(itemId);
        wxString str4path = _T("D:\\CamTrackerVideo\\")+parentItemText+_T("\\")+itemText;
        //wxMessageBox(parentItemText,_T("Error"));
        ShellExecuteW(NULL,L"open",L"wmplayer",str4path,NULL,SW_SHOW);
    }
}

void CamTrackerFrame::OnPanelShowImgResize1(wxSizeEvent& event)
{
    // 获取象素单位的当前设备上下文的大小
    wxCoord w, h;
    dc->GetSize(&w, &h);
    //计算一个合适的缩放值
    float scaleX = ( float )w/FRAME_WIDTH ;
    float scaleY = ( float )h/FRAME_HEIGHT ;
    // 选择或者方向上较小的那个XY
    float actualScale = wxMin ( scaleX , scaleY );
    // 计算图片在设备上的合适位置以便居中
    float posX = ( float )(( w-(FRAME_WIDTH*actualScale ))/2.0);
    float posY = ( float )(( h-(FRAME_HEIGHT*actualScale ))/2.0);
    // 设置设备平移和缩放
    dc->SetUserScale ( actualScale , actualScale );
    dc->SetDeviceOrigin ( ( long )posX , ( long ) posY );
    dc->Clear();
}

void CamTrackerFrame::OnTreeCtrl1ItemMenu(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
    wxTreeItemData *item = itemId.IsOk() ? this->TreeCtrl1->GetItemData(itemId)
                                         : NULL;
    wxPoint clientpt = event.GetPoint();
    wxCoord w, h;
    dc->GetSize(&w, &h);
    double scaleX,scaleY;
    dc->GetUserScale(&scaleX,&scaleY);
    clientpt.x += w;
    clientpt.x += (int)(25*scaleX);
    clientpt.y += (int)(10*scaleY);
    wxPoint screenpt = ClientToScreen(clientpt);

    wxString title;
    if ( itemId.IsOk() )
    {
        title << wxT("Menu for ") << this->TreeCtrl1->GetItemText(itemId);
    }
    else
    {
        title = wxT("Menu for no particular item");
    }

    PopupMenu(&Menu1, clientpt);
    event.Skip();
    if( menuItemSelected )
    {
        if( delSavedFile )    //delete the saved file at the same time
        {
            if( this->TreeCtrl1->ItemHasChildren(itemId) )
            {
                wxString parentItemText = this->TreeCtrl1->GetItemText(itemId);
                wxDir dir4del(_T("D:\\CamTrackerVideo\\")+parentItemText);
                wxString itemText;
                if( dir4del.GetFirst(&itemText) )
                {
                    do
                    {
                        wxString str4path = _T("D:\\CamTrackerVideo\\")+parentItemText+_T("\\")+itemText;
                        if( !wxRemoveFile(str4path) )
                        {
                            wxString str = _T("Fail to remove file ")+itemText;
                            wxMessageBox(str,_T("Error"));
                            break;
                        }
                    }while(dir4del.GetNext(&itemText));
                }
                if( !dir4del.HasFiles() )
                {
                    wxRmdir(_T("D:\\CamTrackerVideo\\")+parentItemText);
                    this->TreeCtrl1->DeleteChildren(itemId);
                    this->TreeCtrl1->Delete(itemId);
                }
            }
            else
            {
                wxString itemText = this->TreeCtrl1->GetItemText(itemId);
                if( itemText.Find('.',true) ==wxNOT_FOUND )
                {
                    wxRmdir(_T("D:\\CamTrackerVideo\\")+itemText);
                    this->TreeCtrl1->Delete(itemId);
                }
                else
                {
                    wxTreeItemId ParentItemId = this->TreeCtrl1->GetItemParent(itemId);
                    wxString parentItemText = this->TreeCtrl1->GetItemText(ParentItemId);
                    wxString str4path = _T("D:\\CamTrackerVideo\\")+parentItemText+_T("\\")+itemText;
                    if( wxRemoveFile(str4path) )
                    {
                        this->TreeCtrl1->Delete(itemId);
                    }
                }
            }
        }
        else
        {
            if( this->TreeCtrl1->ItemHasChildren(itemId) )
            {
                this->TreeCtrl1->DeleteChildren(itemId);
                this->TreeCtrl1->Delete(itemId);
            }
            else
            {
                this->TreeCtrl1->Delete(itemId);
            }
        }
    }
    menuItemSelected = false;
}

void CamTrackerFrame::OnMenuItem1Selected(wxCommandEvent& event)
{
    menuItemSelected = true;
    if(MessageDialog1->ShowModal() == wxID_YES)
    {
        delSavedFile = true;
    }
    else
    {
        delSavedFile = false;
    }
}
