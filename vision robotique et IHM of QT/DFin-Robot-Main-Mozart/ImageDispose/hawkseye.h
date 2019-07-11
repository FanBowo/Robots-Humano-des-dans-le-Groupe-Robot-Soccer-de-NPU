#ifndef HAWKSEYE_H
#define HAWKSEYE_H
#include <iostream>
#include "robot_imagedispose.h"
#include <vector>

using namespace std;

class HawksPrey : public ImageDisposeResult
{
public:
    virtual void operator=(ImageDisposeResult &_res);
    virtual QString toString();

public:
    bool valid;
    //以下结果用于存储检测所得的的位置形状信息
    cv::Point circle_centre;
    float circle_radius;
    cv::Point rect_centre;

    Rect        rect;
    RotatedRect min_rect;
};

class HawksEye : public Robot_ImageDispose
{
public:
    HawksEye() {res = new HawksPrey;}

protected:
    virtual void dispose();

private:
    vector<vector<Point> > contours;
    vector<vector<Point> > contours_poly;
    vector<Rect> boundRect;
    vector<Point2f>center;
    vector<float>radius;
    vector<RotatedRect> minRect;
    int most_possible;
    void showResult();
};

#endif // HAWKSEYE_H
