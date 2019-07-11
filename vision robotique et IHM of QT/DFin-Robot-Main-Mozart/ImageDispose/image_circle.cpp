#include "image_circle.h"

void Image_Circle::findCircle(Mat &src, std::vector<Vec3f> &circles)
{
    Mat src_gray=src;
    //GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
    HoughCircles( src_gray, circles, cv::HOUGH_GRADIENT, 2, src_gray.rows/4, 150, 50,
                  r_ball_min, r_ball_max );
				  //circles是三通道 前两通道用于存储圆心位置，第三通道存储半径
}

void Image_Circle::drawCircle(Mat &src, std::vector<Vec3f> &circles)
{
    if (circles.empty())
        return;

    for( size_t i = 0; i < 1; i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		//cvRound 计算最近的整数
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( src, center, 3, Scalar(0,0,255), -1, 8, 0 );
        // circle outline
        circle( src, center, radius, Scalar(0,255,0), 3, 8, 0 );
    }

    for( size_t i = 1; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

}

void Image_BallCentre:: operator=(ImageDisposeResult &_res)
{
    //向下类型转换
    Image_BallCentre *p_res = dynamic_cast<Image_BallCentre*>(&_res);
    //向下类型转换
    this->ball=p_res->ball;//球的坐标
    this->valid=p_res->valid;
    this->circles = p_res->circles;
}

QString Image_BallCentre::toString() {
    QString _temp;
    if (!valid)
        _temp = "not found";
    else
    {
        _temp = "x: "+QString::number(ball.x) + "y: "+QString::number(ball.y);
    }
    return _temp;
}

void Image_Circle:: dispose()
{

    //同样的,释放上次处理申请的内存
    //并给temp_res赋值SampleResult对象,并进行类型转换
    //delete temp_res;
    temp_res = new Image_BallCentre;
    Image_BallCentre *p_res =
            dynamic_cast<Image_BallCentre*>(temp_res);

    findCircle(img,crcls);

    if (crcls.size() == 0)
        p_res->valid = false;
    else
    {
        p_res->valid = true;
        p_res->ball.x = cvRound(crcls[0][0]);
        p_res->ball.y = cvRound(crcls[0][1]);
        p_res->circles = crcls;
    }

    if (show_mid_result)
        showResult();
}
void Image_Circle:: dispose2(){

}
