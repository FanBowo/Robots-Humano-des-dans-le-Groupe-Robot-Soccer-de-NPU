#include "samplestrategist.h"

SampleStrategist::SampleStrategist()
{
}

bool SampleStrategist::preprocessing()
{
    if (obj_inputs.size() == 1)
        ib = dynamic_cast<Image_BallCentre*>(obj_inputs[0]);
    return true;
}

void SampleStrategist::makestrategy(bool block,bool mover)
{
    actions.push_back(MOVE_FORWARD);
}
