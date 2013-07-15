#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int radialBlur(Mat img, Mat& retVal, Point p1, bool grad);
int partialBlur(Mat img, Mat& retVal, Point p1, int radius);
int multiplePartialBlur(Mat img, Mat& retVal, Point p1, int radius);
int partialErodeBlur(Mat img, Mat mask, Mat& retVal);
int bokeh(Mat img, Mat disp, Mat& retVal);
int deFocus(Mat img, Mat& retVal, int size, int radius);
int deFocus8(Mat img, Mat& foreground, int size, int w, int h);
int gaussian(Mat img, Mat &retVal);