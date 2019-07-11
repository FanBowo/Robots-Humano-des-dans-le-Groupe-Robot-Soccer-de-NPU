#include "striker3v3.h"
#define ONESHIFT 200
#define VDIVISION 200//y坐标大于此值则视为近，低头
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

striker3v3::striker3v3()
{
  headState = 11;
  hereturn = false;
  pre_pt.x = 0;
  pre_pt.y = 0;
  losttimes = 0;
}

bool striker3v3::preprocessing()
{
  Ball = dynamic_cast<HawksPrey*>(obj_inputs[0]);
  if (Ball == NULL)
  {
    return false;
  }
  else
    return true;
}

void striker3v3::makestrategy(bool block,bool mover)
{
  x = Ball->circle_centre.x-MIDDLE;
  y = Ball->circle_centre.y;
  if(x > 320 || x < -320) Ball->valid = false;
  if(y > 480 || y < 0) Ball->valid = false;

  qDebug()<<endl;
  qDebug()<<"headstate: "<<headState;
  qDebug()<<"x:"<<x<<" y:"<<y<<" Valid:"<<Ball->valid;

  if(Ball->valid == false)
  {
    searchBall();
    losttimes++;
    return;
  }
  else
  {
    losttimes = 0;
  }

  if(hereturn == true)
  {
    headreturn();
    return;
  }

  //踢球
  if(headState == 12)
  {
    if(x > 0)
      actions.push_back(TURN_RIGHT);

    actions.push_back(HEAD_LEFT);
    headState = 11;
  }

  else if(headState == 13)
  {
    if(x < 0)
      actions.push_back(TURN_LEFT);

    actions.push_back(HEAD_RIGHT);
    headState = 11;
  }

  else if(headState == 11)
  {
    if(x > ONESHIFT)
      actions.push_back(TURN_RIGHT);

    else if(x < -ONESHIFT)
      actions.push_back(TURN_LEFT);

    else if(y > DISTANTSS)
    {
      actions.push_back(HEAD_DOWN);
      headState = 1;
    }

    else
      actions.push_back(MOVE_FORWARD);
  }

  else if(headState == 1)
  {
    if(y < DISTANTS)//较远
    {
      if(x > ONESHIFT)
        actions.push_back(TURN_RIGHT);

      else if(x < -ONESHIFT)
        actions.push_back(TURN_LEFT);

      else
        actions.push_back(MOVE_FORWARD);
    }

    else if(y >= DISTANTS)//较近
    {
      actions.push_back(HEAD_DOWN);
      headState = -1;
    }
  }

  else if(headState == -1)
  {
    if(x > ONESHIFT)
      actions.push_back(MOVE_RIGHT);

    else if(x < -ONESHIFT)
      actions.push_back(MOVE_LEFT);

    else if(y < DISTANTC)
      actions.push_back(MOVE_FORWARD_LITTLE);

    else if(x > 0)
    {
      actions.push_back(MOVE_FORWARD);
      actions.push_back(MOVE_FORWARD);
    }

    else if(x < 0)
    {
      actions.push_back(MOVE_FORWARD);
      actions.push_back(MOVE_FORWARD);
    }
  }

  else if(headState == 2)
  {
    if(x > 0)
      actions.push_back(TURN_RIGHT);

    actions.push_back(HEAD_LEFT);
    headState = 1;
  }

  else if(headState == -2)
  {
    if(x > -ONESHIFT)

      actions.push_back(TURN_RIGHT);

    actions.push_back(HEAD_LEFT);
    headState = -1;
  }

  else if(headState == 3)
  {
    if(x < ONESHIFT)
      actions.push_back(TURN_LEFT);

    actions.push_back(HEAD_RIGHT);
    headState = 1;
  }

  else if(headState == -3)
  {
    if(x < ONESHIFT)
      actions.push_back(TURN_LEFT);

    actions.push_back(HEAD_RIGHT);
    headState = -1;
  }

  pre_pt.x = x;
  pre_pt.y = y;
}

void striker3v3::searchBall()
{
  if(losttimes == 0)
  {
    if(headState == -1 || headState == -2 || headState == -3)
    {
      actions.push_back(HEAD_UP);
      headState = -headState;
    }
    else if(headState == 1 || headState == 2 || headState == 3)
    {
      if(pre_pt.y < DISTANTS)
      {
        actions.push_back(HEAD_UP);
        headState += 10;
      }
      else
      {
        actions.push_back(MOVE_FORWARD);
      }
    }
    else if(headState == 11 || headState == 12 || headState == 13)
    {
      if(pre_pt.y < DISTANTS)
      {
        actions.push_back(MOVE_FORWARD);
      }
      else
      {
        actions.push_back(HEAD_DOWN);
        headState += 10;
      }
    }
    else losttimes++;//废弃了

  }
  //正常第二次迷失或抬头第一次迷失
  if((pre_pt.y < DISTANTS) && losttimes == 1)
  {
    if(pre_pt.x > 0)
      actions.push_back(TURN_RIGHT);

    else if(pre_pt.x < 0)
      actions.push_back(TURN_LEFT);

    else
      roundsearch();
  }

  else if(pre_pt.y > DISTANTS && losttimes == 1)
  {
    if(headState == 1 || headState == 2 || headState == 3)
    {
      actions.push_back(HEAD_DOWN);
      headState = -headState;
    }
    else
      roundsearch();
  }

  else
    roundsearch();
}

void striker3v3::roundsearch()
{
  headreturn();

  if(losttimes % 3 == 0)
  {
    actions.push_back(TURN_RIGHT);
  }
  else
  {
    if(headState < 0)
    {
      actions.push_back(HEAD_UP);
      actions.push_back(MOVE_FORWARD);
      headState = -headState;
    }
    else if(headState < 10)
    {
      actions.push_back(HEAD_UP);
      headState += 10;
    }
    else
    {
      actions.push_back(HEAD_DOWN);
      headState -= 10;
    }
  }

}

void striker3v3::headreturn()
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
