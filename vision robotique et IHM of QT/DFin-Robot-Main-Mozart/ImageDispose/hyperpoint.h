#ifndef HYPERPOINT_H
#define HYPERPOINT_H
#include "robot_imagedispose.h"
#include "hawkseye.h"

class  HyperPoint : public ImageDisposeResult//用作点球传参数-_-
{
public:
    virtual void operator=(ImageDisposeResult &_res);
    virtual QString toString();
    void hyperres(ImageDisposeResult &res1, ImageDisposeResult &res2);

public:
    bool valid;

    cv::Point circle_centre;
    float circle_radius;

    cv::Point rect_centre;

    Rect        rect;
    RotatedRect min_rect;

    cv::Point gate_centre;
    bool gate_valid;
};

#endif // HYPERPOINT_H
