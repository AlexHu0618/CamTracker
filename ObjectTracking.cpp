/***************************************************************
 * Name:      ObjectTracking.h
 * Purpose:   Define member function to class ObjectTracking
 * Author:    Alex (jthu4alex@163.com)
 * Created:   2016-07-13
 * Copyright: Alex (www.gzrobot.net)
 * License:
 **************************************************************/

#include "ObjectTracking.h"
#include <wx/string.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <opencv2/imgproc/imgproc.hpp>

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 35;//50;
int H_MAX = 77;//80;
int S_MIN = 52;//120;
int S_MAX = 181;//256;
int V_MIN = 54;//0;
int V_MAX = 116;//256;
//default capture width and height,they will be used to set the video
int FRAME_WIDTH = 640;
int FRAME_HEIGHT = 480;
const double FPS = 10.0;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
bool shouldCreate = true;
bool justObjFound = false;
VideoWriter writer;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

const long ObjectTracking::ID_TIMER2 = wxNewId();

ObjectTracking::ObjectTracking(VideoCapture cap)
{
    icap=cap;
    trackObjects = true;
    useMorphOps = true;
    x=0;
    y=0;
    shouldAddItem = false;
    //timer4save.SetOwner(this,ID_TIMER2);
}

/** \brief use some of the openCV drawing functions to draw crosshairs on your tracked image!
 * the brief will continue here.
 * \details this is the detalis about the function.
 * \param x is the center Point(x,y)
 * \param y is the center Point(x,y)
 * \param frame is a pointer to the original image
 * \return no return, the circle or line will add to the frame directly.²âÊÔÖÐÎÄ
 *
 */
void ObjectTracking::drawObject(int x, int y,Mat &frame)
{
	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
        line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else
        line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
        line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else
        line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
        line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else
        line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
        line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else
        line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

	/*putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);*/
}

void ObjectTracking::morphOps(Mat &thresh)
{
	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

void ObjectTracking::trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed)
{
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	Mat dst = Mat::zeros(temp.size(), CV_8UC3);
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
                justObjFound = true;
				putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,cameraFeed);
				if(shouldCreate)
                {
                    //these for built a directory and a file
                    wxDateTime DT = wxDateTime::Now();
                    wxString str4date = DT.FormatISODate();
                    wxString str4time = DT.FormatISOTime();
                    str4time.Replace(":","-");
                    wxString str4dir=_T("D:\\CamTrackerVideo\\")+str4date;
                    wxFileName::SetCwd(_T("D:\\"));    //we should set the current working directory before build a sub-directory
                    if(!wxFileName::DirExists(_T("CamTrackerVideo")))
                    {
                        if(!wxFileName::Mkdir(_T("CamTrackerVideo")))
                        {
                            wxMessageBox(_T("Fail to build directory CamTrackerVideo!"),_T("Error"));
                        }
                    }
                    wxFileName::SetCwd(_T("D:\\CamTrackerVideo"));
                    if(!wxFileName::DirExists(str4date))
                    {
                        if(!wxFileName::Mkdir(str4date))
                        {
                            wxMessageBox(_T("Fail to build directory by date!"),_T("Error"));
                        }
                    }
                    wxString name2save=str4dir+_T("\\")+str4time+".avi";
                    std::string Cname2save = std::string(name2save.mbc_str());      //exchange wxString to std::string
                    writer.open(Cname2save,CV_FOURCC('M','J','P','G'),FPS,Size(FRAME_WIDTH,FRAME_HEIGHT));
                    if(!writer.isOpened())
                    {
                        wxMessageBox(_T("Fail to open file!"),_T("Error"));
                    }
                    shouldCreate = false;
                    shouldAddItem = true;
                }
                writer << cameraFeed;
				}
				/*else{
                    writer.release();
                    shouldCreate = true;
				}*/

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
	else{
            if(writer.isOpened())
            {
                if(justObjFound)
                {
                    timer4save.Start(1000,true);
                    justObjFound = false;
                }
                else
                {
                    if(timer4save.IsRunning())
                    {
                        writer << cameraFeed;
                    }
                    else
                    {
                        writer.release();
                        shouldCreate = true;
                    }
                }
            }
	}
}

void ObjectTracking::tracking(Mat &frame)
{
    //store image to matrix
    icap.read(cameraFeed);
    FRAME_HEIGHT = icap.get(CV_CAP_PROP_FRAME_HEIGHT);
	FRAME_WIDTH  = icap.get(CV_CAP_PROP_FRAME_WIDTH);
    //convert frame from BGR to HSV colorspace
    cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
    //filter HSV image between values and store filtered image to
    //threshold matrix
    inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
    //perform morphological operations on thresholded image to eliminate noise
    //and emphasize the filtered object(s)
    if(useMorphOps)
        morphOps(threshold);
    //pass in thresholded frame to our object tracking function
    //this function will return the x and y coordinates of the
    //filtered object
    if(trackObjects)
    {
        trackFilteredObject(x,y,threshold,cameraFeed);
    }
    frame = cameraFeed;
}
