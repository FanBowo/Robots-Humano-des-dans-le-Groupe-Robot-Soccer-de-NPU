#ifndef STRATEGY_GUARD_H
#define STRATEGY_GUARD_H

#include "robot_strategist.h"
#include "../ImageDispose/image_circle.h"
#include "../ImageDispose/hawkseye.h"
class  Strategy_Guard : public Robot_Strategist{
public:
    Strategy_Guard();
    ~Strategy_Guard();
protected:
    virtual bool preprocessing();
    virtual void makestrategy(bool block,bool mover);
    HawksPrey* Ball_Guard;
    int state ;
    int x_old;
    int x_new;
    int y_old;
    int y_new;
    int position_Guard;
};

#endif // STRATEGY_GUARD_H
