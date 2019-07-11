#ifndef STRIKER3V3_H
#define STRIKER3V3_H
#include "Strategists/robot_strategist.h"
#include "./ImageDispose/hawkseye.h"
class striker3v3 : public Robot_Strategist
{
public:
    striker3v3();

protected:
    virtual bool preprocessing();
    virtual void makestrategy(bool block,bool mover);
    void headreturn();
    void searchBall();
    void roundsearch();

private:
    HawksPrey* Ball;
    int x,y;
    int headState;
    /*HEADSTATE:      3  1  2
                     -3 -1 -2*/
    bool hereturn;
    int losttimes;
    cv::Point pre_pt;
};

#endif // STRIKER3V3_H
