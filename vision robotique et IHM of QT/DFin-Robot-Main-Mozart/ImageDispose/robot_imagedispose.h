#ifndef ROBOT_IMAGEDISPOSE_H
#define ROBOT_IMAGEDISPOSE_H

/*
 *  Roboteam 图像处理框架v1.2
 *  修改内容：
 *      ImageDisposeResult增加 toString 方法
 *
 *
 *  Roboteam 图像处理框架v1.1
 *  修改内容：
 *      输入图片接口 参数类型 IplImage* -> Mat
 *      增加颜色滤波功能
 *
 *  Roboteam 图像处理框架v1.0
 *
 *  基于模块化的原则,该框架将图像处理部分从其他模块中独立出来
 *  并提供两个接口：getImage(), result()用于输入图像和输出结果
 *
 *  框架使用：
 *      请先在.pro文件内修改opencv路径
 *
 *      在工程内添加文件,文件请中继承
 *      class ImageDisposeResult;
 *      class Robot_ImageDispose;
 *      并在ImageDisposeResult的继承类中
 *      添加图像处理结果变量，重载operator=()方法
 *      在Robot_ImageDispose的继承类中
 *      添加构造函数，重载void dispose()方法
 *                  请见工程文件夹中sampleImageDispose.h
 *
 *  注意：如果只需要编写图像处理部分，请不要修改框架
 *  Author: Ding(leeleedzy@gmail.com)
 *      框架改进方向(disposeImage()应变为槽函数)
 */
#include "Robot_ImageDispose_global.h"

class ImageDisposeResult
{
public:
    virtual void operator=(ImageDisposeResult &res) = 0;
    virtual QString toString() = 0;
};

class  Robot_ImageDispose
{
public:
    Robot_ImageDispose();//构造函数，初始化两个滤波器的阈值
    ~Robot_ImageDispose();
    bool getImage(Mat _img);//获取图片并将图片存入类的 img 成员
    bool result(ImageDisposeResult &res );

protected:
    void disposeImage();
    virtual void dispose()=0;

protected:
    Mat img;
    ImageDisposeResult *temp_res;
    ImageDisposeResult *res;
private:
    bool recving_img;//flag of receiving imag 
    bool disposing;//flag of processing img 
    bool copying_res;

//=============color filter================
public:
    //设定颜色滤波器的滤波对象
    //请使用前设定
    bool setDetectObj(Mat &_sample);
    void setShow(bool m_show) { show_mid_result = m_show; }
    void setResultSwitcher(int m_switcher) {
        if (m_switcher==0||
                m_switcher==1||
                m_switcher==2)
            filter_switcher = m_switcher;
    }
    void setWindowName(const char* _name) { show_window_name = _name; }
    void setseq(int s){seq=s;}

protected:
    bool obj_enable;

    Mat sample_hist_hs;     //输入样本的h-s直方图
    Mat sample_hist_rgb;    //输入样本的rgb直方图

    Mat filted_img_rgb;     //rgb滤波结果
    Mat filted_img_hs;      //h-s滤波结果
    Mat filted_synthese;    //两者滤波综合结果

    int filter_switcher;         //使用哪一滤波结果
    bool show_mid_result;   //是否显示图像
    const char* show_window_name;   //显示图像窗口名
    int seq;

private:
    //进行滤波的参数
    int histsize_rgb[3],histsize_hs[2];
    float range[2];
    int channels_rgb[3], channels_hs[2];

    //滤波的功能函数
    void sampleHistExtract(Mat &input);
    void colorFilter(Mat &input);
//==============color filter===============
};


#endif // ROBOT_IMAGEDISPOSE_H
