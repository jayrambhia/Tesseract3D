#include "segmentation.h"

int segmentForeground(Mat img, Mat& foreground, Mat& background)
{
    if (!img.data)
    {
        printf("Invalid Mat\n");
        return 0;
    }

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat drawing, kernel;
    Mat temp;
    int size=3;

    drawing = Mat::zeros(img.size(), CV_8UC3);
    findContours(img.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    for (int i=0; i<contours.size(); i++)
    {
        if (contourArea(contours[i]) > 15000)
        {
            drawContours(drawing, contours, i, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy, 0, Point());
        }
    }

    kernel = getStructuringElement(MORPH_ELLIPSE, Size(2*size+1, 2*size+1), Point(size, size));
    erode(drawing, drawing, kernel, Point(-1, -1), 2);
    dilate(drawing, temp, kernel, Point(-1, -1), 1);

    drawing = Mat::zeros(img.size(), CV_8UC3);
    contours.clear();
    hierarchy.clear();

    cvtColor(temp, temp, CV_BGR2GRAY);
    findContours(temp.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    for (int i=0; i<contours.size(); i++)
    {
        if (contourArea(contours[i]) > 15000)
        {
            drawContours(drawing, contours, i, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy, 0, Point());
        }
    }

    dilate(drawing, drawing, kernel, Point(-1, -1), 1);

    drawing.copyTo(foreground);
    temp = Mat::ones(img.size(); CV_8UC3);
    temp = 255*temp;
    subtract(temp, foreground, background);

    if (foreground.size() != img.size())
    {
        return 0;
    }
    return 1;
}

int threshDisparity(Mat disp, Mat& retVal, Point p1, int range)
{
    if (!disp.data)
    {
        printf("Invalid input image\n");
        return 0;
    }
    int disval;
    double min, max;
    Point minP, maxP;
    minMaxLoc(img, &min, &max, &minP, &maxP);

    disval = img.at<uchar>(p1.y, p1.x);
    range = disval/20;

    inRange(img, disval - range, disval + range, retVal);
    medianBlur(retVal, retVal, 9);

    if (retVal.size() != disp.size())
    {
        return 0;
    }
    return 1;   
}