#ifndef SAMPLEIMAGEDISPOSE_H
#define SAMPLEIMAGEDISPOSE_H

#include "robot_imagedispose.h"

/*
 *  在工程文件夹下Robot_ImageDispose_global.h中
 *  有Log函数及Wrapper类
 */

//继承ImageDisposeResult类,实现自定的图像处理结果类型
class SampleResult : public ImageDisposeResult
{
public:

    //处理结果，应该放在public域
    /*! @todo
      添加处理结果
      */
    QString s;
    //处理结果
public:
    //重载operator=, 请注意, 需要将参数_res向下类型转换为
    //SampleResult类
    virtual void operator=(ImageDisposeResult &_res)
    {
        //向下类型转换
        SampleResult *p_res = dynamic_cast<SampleResult*>(&_res);
        //向下类型转换

        //处理结果复制(请注意深浅拷贝问题)
        this->s = p_res->s;
        //处理结果复制
    }
    virtual QString toString() { return "that's a sample"; }
};

//继承Robot_ImageDispose类,实现自定的图像处理模块
class SampleImageDispose : public Robot_ImageDispose
{
public:
    //实现构造函数, 必须将父类中的res属性
    //赋值与自定的SampleImageDispose对象指针
    SampleImageDispose() { res = new SampleResult; }
    ~SampleImageDispose() { delete res; }
    //实现构造函数,析构函数
protected:
    //重载图像处理函数
    virtual void dispose()
    {
        //同样的,释放上次处理申请的内存
        //并给temp_res赋值SampleResult对象,并进行类型转换
        delete temp_res;
        temp_res = new SampleResult;
        SampleResult *p_res =
                dynamic_cast<SampleResult*>(temp_res);
        //
        /*! @todo
          添加图像处理部分,图像指针IplImage *img
          结果添加至*p_res
          */
        //示例添加结果
        p_res->s = "hello Robot";
        //示例添加结果
    }
    virtual void dispose2(){}

};

#endif // SAMPLEIMAGEDISPOSE_H
