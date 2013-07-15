#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int vignette(Mat img, Mat& retVal, Point p1);
int vignette(Mat img, Mat& retVal, Point p1, int rad);
int sepia(Mat img, Mat &retVal);