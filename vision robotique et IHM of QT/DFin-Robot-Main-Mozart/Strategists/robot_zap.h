#ifndef ROBOT_ZAP_H
#define ROBOT_ZAP_H

#include "robot_strategist.h"
#include "../ImageDispose/hawkseye.h"
#include <QTimer>

class ZapStrategist : public Robot_Strategist
{
public:
    ZapStrategist() {

        stage = 0;
        stituation = 0;
    }

protected:
    virtual bool preprocessing() {
        obstacle1 = dynamic_cast<HawksPrey*>(obj_inputs[0]);
        obstacle2 = dynamic_cast<HawksPrey*>(obj_inputs[1]);

        //if (obstacle1==NULL || obstacle2==NULL || obstacle3==NULL)
        //    return false;
        //else
        return true;
    }
    virtual void makestrategy(bool block,bool mover)
    {
        if(stage == 0)
        {
            if ( obstacle2->rect.area()>20)
            {
                if (obstacle1->rect.area() < 20)
                {
                    actions.push_back(KICK_LEFT);
                }
                else if (obstacle1->circle_centre.x > obstacle2->circle_centre.x)
                {
                    stituation = 1;
                    actions.push_back(MOVE_BACK);
                }
                else
                {
                    stituation = 3;
                    actions.push_back(MOVE_BACK2);
                }
            }
            else
            {
                stituation = 2;
                actions.push_back(MOVE_LEFT_LITTLE);
            }
            stage++;
        }
        if(actions.empty())
        {
            actions.push_back(STAND_POSE);
        }
    }


    QTimer timer;
    int stage;
    int stituation;
    HawksPrey *obstacle1, *obstacle2, *obstacle3;
};


#endif // ROBOT_ZAP_H
