#include "mainwindow.h"
# include "Strategists/Strategy_Guard.h"
Strategy_Guard:: Strategy_Guard()
{
    state = 0;
}
Strategy_Guard ::~ Strategy_Guard()
{

}
bool Strategy_Guard :: preprocessing()
{

    Ball_Guard = dynamic_cast<HawksPrey*>(obj_inputs[0]);
    if (Ball_Guard==NULL)
    {
        return false;
    }
    else
        return true;

}
void Strategy_Guard::makestrategy(bool block,bool mover)
{
    if(Ball_Guard->valid == true)
    {
        if(0 == state )
        {
            x_old = Ball_Guard->circle_centre.x;
            y_old = Ball_Guard->circle_centre.y;
            x_new = Ball_Guard->circle_centre.x;
            y_new = Ball_Guard->circle_centre.y;
            state++;
        }
        if(state == 1 )
        {
            x_new = Ball_Guard->circle_centre.x;
            y_new = Ball_Guard->circle_centre.y;

        }
    }
    if((x_old - x_new) > 120)
    {
        //actions.push_back(MOVE_RIGHT);

        actions.push_back(MOVE_LEFT);
        actions.push_back(TURN_LEFT);
        actions.push_back(END);
    }
    if(x_old - x_new < -120)
    {
        actions.push_back(MOVE_RIGHT);
        actions.push_back(TURN_RIGHT);
        actions.push_back(END);
    }
    if(x_old - x_new < 100 && x_old - x_new > -100)
    {
        actions.push_back(STAND_POSE);
    }
    qDebug()<<"old_position"<<x_old<<"  "<<y_old;
    qDebug()<<"new_positon"<<x_new<<" "<<y_new;
}
