#include "filters.h"

int vignette(Mat img, Mat& retVal, Point p1)
{
    if (!img.data)
    {
        printf("Empty image\n");
        return 0;
    }

    Mat gaussmask, mask;
    float rad;

    gaussmask = Mat::zeros(img.rows, img.cols, CV_8UC3);
    rad = sqrt(img.cols*img.cols+img.rows*img.rows);
    circle(gaussmask, Point(p1.x, p1.y), rad-30, Scalar(255, 255, 255), -1);
    GaussianBlur(gaussmask, gaussmask, Size(51, 51), 5);
    addWeighted(img, 0.9, gaussmask, 0.08, 0, retVal);
    if (retVal.size() != img.size())
    {
        return 0;
    }
    return 1;
}

int vignette(Mat img, Mat& retVal, Point p1, int rad)
{
    if (!img.data)
    {
        printf("Empty image\n");
        return 0;
    }

    Mat gaussmask, mask;
    gaussmask = Mat::zeros(img.rows, img.cols, CV_8UC3);
    circle(gaussmask, Point(p1.x, p1.y), rad, Scalar(255, 255, 255), -1);
    GaussianBlur(gaussmask, gaussmask, Size(51, 51), 5);
    addWeighted(img, 0.9, gaussmask, 0.08, 0, retVal);
    if (retVal.size() != img.size())
    {
        return 0;
    }
    return 1;
}

int sepia(Mat img, Mat &retVal)
{
    if (!img.data)
    {
        printf("Invalid input\n");
        return 0;
    }

    Mat kernel;
    kernel = (cv::Mat_<float>(3,3) <<  0.272, 0.534, 0.131,
                                        0.349, 0.686, 0.168,
                                        0.393, 0.769, 0.189);
    transform(img, retVal, kernel);
    if (retVal.size() != img.size())
    {
        return 0;
    }
    return 1;
}