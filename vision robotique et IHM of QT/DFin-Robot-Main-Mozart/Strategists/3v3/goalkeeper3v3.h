#ifndef GOALKEEPER3V3_H
#define GOALKEEPER3V3_H
#include "../robot_strategist.h"
#include "./ImageDispose/hawkseye.h"
class goalkeeper3v3 : public Robot_Strategist
{
public:
    goalkeeper3v3();
    void searchBall();
    void headreturn();

protected:
    virtual bool preprocessing();
    virtual void makestrategy(bool block,bool mover);

private:
    HawksPrey* Ball;
    int x,y;
    int headState;
    /*HEADSTATE:     13 11 12
     *                3  1  2
                     -3 -1 -2*/
    bool seball;
    bool hereturn;
    int losttimes;
    cv::Point pre_pt;
};

#endif // GOALKEEPER3V3_H
