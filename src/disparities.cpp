#include "disparities.h"

int getDisparity(Mat g1, Mat g2, Mat &disp)
{
    if (!g1.data || !g2.data)
    {
        printf("Invalid Mat\n");
        return 0;
    }

    if (g1.size() != g2.size())
    {
        printf("Different Image sizes\n");
        return 0;
    }

    Mat disp16;
    StereoSGBM sbm;

    sbm.SADWindowSize = 7;
    sbm.numberOfDisparities = 128
    sbm.preFilterCap = 4;
    sbm.minDisparity = -39;
    sbm.uniquenessRatio = 9;
    sbm.speckleWindowSize = 180;
    sbm.speckleRange = 2;
    sbm.disp12MaxDiff = 20;
    sbm.fullDP = false;
    sbm.P1 = 600;
    sbm.P2 = 2400;

    sbm(g1, g2, disp16);
    normalize(disp16, disp, 0, 255, CV_MINMAX, CV_8U);

    if (disp.size() != g1.size())
    {
        return 0;
    }

    return 1;
}