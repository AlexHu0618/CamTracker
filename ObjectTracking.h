/***************************************************************
 * Name:      ObjectTracking.h
 * Purpose:   Declare class to track specified object
 * Author:    Alex (jthu4alex@163.com)
 * Created:   2016-07-13
 * Copyright: Alex (www.gzrobot.net)
 * License:
 **************************************************************/

#ifndef OBJECTTRACKING_H
#define OBJECTTRACKING_H

#include <wx/timer.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class ObjectTracking
{
    public:
        ObjectTracking(VideoCapture cap);
        ~ObjectTracking(void){};
        void tracking(Mat &frame);

        bool shouldAddItem;

    private:
        //some boolean variables for different functionality within this
        //program
        bool trackObjects;
        bool useMorphOps;
        //Matrix to store each frame of the webcam feed
        Mat cameraFeed;
        //matrix storage for HSV image
        Mat HSV;
        //matrix storage for binary threshold image
        Mat threshold;
        //x and y values for the location of the object
        int x, y;
        VideoCapture icap;

        wxTimer timer4save;
        static const long ID_TIMER2;

        void drawObject(int x, int y,Mat &frame);
        void morphOps(Mat &thresh);
        void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);
};
#endif
