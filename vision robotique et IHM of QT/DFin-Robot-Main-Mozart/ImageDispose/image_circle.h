#ifndef IMAGE_CIRCLE_H
#define IMAGE_CIRCLE_H

#include "robot_imagedispose.h"


class Image_BallCentre:public ImageDisposeResult
{
    //�̳�ImageDisposeResult��,ʵ���Զ���ͼ����������
public:
    cv::Point ball;
    std::vector<Vec3f> circles;
    bool valid;

public:
    //����operator=, ��ע��, ��Ҫ������_res��������ת��Ϊ
    virtual void operator=(ImageDisposeResult &_res);
    virtual QString toString();
};
//�̳�Robot_ImageDispose��,ʵ���Զ���ͼ����ģ��
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
        ����ͼ����������Ҫ��������������ܣ�
        (1)����getBallFocal()������ĵ�����
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
