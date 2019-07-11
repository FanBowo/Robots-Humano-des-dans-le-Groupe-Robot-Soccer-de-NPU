#include "goalkeeper3v3.h"
#define VDIVISION 200//y坐标小于此值则视为近
#define MIDDLE 320
#define VMIDDLE 240
//x
#define ONESHIFT 160//一个正常平移
#define ONESSHIFT 160//一个小平移，---x小于此值时可踢
#define GATETHRE 100//头直时，球门x绝对值大于此值则符合条件
//y
#define DISTANTSS 300//抬头小于此值徐前进,大于时低头
#define DISTANTS 240//低头小于此值需前进 大于时isclose为真，且向前一小步可踢
//抬头小于此值时往前走，大于此值时低头
#define DISTANTC 280//低头小于此值则需小步前进 大于此值可以踢

goalkeeper3v3::goalkeeper3v3()
{
  headState=11;
  losttimes = 0;
  seball=false;
  hereturn=false;
  pre_pt.x = 0;
  pre_pt.y = 0;
}

bool goalkeeper3v3::preprocessing()
{
  Ball = dynamic_cast<HawksPrey*>(obj_inputs[0]);

  if (Ball==NULL)
  {
    return false;
  }
  else
    return true;
}

void goalkeeper3v3::makestrategy(bool block,bool mover)
{
  qDebug()<<endl;
  if(Ball->valid == false)//判断球
  {
    seball=true;
    qDebug()<<"headstate:"<<headState;
  }
  else
  {
    qDebug()<<"ball valid true";
    pre_pt.x = x;
    pre_pt.y = y;
    x = Ball->circle_centre.x-MIDDLE;
    y = Ball->circle_centre.y;
    qDebug()<<"x:"<<x<<" y:"<<y;
    qDebug()<<"headstate:"<<headState;

    losttimes=0;
    seball=false;

    if(hereturn==true)//头返回
    {
      headreturn();
      return;
    }

    if(headState == 3 || headState == -3 || headState == 13)
    {
      actions.push_back(MOVE_LEFT);
      if(x > ONESHIFT) hereturn=true;
      return;
    }

    else if(headState==2 || headState ==-2 || headState == 12)
    {
      actions.push_back(MOVE_RIGHT);
      if(x < -ONESHIFT) hereturn=true;
      return;
    }
  }

  if(Ball->circle_centre.x > 640 || Ball->circle_centre.x < 0)
  {
    seball = true;
  }
  if(Ball->circle_centre.y > 480 ||Ball->circle_centre.y < 0)
  {
    seball = true;
  }
  qDebug()<<"seball:"<<seball;

  if(seball == true)//找球
  {
    searchBall();
    losttimes++;
    return;
  }

  if(x > ONESHIFT)
  {
    actions.push_back(MOVE_RIGHT);
  }
  else if(x < -ONESHIFT)
  {
    actions.push_back(MOVE_LEFT);
  }

  else if(headState == 11)
  {
    if(y > DISTANTS)
    {
      actions.push_back(HEAD_DOWN);
      headState = 1;
    }
  }

  else if(headState == 1)//球在中间
  {
    if(y > DISTANTS)
    {
      actions.push_back(HEAD_DOWN);
      headState=-1;
    }
  }

  else if(headState == -1)
  {
    if(y > DISTANTS)
    {
      actions.push_back(STAND_POSE);
    }
    else if(y > DISTANTS)
    {
      actions.push_back(HEAD_UP);
      headState=1;
    }
  }
}

void goalkeeper3v3::searchBall()
{
  qDebug()<<"enter searhball";
  if(losttimes == 0)
  {
    if(pre_pt.y > DISTANTS)
    {
      if(headState > 0)
      {
        actions.push_back(HEAD_DOWN);

        if(headState > 10)
          headState -= 10;

        else if(headState > 0)
          headState = -headState;
      }
    }
    else if (pre_pt.y < DISTANTS)
    {
      if(headState < 10)
      {
        actions.push_back(HEAD_UP);
        if(headState < 0)
          headState = -headState;

        else if(headState < 10)
          headState += 10;
      }
    }
  }

  else if(losttimes == 1)
  {
    if(pre_pt.x > 2*ONESHIFT)
      actions.push_back(MOVE_RIGHT);

    else if(pre_pt.x < -2*ONESHIFT)
      actions.push_back(MOVE_LEFT);

    else losttimes++;
  }

  else
  {
    if(headState==1)
    {
      actions.push_back(HEAD_UP);
      headState=11;
    }
    else if(headState == 11)
    {
      actions.push_back(HEAD_RIGHT);
      headState = 12;
    }
    else if(headState == 12)
    {
      actions.push_back(HEAD_DOWN);
      headState = 2;
    }
    else if(headState==2)
    {
      actions.push_back(HEAD_LEFT);
      actions.push_back(HEAD_LEFT);
      headState=3;
    }
    else if(headState==-2)
    {
      actions.push_back(HEAD_LEFT);
      headState=-1;
    }
    else if(headState==-1)
    {
      actions.push_back(HEAD_LEFT);
      headState=-3;
    }
    else if(headState==-3)
    {
      actions.push_back(HEAD_UP);
      headState=3;
    }
    else if(headState==3)
    {
      actions.push_back(HEAD_UP);
      headState=13;
    }
    else if(headState == 13)
    {
      actions.push_back(HEAD_RIGHT);
      headState = 11;
    }
  }
}

void goalkeeper3v3::headreturn()
{
  if(headState==3||headState==-3||headState == 13)
    actions.push_back(HEAD_RIGHT);

  else if(headState==2||headState==-2||headState == 12)
    actions.push_back(HEAD_LEFT);

  if(headState == 13||headState == 12)
    headState = 11;
  else if(headState < 0)
    headState = -1;
  else if(headState > 0 && headState < 5)
    headState = 1;

  qDebug()<<"headreturn----->now headstate: "<<headState;
}
