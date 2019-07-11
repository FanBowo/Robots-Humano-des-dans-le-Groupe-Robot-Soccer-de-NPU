#include "robot_strategist.h"


Robot_Strategist::Robot_Strategist()
{
}

bool Robot_Strategist::getDisposeResult(vector<ImageDisposeResult *> &m_inputs,bool block,bool mover)
{
    obj_inputs.clear();
    obj_inputs = m_inputs;

    actions.clear();

    if (!preprocessing())
        return false;

    makestrategy(block,mover);

    return true;
}
