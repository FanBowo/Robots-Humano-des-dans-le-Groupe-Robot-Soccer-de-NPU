#ifndef STRATEGY_POINTDC_H
#define STRATEGY_POINTDC_H
#include "robot_strategist.h"
#include "./ImageDispose/hawkseye.h"
#include "./ImageDispose/hyperpoint.h"

class strategy_pointdc : public Robot_Strategist
{
public:
    strategy_pointdc();
protected:
    virtual bool preprocessing();
    virtual void makestrategy(bool block,bool mover);
    void fstart();//开始处理，判断球的整体位置
    void hereturn();//直头动作
    void shootadjuster();//近距离调整
    void lostNsearch();//迷失处理


private:
    HyperPoint* Ball;
    cv::Point pre_pt;
    bool start;
    int headState;
    /*HEADSTATE:      13 11 12
     *                3  1  2
                     -3 -1 -2*/
    int ballstate;//-1为左 1为右
    int x,y;
    int losttimes;//0为按球上次出现的X方向偏向，1为y方向，2-7为转头一圈，8-13为左转，9-...为右转
    int startlt;//开始时迷失次数
    bool isclose;
    bool islost;
    int isturn;//0表示不需要调，2时表示球在正中间，1在特别偏右，-1在特别偏左
    bool gateready;
    //绕球走
    int cstate;//0为（初始 || !isclose)，1为看门，2为看球，3为调整，4为踢球
    bool ballready;//判断cstate1是由cstate2进入的还是由lostNsearch进入的
};

#endif // STRATEGY_POINTDC_H
