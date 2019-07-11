#include "mainwindow.h"
# include "Strategists/Strategy_Ball.h"
Strategy_Ball::Strategy_Ball()
{
    state = 0;
    position = 0;
    x = 0;
    y = 0;
    exe = 0;
    Ball = NULL;
    once=1;
    no_ball_times=0;
    search_area=0;
    radius=0;
    counters=0;
    xiuzheng=0;
    keep_kick=0;
    kick_foot= STAND_POSE;
    back = true;


    //moveX=-10;
    moveX=10;
    moveY=-5;
//x:
    a=160+moveX;
    b=260+moveX;
    c=360+moveX;
    d=460+moveX;
    left=200+moveX;
    right=420+moveX;
    Cleft=300+moveX-8;
    Cright=320+moveX+8;
//y:
    buffer1=410+moveY;
    buffer2=440+moveY;
    e=270;
    f=380;
//distance:
    left=left-20;
    //right=right+20;
    right=right+40;
    Cleft=Cleft+0;
    Cright=Cright+10;
}
Strategy_Ball ::~Strategy_Ball()
{

}
bool Strategy_Ball:: preprocessing()
{

    Ball = dynamic_cast<HawksPrey*>(obj_inputs[0]);
    if (Ball==NULL)
    {

        return false;
    }
    else
        return true;
}
void Strategy_Ball::makestrategy(bool block,bool mover)
{
    actions.clear();//gu....
    exe = 1;

    if(Ball->valid == true)
    {
        x = Ball->circle_centre.x;
        y = Ball->circle_centre.y;
        radius=Ball->circle_radius;

    }
    else
    {
        x = 0;
        y = 0;
        radius=0;
    }
    if(state==0)
    {
        if(radius<20)
        {
            x=0;
            y=0;
        }
        if(x == 1 && y == 1)
        {
            x--;
            y--;

        }
        if(y>200)
        {
            state=1;//避免触球
            if(x<a) position=1;
            else if(x<b) position=2;
            else if (x<c)position=3;
            else if(x<d) position=4;
            else position=5;
        }
    }


    if(state==0 && exe == 1)
    {
        if((x == 0 && y == 0)||(radius<=20))
        {
              if(back) {
                  actions.push_back(MOVE_BACK);
                  back = false;

              }
              else{
                  return;
              }

        }
        if(x > 0 && x <a)
        {
            position = 1;
            state = 1;
            exe = 0;
            actions.push_back(MOVE_LEFT);
            actions.push_back(MOVE_LEFT);
            actions.push_back(MOVE_FORWARD_LITTLE);
             actions.push_back(STAND_POSE);

        }
        if(x > a && x < b)
        {

            position = 2;
            state = 1;
            exe = 0;
            actions.push_back(MOVE_LEFT);
            actions.push_back(MOVE_FORWARD_LITTLE);
             actions.push_back(STAND_POSE);
        }
        if(x >= b && x <= c)
        {

            state = 1;
            position = 3;
            exe = 0;
            actions.push_back(MOVE_FORWARD_LITTLE);
             actions.push_back(STAND_POSE);
        }
        if(x > c && x <= d)
        {

            state = 1;
            position = 4 ;
            exe = 0;
            actions.push_back(MOVE_RIGHT);
            actions.push_back(MOVE_FORWARD_LITTLE);
             actions.push_back(STAND_POSE);
        }
        if(x > d && x <= 640)
        {
            state = 1;
            position = 5 ;
            exe = 0;
            actions.push_back(MOVE_RIGHT);
            actions.push_back(MOVE_RIGHT);
            actions.push_back(MOVE_FORWARD_LITTLE);
            actions.push_back(STAND_POSE);
        }
    }
    if(state == 1 &&  exe == 1)
    {
        if(counters>=2)
        {
            search_area=1;

        }

        if(x > 0 && x < b)
        {

            state = 1;
            exe = 0;
            actions.push_back(MOVE_LEFT_LITTLE);
            actions.push_back(STAND_POSE);
        }
        if(x >= b && x <= c)
        {

            state = 1;
            exe = 0;
            if(y > 0 && y < e)
            {

                actions.push_back(MOVE_FORWARD_TINY);
                 actions.push_back(STAND_POSE);
            }
            if(y >= e && y <= f)
            {
                if(x>=b&&x<(b+c)/2)
                {
                    actions.push_back(MOVE_RIGHT_LITTLE);

                }
                else
                {
                    actions.push_back(MOVE_LEFT_LITTLE);
                }
                actions.push_back(MOVE_FORWARD_TINY);
                actions.push_back(STAND_POSE);
                state=2;
                exe=0;

            }
            if(y >= f && y <= 480)
            {
                state = 2;
                 actions.push_back(STAND_POSE);
            }
        }
        if(x > c && x <= 640)
        {

            state = 1;
            exe = 0;
            actions.push_back(MOVE_RIGHT_LITTLE);
             actions.push_back(STAND_POSE);
        }
        if(x == 0 && y == 0)
        {
            state = 1;
            exe = 0;
            actions.push_back(MOVE_BACK);
             actions.push_back(STAND_POSE);

        }
    }

    if(2 == state && exe == 1 && once==0)
    {
        if(x < left && x >=0)
        {
            exe = 0;
            state = 2;
            actions.push_back(MOVE_LEFT_LITTLE);
             actions.push_back(STAND_POSE);
        }
        if(x >= left && x <= right)
        {
            if(x >= Cleft && x<= Cright)
            {
                actions.push_back(MOVE_RIGHT_LITTLE);
                actions.push_back(STAND_POSE);
            }
            else
            {
                if(y >= 0 && y <= buffer1)
                {
                    actions.push_back(MOVE_FORWARD_TINY);
                    actions.push_back(STAND_POSE);
                    exe = 0;
                }
                if( y > buffer1 && y <= buffer2)
                {
                    actions.push_back(MOVE_FORWARD_TINY);
                     actions.push_back(STAND_POSE);
                }
                if(y > buffer2 && y <= 480 )
                {

                    if(x >= left && x <= Cleft)
                    {
                        actions.push_back(KICK_LEFT);
                        kick_foot=KICK_LEFT;

                    }

                   else if(x >= Cright && x<= right)
                    {
                        actions.push_back(KICK_RIGHT);
                        kick_foot = KICK_RIGHT;
                    }
                    exe = 0;
                    state = 3;
                }
            }
        }
        if(x > right && x < 640)
        {
            exe = 0;
            state = 2;
            actions.push_back(MOVE_RIGHT_LITTLE);
             actions.push_back(STAND_POSE);
        }
    }

    if(state==3&& exe == 0)
    {
        if(keep_kick==1)
        {
            actions.push_back(STAND_POSE);
            actions.push_back(kick_foot);
            keep_kick = 0;
        }
        else
        {

            actions.push_back(STAND_POSE);
            actions.push_back(MOVE_FORWARD_LITTLE);
            keep_kick = 1;
        }
        exe = 0;
        state = 3;

    }
    cout<<radius<<endl;
}
