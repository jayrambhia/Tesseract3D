#include "blurs.h"

int radialBlur(Mat img, Mat& retVal, Point p1, bool grad)
{
    if (!img.data)
    {
        printf("No data in the imag\n");
        return 0;
    }

    int width, height, x, y, i;
    float center_x, center_y, blur, blurRadius;
    int iterations;
    Mat growMapx, growMapy;
    Mat shrinkMapx, shrinkMapy;
    Mat tmp1, tmp2;

    width = img.cols;
    height = img.rows;
    center_x = p1.x;
    center_y = p1.y;
    blurRadius = 0;
    iterations = 5;
    blur = 0.000035;
    if (!grad)
    {
        blur = 0.02;
        blurRadius = blur;
    }

    growMapx = Mat::zeros(height, width, CV_32F);
    growMapy = Mat::zeros(height, width, CV_32F);
    shrinkMapx = Mat::zeros(height, width, CV_32F);
    shrinkMapy = Mat::zeros(height, width, CV_32F);
    for(x = 0; x < width; x++) {
      for(y = 0; y < height; y++) {
        if (grad)
        {
            blurRadius = blur*sqrt((center_x-x)*(center_x-x)+(center_y-y)*(center_y-y));
        }
        growMapx.at<float>(y,x) = x+((x - center_x)*blurRadius);
        growMapy.at<float>(y,x) = y+((y - center_y)*blurRadius);
        shrinkMapx.at<float>(y,x) = x-((x - center_x)*blurRadius);
        shrinkMapy.at<float>(y,x) = y-((y - center_y)*blurRadius);
      }
    }
    
    for(i = 0; i < iterations; i++)  {
      remap(img, tmp1, growMapx, growMapy, CV_INTER_LINEAR); // enlarge
      remap(img, tmp2, shrinkMapx, shrinkMapy, CV_INTER_LINEAR); // shrink
      addWeighted(tmp1, 0.5, tmp2, 0.5, 0, retVal); // blend back to src
    }

    if (retVal.size() != img.size())
    {
        return 0;
    }
    return 1;
}

int partialBlur(Mat img, Mat& retVal, Point p1, int radius)
{
    if (!img.data)
    {
        printf("No data in the imag\n");
        return 0;
    }

    Mat blur, circmask, invcircmask, mask;

    GaussianBlur(img, blur, Size(15, 15), 0, 0);
    circmask = Mat::zeros(img.rows, img.cols, CV_8UC3);
    circle(circmask, p1, radius, Scalar(255, 255, 255), -1);
    invcircmask = Scalar(255, 255, 255) - circmask;
    bitwise_and(invcircmask, blur, invcircmask);
    GaussianBlur(img, blur, Size(5, 5), 0, 0);
    bitwise_and(circmask, blur, circmask);
    add(circmask, invcircmask, retVal);

    if (retVal.size() != img.size())
    {
        return 0;
    }

    return 1;
}

int multiplePartialBlur(Mat img, Mat& retVal, Point p1, int radius)
{
    if (!img.data)
    {
        printf("No data in the imag\n");
        return 0;
    }

    Mat blur, circmask, layer1;
    Mat circmask1, layer2;
    Mat circmask2, layer3;
    Mat invcircmask, layer4;

    GaussianBlur(img, blur, Size(5, 5), 0, 0);
    circmask = Mat::zeros(img.rows, img.cols, CV_8UC3);
    circle(circmask, p1, radius/3, Scalar(255, 255, 255), -1);
    bitwise_and(blur, circmask, layer1);

    GaussianBlur(img, blur, Size(7, 7), 0, 0);
    circmask1 = Mat::zeros(img.rows, img.cols, CV_8UC3);
    circle(circmask1, p1, 2*radius/3, Scalar(255, 255, 255), -1);
    circmask1 = circmask1 - circmask;
    bitwise_and(blur, circmask1, layer2);

    GaussianBlur(img, blur, Size(9, 9), 0, 0);
    circmask2 = Mat::zeros(img.rows, img.cols, CV_8UC3);
    circle(circmask2, p1, radius, Scalar(255, 255, 255), -1);
    invcircmask = Scalar(255, 255, 255) - circmask2;
    circmask2 = circmask2 - circmask1;
    circmask2 = circmask2 - circmask;
    bitwise_and(blur, circmask2, layer3);

    GaussianBlur(img, blur, Size(13, 13), 0, 0);
    bitwise_and(blur, invcircmask, layer4);
    add(layer1, layer2, retVal);
    add(layer3, retVal, retVal);
    add(layer4, retVal, retVal);

    if (retVal.size() != img.size())
    {
        return 0;
    }

    return 1;
}

int partialErodeBlur(Mat img, Mat mask, Mat& retVal)
{
    if (!img.data || !mask.data)
    {
        printf("No data in the imag\n");
        return 0;
    }

    Mat kernel, mask, blur, blurmask, erodemask, inverodemask;
    int size;

    size = 3;
    kernel = getStructuringElement(MORPH_ELLIPSE, Size(2*size+1, 2*size+1),
                                   Point(size, size));
    erode(mask, erodemask, kernel, Point(-1, -1), 2);

    inverodemask = Scalar(255, 255, 255) - dilmask;
    GaussianBlur(img, blur, Size(15, 15), 0, 0);
    bitwise_and(erodemask, blur, erodemask);

    GaussianBlur(img, blur, Size(5, 5), 0, 0);
    bitwise_and(inverodemask, blur, inverodemask);

    add(dilmask, invdilmask, retVal);

    if (retVal.size() != img.size())
    {
        return 0;
    }    
    return 1;
}

int bokeh(Mat img, Mat disp, Mat& retVal)
{
    if (!img.data || !disp.data)
    {
        printf("No data in the imag\n");
        return 0;
    }

    int i, j, disval, size, dia;
    float tSum;
    Mat cImg;
    Mat bImg, filter;

    size = 30;
    cImg = img.clone();
    bImg = img.clone();

    for(i=0; i<img.rows-size; i+=size/2)
    {
        for(j=0; j<img.cols-size; j+=size/2)
        {
            Mat subDisp = disp(Range(i, i+size), Range(j, j+size));
            Mat subImg = cImg(Range(i, i+size), Range(j, j+size));
            disval = sum(subDisp)[0]/(size*size);
            dia = 13 - disval/20;
            if (dia < 2)
                continue;
            filter = Mat::zeros(15, 15, CV_64F);
            circle(filter, Point(7, 7), dia/2+1, (1, 1, 1), -1);
            tSum = sum(filter)[0];
            filter = filter/tSum;
            filter2D(subImg, subImg, -1, filter);
            subImg.copyTo(bImg.colRange(j, j+size).rowRange(i, i+size));
        }
    }

    bImg.copyTo(retVal);

    if (retVal.size() != img.size())
    {
        return 0;
    }    
    return 1;
}

int deFocus(Mat img, Mat& retVal, int size, int radius)
{
    if (!img.data)
    {
        printf("No data in the imag\n");
        return 0;
    }

    Mat filter;
    float totalSum;

    filter = Mat::zeros(size, size, CV_64F);
    circle(filter, Point(size/2, size/2), radius, Scalar(1, 1, 1), -1);
    totalSum = sum(filter)[0];
    filter = filter/totalSum;
    filter2D(img, retVal, -1, filter);
    
    if (retVal.size() != img.size())
    {
        return 0;
    }    
    return 1;
}

int deFocus8(Mat img, Mat& foreground, int size, int w, int h)
{
    if (!img.data)
    {
        printf("No data in the imag\n");
        return 0;
    }

    Mat filter1, filter2, rotMat;
    float totalSum;

    filter1 = Mat::zeros(size, size, CV_64F);
    rectangle(filter1, Point(size/2-w/2, size/2-h/2),
              Point(size/2+w/2, size/2+h/2),
              Scalar(255, 255, 255), -1);
    rotMat = getRotationMatrix2D(Point(size/2, size/2), 45.0, 1.0);
    warpAffine(filter1, filter2, rotMat, Size(size, size));
    bitwise_or(filter1, filter2, filter1);
    filter1 = filter1/255.0;
    totalSum = sum(filter1)[0];
    filter1 = filter1/totalSum;
    filter2D(img, retVal, -1, filter1);
    
    if (retVal.size() != img.size())
    {
        return 0;
    }    
    return 1;
}

int gaussian(Mat img, Mat &retVal)
{
    if (!img.data)
    {
        printf("No data in the imag\n");
        return 0;
    }

    GaussianBlur(img, retVal, Size(13, 13), 13);

    if (retVal.size() != img.size())
    {
        return 0;
    }    
    return 1;
}