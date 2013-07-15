#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

int maskGray(Mat img, Mat& retVal);
int maskImage(Mat img, Mat& retVal);
int addFgBg(Mat foreground, Mat background, Mat &img);