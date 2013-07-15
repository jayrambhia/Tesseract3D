#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
#include <stdio.h>
using namespace cv;
using namespace std;

int segmentForeground(Mat img, Mat& foreground, Mat& background);
int threshDisparity(Mat disp, Mat& retVal, Point p1, int range);