#include "masks.h"

int maskGray(Mat img, Mat& retVal)
{
    if (!img.data || !retVal.data)
    {
        printf("Invalid input image\n");
        return 0;
    }

    if (img.channels() != 1)
    {
        printf("Input image is not a gray image\n");
        return 0;
    }

    if (retVal.channels() == 1)
    {
        bitwise_and(img, retVal, retVal);
        if (retVal.size() != img.size())
        {
            return 0;
        }
        return 1;
    }

    Mat grayMerge;
    vector<Mat> gray;

    gray.push_back(img);
    gray.push_back(img);
    gray.push_back(img);
    merge(gray, grayMerge);

    bitwise_and(grayMerge, retVal, retVal);

    if (retVal.size() != img.size())
    {
        return 0;
    }
    return 1;
}

int maskImage(Mat img, Mat& retVal)
{
    if (!img.data || !retVal.data)
    {
        printf("Invalid input image\n");
        return 0;
    }

    if (img.channels() != retVal.channels())
    {
        pritnf("Number of channels should be same\n");
        return 0;
    }

    bitwise_and(img, retVal, retVal);
    if (retVal.size() != img.size())
    {
        return 0;
    }
    return 1;
}

int addFgBg(Mat foreground, Mat background, Mat &img)
{
    if (!foreground.data || !background.data)
    {
        printf("Invalid input image\n");
        return 0;
    }
    if (foreground.size() != background.size())
    {
        printf("Input images must have same sizes\n");
        return 0;
    }

    add(foreground, background, img);

    if (foreground.size() != img.size())
    {
        return 0;
    }
    return 1;
}