#ifndef SAMPLESTRATEGIST_H
#define SAMPLESTRATEGIST_H

#include "robot_strategist.h"
#include "../ImageDispose/image_circle.h"

class SampleStrategist : public Robot_Strategist
{
public:
    SampleStrategist();

protected:
    virtual bool preprocessing();
    virtual void makestrategy(bool block,bool mover);

    Image_BallCentre* ib;
};

#endif // SAMPLESTRATEGIST_H
