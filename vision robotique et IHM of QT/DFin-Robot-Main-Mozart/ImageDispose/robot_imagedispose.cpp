#include "robot_imagedispose.h"


Robot_ImageDispose::Robot_ImageDispose()
{
    recving_img = false;
    disposing = false;
    copying_res = false;

    //===========color filter================
    obj_enable = false;
    filter_switcher = 1;

    histsize_rgb[0]=30;histsize_rgb[1]=30;histsize_rgb[2]=30; //rgb
    histsize_hs[0]=30;histsize_hs[1]=32;//hs
    channels_rgb[0]=0;channels_rgb[1]=1;channels_rgb[2]=2;
    channels_hs[0]=0;channels_hs[1]=1;
    range[0]=0; range[1]=255;

}
Robot_ImageDispose::~Robot_ImageDispose()
{
    img.release();
}

bool Robot_ImageDispose::getImage(Mat _img)
{
    if ( recving_img || disposing )
        return false;

    recving_img = true;

    this->img = _img.clone();

    recving_img = false;

    disposeImage();

    return true;
}

bool Robot_ImageDispose::result(ImageDisposeResult &_res)
{
    if (copying_res)
        return false;

    _res = *(this->res);
    return true;
}
void Robot_ImageDispose::disposeImage()
{
    disposing = true;
    while(true)
    {
        if (!recving_img) //类似于锁机制，接收图片过程中不能处理图片
            break;
    }
    colorFilter(img);//反向投影
    switch(filter_switcher)
    {
    case 0: img = filted_img_rgb; break;
    case 1: img = filted_img_hs; break;
    case 2: img = filted_synthese; break;
    }

    dispose();

    disposing = false;

    copying_res = true;
    *res = *temp_res;
    copying_res = false;
}
//===============color filter===================
bool Robot_ImageDispose::setDetectObj(Mat &_sample)//设置检测目标
{
    if (!_sample.data)
        return false;

    obj_enable = true;
    sampleHistExtract(_sample);
    return true;
}

void Robot_ImageDispose::sampleHistExtract(Mat &input)
{

    float h_range[2];
    float s_range[2];
    h_range[0] = 0;
    h_range[1] = 180;
    s_range[0] = 0;
    s_range[1] = 255;

    const float* ranges_rgb[] =
    {range,range,range};
    calcHist(&input,1,channels_rgb,Mat(),sample_hist_rgb,
             2,histsize_rgb,ranges_rgb,true,false);
			 // channels_rgb=[0,1,2]表明三个通道都要使用
			 //dims=2 表明结果使用二维表示
			 //histsize_rgb=[30,30,30]表明每个通道要30个柱
			 //ranges_rgb=[[0,255][0,255][0,255]]表明每个柱中数据范围都是全范围
    normalize(sample_hist_rgb,sample_hist_rgb,
              0,255,NORM_MINMAX,-1,Mat());


    const float* ranges_hs[3] =
    {h_range,s_range};
    cvtColor(input,input,cv::COLOR_BGR2HSV);
    calcHist(&input,1,channels_hs,Mat(),sample_hist_hs,
             2, histsize_hs, ranges_hs, true, false);
    normalize(sample_hist_hs,sample_hist_hs, 0,255,NORM_MINMAX,-1,Mat());
}
void Robot_ImageDispose::colorFilter(Mat &input)
{
    int _threshold = 55;
    int s_threshold = 15;
    float h_range[2];
    float s_range[2];
    h_range[0] = 0;
    h_range[1] = 180;
    s_range[0] = 0;
    s_range[1] = 255;
    std::vector<Mat> hsv_channels;
    const float* ranges_rgb[] = {range,range,range};

    //rbg
    calcBackProject(&input,1,channels_rgb,sample_hist_rgb,
                    filted_img_rgb,ranges_rgb,1,true);
					//
    threshold(filted_img_rgb,filted_img_rgb,_threshold,255,cv::THRESH_BINARY);
    medianBlur(filted_img_rgb,filted_img_rgb,5);
    medianBlur(filted_img_rgb,filted_img_rgb,5);

    //hsv
    const float* ranges_hs[3] =
    {h_range,s_range};


    cvtColor(input,input,cv::COLOR_BGR2HSV);
    split(input,hsv_channels);
    threshold(hsv_channels[1],hsv_channels[1],s_threshold,255,cv::THRESH_BINARY);
    filted_img_hs = Scalar::all(0);
    calcBackProject(&input,1,channels_hs,sample_hist_hs,
                    filted_img_hs,ranges_hs,1,true);
    bitwise_and(filted_img_hs,hsv_channels[1],filted_img_hs);
    //threshold(filted_img_hs,filted_img_hs,_threshold,255,CV_THRESH_BINARY);
    medianBlur(filted_img_hs,filted_img_hs,5);
    medianBlur(filted_img_hs,filted_img_hs,5);
    Mat element(7,7,CV_8U,cv::Scalar(1));
    dilate(filted_img_hs,filted_img_hs,element);
    //    cvtColor(filted_img_hs,filted_img_hs,CV_HSV2BGR);
    //    cvtColor(filted_img_hs,filted_img_hs,CV_BGR2GRAY);
    //    threshold(filted_img_hs,filted_img_hs,_threshold,255,cv::THRESH_BINARY);
    //    medianBlur(filted_img_hs,filted_img_hs,5);
    //    medianBlur(filted_img_hs,filted_img_hs,5);
    /******去掉饱和度低于某阈值的点**************/
    //    imshow("filted_img_hs",filted_img_hs);
    //    vector<Mat> v;
    //    split(input,v);
    //    threshold(v[1],v[1],190,255,THRESH_BINARY);
    //    bitwise_and(filted_img_hs,v[1],filted_img_hs);
    //    imshow("aws",filted_img_hs);
    filted_synthese = filted_img_rgb & filted_img_hs;
    //imshow("filted_img_synthese",filted_synthese);
}

