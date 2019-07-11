#ifndef IMAGE_CIRCLE_H
#define IMAGE_CIRCLE_H

#include "robot_imagedispose.h"


class Image_BallCentre:public ImageDisposeResult
{
    //继承ImageDisposeResult类,实现自定的图像处理结果类型
public:
    cv::Point ball;
    std::vector<Vec3f> circles;
    bool valid;

public:
    //重载operator=, 请注意, 需要将参数_res向下类型转换为
    virtual void operator=(ImageDisposeResult &_res);
    virtual QString toString();
};
//继承Robot_ImageDispose类,实现自定的图像处理模块
class Image_Circle : public Robot_ImageDispose
{
public:
    Image_Circle() { res = new Image_BallCentre;
                    r_ball_min = 1; r_ball_max = 80;}
    ~Image_Circle() { delete res; }
    void setBallRadius(int _min, int _max)
    { r_ball_min = _min; r_ball_max = _max;}

protected:

    /*
        重载图像处理函数，主要完成下面三个功能，
        (1)调用getBallFocal()获得球心的坐标
    */
    virtual void dispose();
    virtual void dispose2();

private:
    void drawCircle(Mat &src, std::vector<Vec3f> &circles);
    void findCircle(Mat &mat, std::vector<Vec3f> &circles);
    void showResult()
    {
        Mat result;
        cvtColor(img,result,cv::COLOR_GRAY2BGR);
        drawCircle(result,crcls);
        imshow(show_window_name, result);
    }

private:
    std::vector<Vec3f> crcls;
    int r_ball_min, r_ball_max;
};


#endif // IMAGE_CIRCLE_H
