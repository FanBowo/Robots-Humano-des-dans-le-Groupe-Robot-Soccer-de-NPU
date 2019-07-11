#ifndef STRATEGY_BALL_H
#define STRATEGY_BALL_H

#include "robot_strategist.h"
#include "../ImageDispose/image_circle.h"
#include "../ImageDispose/hawkseye.h"
class Strategy_Ball  : public Robot_Strategist{
public:
    Strategy_Ball();
    ~Strategy_Ball();
protected:
    virtual bool preprocessing();
    virtual void makestrategy(bool block,bool mover);
    HawksPrey* Ball;
    HawksPrey* Rect;
    int rect_y;
    int state ;
    int exe;
    int x;
    int y;
    int x0;
    int y0;
    int position;
    float radius;
    int counters;
    int xiuzheng;
    bool keep_kick;
    bool back;
    robot_action kick_foot;



    int no_ball_times;
    //bool block;
     bool once;
    int search_area;
     int a,b,c,d,e,f,left,Cleft,right,Cright,buffer1,buffer2;
     int moveX,moveY;

private:
      double height;
      double angle_of_pitch;
      double wide_angle;
      double l1,l2;


public:
      double calc_dis(double xx);
};


#endif // STRATEGY_BALL_H
