#include "strategy_pointdc.h"
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
#define DISTANTC 270//低头小于此值则需小步前进 大于此值可以踢

/*
头部角度假设：头左右转时的中心点在头正视时一定在视野内
*/

/*islost的判定集中在makestrategy()前排*/


/*需要校准的量
        headstate及#define的配合
        ballstate的判断是否准确
*/

/*
先进入2阶段对准球，然后进入1校准球门，再进入3移动，循环数遍满足阈值后踢球
cstate=1 确定门的位置 只转头不转身体 若没有门则平移旋转，然后进入cstate=2重新校准球，
                满足阈值后进入cstate=4确认
cstate=2 确定球的位置 使人正对球 且有一段距离 防止进行调整时碰到球
cstate=3 根据门的位置 平移转身..多试几次一定会成功的
cstate=4 判定ballready是否为真，若真，向前小挪一步，踢球！
            若不满足，进入cstate=2-------因为4由1进入，所以门必然满足

总顺序为 2-1-3-2-1-4
*/

/*
cstate=1找到后才可能进入cstate=3,所以cstate=3没有迷失
cstate=3只运行一遍便进入cstate=2，所以务必一次调准！
cstate=2满足后进入cstate=1

islost和cstate=2均可进入cstate=1，islost进入时ballready=false,cstate=2进入时ballready=true
cstate=4时判定ballready，不为真时再进入cstate=2进行微调
*/


strategy_pointdc::strategy_pointdc()
{
  start=true;
  isclose = false;
  islost = false;
  ballready = false;
  gateready = false;

  isturn = 0;
  ballstate = 1;
  headState = 11;
  losttimes = 0;
  startlt = 0;
  cstate = 0;
  x = 0;
  y = 0;
  pre_pt.x = 0;
  pre_pt.y = 0;
}

bool strategy_pointdc::preprocessing()
{
  Ball = dynamic_cast<HyperPoint*>(obj_inputs[0]);
  if (Ball==NULL)
  {
    return false;
  }
  else
    return true;
}

void strategy_pointdc::makestrategy(bool block,bool mover)
{
  x = Ball->circle_centre.x-MIDDLE;
  y = Ball->circle_centre.y;
  if(x > 320 || x < -320) Ball->valid = false;
  if(y > 480 || y < 0) Ball->valid = false;
  if(Ball->gate_centre.x > 640 || Ball->gate_centre.x < 0) Ball->gate_valid = false;
  if(Ball->gate_centre.y > 480 ||Ball->gate_centre.y < 0) Ball->gate_valid = false;
  if(Ball->gate_centre.x == 0 && Ball->gate_centre.y == 0) Ball->gate_valid = false;

  qDebug()<<endl;
  qDebug()<<"start: "<<start;
  qDebug()<<"ballstate: "<<ballstate;
  qDebug()<<"headstate: "<<headState;
  qDebug()<<"isturn: "<<isturn;
  qDebug()<<"x:"<<x<<" y:"<<y<<" Valid:"<<Ball->valid;
  qDebug()<<"gate--->x:"<<Ball->gate_centre.x<<" y:"<<Ball->gate_centre.y<<" Valid:"<<Ball->gate_valid;
  qDebug()<<"isclose: "<<isclose;

  if(start)//意义在于判断ballstate
  {
    fstart();
    qDebug()<<"ballstate: "<<ballstate;
    return;
  }

  qDebug()<<"cstate: "<<cstate;
  if(cstate == 0 || cstate == 2)
  {
    if(!Ball->valid)
    {
      islost = true;
    }
    else
    {
      islost = false;
      losttimes = 0;
    }
  }

  else if(cstate == 3||cstate == 1)
  {
    if(Ball->gate_valid || gateready)
    {
      islost = false;
      losttimes = 0;
    }

    else
      islost = true;
  }
  qDebug()<<"islost: "<<islost;
  qDebug()<<"gateready: "<<gateready;
  qDebug()<<"Ballready: "<<ballready;

  if(islost)
    lostNsearch();

  else if(isclose)
    shootadjuster();

  else if(Ball->valid)//有球
  {
    //头纠正
    if(headState != 1 && headState!=-1 && headState != 11)
    {
      if(headState == 2||headState == -2 || headState == 12)
      {
        if(x < 100)
        {
          actions.push_back(TURN_RIGHT);
          hereturn();
        }
        else
          actions.push_back(TURN_RIGHT);
      }
      else if(headState == 3||headState == -3||headState == 13)
      {
        if(x > 100)
        {
          actions.push_back(TURN_LEFT);
          hereturn();
        }
        else
          actions.push_back(TURN_LEFT);
      }
      else
        hereturn();

    }//头纠正

    else//headstate=+-1/11
    {
      if(headState == 11)
      {
        if(y > DISTANTSS)
        {
          actions.push_back(HEAD_DOWN);
          headState = 1;
        }
        else
        {
          if(x < -100)
            actions.push_back(TURN_LEFT);

          else if(x > 100)
            actions.push_back(TURN_RIGHT);

          else
            actions.push_back(MOVE_FORWARD);
        }
      }
      else if(y < DISTANTS-70 && headState == 1)
      {
        if(x < -100)
          actions.push_back(MOVE_LEFT);

        else if(x > 100)
          actions.push_back(MOVE_RIGHT);

        else
          actions.push_back(MOVE_FORWARD_LITTLE);
      }
      else
      {
        isclose = true;
        cstate = 2;
        shootadjuster();
      }
    }//靠近球
  }//有球
  if(Ball->valid)
  {
    pre_pt.x = x;
    pre_pt.y = y;
  }
  qDebug()<<"ballready: "<<ballready;
  //以防万一，以后在mainwindow里改
  if(actions.empty()) actions.push_back(STAND_POSE);
}

void strategy_pointdc::fstart()
{
  qDebug()<<"x:"<<x<<" y:"<<y;
  if(Ball->valid == false)
  {
    qDebug()<<"ball->valid: "<<Ball->valid;
    if(startlt == 0)
    {
      actions.push_back(HEAD_DOWN);
      headState = 1;
    }
    else if(startlt == 1)
    {
      actions.push_back(HEAD_UP);
      actions.push_back(HEAD_LEFT);
      headState = 13;
    }
    else if(startlt == 2)
    {
      actions.push_back(HEAD_RIGHT);
      actions.push_back(HEAD_RIGHT);
      headState = 12;
    }
    else if(startlt == 3)
    {
      actions.push_back(HEAD_LEFT);
      headState = 11;
      startlt++;
    }
    if(startlt > 3)
      actions.push_back(MOVE_FORWARD);

    startlt++;
  }
  else
  {
    x = Ball->circle_centre.x-MIDDLE;
    y = Ball->circle_centre.y;

    if(headState == 11)
    {
      if(x < 0)
      {
        ballstate = -1;
      }
      else
      {
        ballstate = 1;
      }
      if(x < 90 && x > -90) isturn = 2;
    }
    else if(headState == 12)
    {
      ballstate = 1;
      isturn = 1;
    }
    else
    {
      ballstate = -1;
      isturn = -1;
    }

    start = false;
  }
}

/**************************射门！*************************************/
void strategy_pointdc::shootadjuster()
{
  if(cstate == 1)//找到球门了
  {
    qDebug()<<"enter cstate=1";
    if(gateready == false)
      cstate = 3;

    else if(ballready == false)
      cstate = 2;

    else
      cstate = 4;
  }

  if(cstate == 2)
  {
    qDebug()<<"enter cstate=2";
    if(headState == 11)
    {
      actions.push_back(HEAD_DOWN);
      headState = 1;
    }

    else if(headState == 1)
    {
      if(x < -ONESHIFT)
        actions.push_back(MOVE_LEFT);

      else if(x > ONESHIFT)
        actions.push_back(MOVE_RIGHT);

      else if(x < -ONESSHIFT)
        actions.push_back(MOVE_LEFT_LITTLE);

      else if(x > ONESSHIFT)
        actions.push_back(MOVE_RIGHT_LITTLE);

      else
        if(y < DISTANTS)
          actions.push_back(MOVE_FORWARD_LITTLE);
        else
        {
          actions.push_back(HEAD_DOWN);
          headState = -1;
        }
    }
    else if(headState == -1)
    {
      if(x < -ONESHIFT)
        actions.push_back(MOVE_LEFT);

      else if(x > ONESHIFT)
        actions.push_back(MOVE_RIGHT);

      else if(x < -ONESSHIFT)
        actions.push_back(MOVE_LEFT_LITTLE);

      else if(x > ONESSHIFT)
        actions.push_back(MOVE_RIGHT_LITTLE);

      if(y < DISTANTS-150)
        actions.push_back(MOVE_FORWARD_LITTLE);
      else//球已对准，进入1阶段
      {
        if(gateready)
        {
          cstate = 1;
          ballready = true;
        }
        else
        {
          cstate = 1;
          ballready = true;
          actions.push_back(HEAD_UP);
          actions.push_back(HEAD_UP);
          headState = 11;
        }
      }
    }
    else//头不直
    {
      if(headState == 2 || headState == -2)
      {
        if(x > 0)
        {
          actions.push_back(MOVE_LEFT);
          actions.push_back(TURN_RIGHT);
        }
        hereturn();
      }
      else if(headState == 3 || headState == -3)
      {
        if(x < 0)
        {
          actions.push_back(MOVE_RIGHT);
          actions.push_back(TURN_LEFT);
        }
        hereturn();
      }
      else if(headState == 13|| headState ==12)
        hereturn();
    }
  }

  //进入此阶段的均为不满足球门阈值的
  if(cstate == 3)
  {
    qDebug()<<"enter cstate=3";
    gateready = true;
    cstate = 2;
    //先抬头
    if(headState < 0)
    {
      actions.push_back(HEAD_UP);
      headState = -headState;
      cstate = 3;
    }
    if(headState < 10)
    {
      actions.push_back(HEAD_UP);
      headState += 10;
      cstate = 3;
      return;
    }

    //挪
    if(ballstate == 1)
    {
      if(headState == 11)
      {
        if(Ball->gate_centre.x < 0)//组合运动
        {
          actions.push_back(MOVE_RIGHT);
          actions.push_back(TURN_LEFT);
          actions.push_back(HEAD_LEFT);
          headState = 11;
        }
      }
      else if(headState == 12)
      {
        actions.push_back(HEAD_LEFT);
        headState = 11;
      }
      else if(headState == 13)//无条件转一次
      {
        actions.push_back(MOVE_RIGHT);
        actions.push_back(TURN_LEFT);
        actions.push_back(HEAD_LEFT);
        headState = 11;
      }
    }


    else if(ballstate == -1)
    {
      if(headState == 11)
      {
        if(Ball->gate_centre.x > 0)//一次组合运动
        {
          actions.push_back(MOVE_LEFT);
          actions.push_back(TURN_RIGHT);
          actions.push_back(HEAD_RIGHT);
          headState = 11;
        }
      }
      else if(headState == 12)//无条件转一次
      {
        actions.push_back(MOVE_LEFT);
        actions.push_back(TURN_RIGHT);
        actions.push_back(HEAD_RIGHT);
        headState = 11;
      }
      else if(headState == 13)
      {
        actions.push_back(HEAD_RIGHT);
        headState = 11;
      }
    }

    if(headState > 10)
    {
      actions.push_back(HEAD_DOWN);
      actions.push_back(HEAD_DOWN);
      headState = 10-headState;
    }
  }

  //cstate = 4,gateready=ballready=true
  if(cstate == 4)
  {
    qDebug()<<"enter cstate=4";
    if(headState == 1)
    {
      actions.push_back(HEAD_DOWN);
      headState = -1;
    }
    else if(headState == -1)//ballready现场判定，不论真假
    {
      if(y > DISTANTC)//踢！踢！踢！踢！踢！踢！踢！踢！
      {
        if(x > 250)
          actions.push_back(MOVE_RIGHT);

        else if(x < -250)
          actions.push_back(MOVE_LEFT);

        else if(x > 25)
          actions.push_back(KICK_RIGHT);

        else if(x < -25)
          actions.push_back(KICK_LEFT);

        else if(x > 0)
          actions.push_back(MOVE_LEFT);

        else
          actions.push_back(MOVE_RIGHT);

        isclose = false;
      }
      else
        actions.push_back(MOVE_FORWARD_LITTLE);
    }
    else if(headState >=10)
    {
      actions.push_back(HEAD_DOWN);
      headState -= 10;
    }
    else
    {
      hereturn();
    }
  }
}

/*****************************迷失处理*********************************/
void strategy_pointdc::lostNsearch()
{
  qDebug()<<"enter lostNsearch";
  //初次迷失处理
  if(losttimes == 0)
  {
    qDebug()<<"losttimes = 0";
    if(cstate == 1 ||cstate == 3)//找门
    {
      if(headState < 0)//先抬头
      {
        actions.push_back(HEAD_UP);
        actions.push_back(HEAD_UP);
        headState =-headState+10;
      }
      else if(headState < 10)
      {
        actions.push_back(HEAD_UP);
        headState += 10;
      }

      else if(headState == 11)
      {
        gateready = true;//找不到门妥协步骤
        if(isturn == 2)
        {
          actions.push_back(MOVE_LEFT);
          actions.push_back(TURN_RIGHT);
        }
        else if(isturn == 1)
        {
          actions.push_back(MOVE_RIGHT);
          actions.push_back(TURN_LEFT);
        }
        else if(isturn == -1)
        {
          actions.push_back(MOVE_LEFT);
          actions.push_back(TURN_RIGHT);
        }
        else if(ballstate == -1)
        {
          actions.push_back(HEAD_LEFT);
          headState = 13;
        }
        else if(ballstate == 1)
        {
          actions.push_back(HEAD_RIGHT);
          headState = 12;
        }
      }
      else
        hereturn();
      return;
    }
    else if(cstate == 2)//找球,若依然没有球，转头找球
    {
      ballready = false;
      if(headState > 10)
      {
        actions.push_back(HEAD_DOWN);
        actions.push_back(HEAD_DOWN);
        headState -= 10;
        headState = -headState;
        return;
      }
      else if(headState > 0)
      {
        actions.push_back(HEAD_DOWN);
        headState = -headState;
      }
      else
      {
        actions.push_back(MOVE_BACK);
        losttimes = 2;
        return;
      }
    }

    //cstate == 0的迷失处理
    else if(headState == 11 || headState == 12 || headState == 13)
    {
      actions.push_back(HEAD_DOWN);
      headState -= 10;
    }
    else if(headState > 0)//头朝上
    {
      if(pre_pt.y > DISTANTS)
      {
        actions.push_back(HEAD_DOWN);
        headState = -headState;
      }
      else
      {
        actions.push_back(HEAD_UP);
        actions.push_back(MOVE_BACK);
        headState += 10;
      }
    }
    else//头朝下
    {
      if(pre_pt.y < DISTANTS)
      {
        actions.push_back(HEAD_UP);
        headState = -headState;
      }
      else
        losttimes++;
    }
  }

  if(losttimes == 1)
  {
    qDebug()<<"losttimes = 1";
    if(headState == 11)
    {
      actions.push_back(HEAD_DOWN);
      headState -= 10;
    }
    else if(headState == 1)
    {
      if(pre_pt.x > 50)
      {
        actions.push_back(HEAD_RIGHT);
        headState = 2;
      }
      else if(pre_pt.x < -50)
      {
        actions.push_back(HEAD_LEFT);
        headState = 3;
      }
      else
        losttimes++;
    }
    else if(headState == -1)
    {
      if(pre_pt.x > 100)
      {
        actions.push_back(HEAD_RIGHT);
        headState = -2;
      }
      else if(pre_pt.x < -100)
      {
        actions.push_back(HEAD_LEFT);
        headState = -3;
      }
      else
        losttimes++;
    }

    else if(headState != 1 && headState != -1)
      hereturn();
  }

  //转头查找
  if(losttimes < 8 && losttimes > 1)
  {
    qDebug()<<"losttimes :"<<losttimes;
    if(headState == 1)
    {
      actions.push_back(HEAD_RIGHT);
      headState = 2;
    }
    else if(headState == 2)
    {
      actions.push_back(HEAD_DOWN);
      headState = -2;
    }
    else if (headState == -2)
    {
      actions.push_back(HEAD_LEFT);
      headState = -1;
    }
    else if(headState == -1)
    {
      actions.push_back(HEAD_LEFT);
      headState = -3;
    }
    else if(headState == -3)
    {
      actions.push_back(HEAD_UP);
      headState = 3;
    }
    else if(headState == 3)
    {
      actions.push_back(HEAD_RIGHT);
      headState = 1;
    }
  }

  //isclose 迷失处理，losttimes=8,
  //此处假设一次平移和转动后 球依然在视野中心附近
  else if(cstate == 1||cstate ==3)
  {
    qDebug()<<"lostNsearch:cstate = 1,3,losttimes == 8";
    if(headState < 0)
    {
      actions.push_back(HEAD_UP);
      headState = -headState;

      if(headState < 10)
      {
        actions.push_back(HEAD_UP);
        headState += 10;
      }

      if(headState != 11)
        hereturn();

      if(ballstate == 1)
      {

        actions.push_back(MOVE_LEFT);
        actions.push_back(TURN_RIGHT);
        cstate = 2;
      }

      else if(ballstate == -1)
      {

        actions.push_back(MOVE_RIGHT);
        actions.push_back(TURN_LEFT);
        cstate = 2;
      }

      actions.push_back(HEAD_DOWN);
      actions.push_back(HEAD_DOWN);
      headState-=10;
      headState = -headState;
      return;
    }
    //不明情况后退一步先
    else if(cstate != 0)
    {
      actions.push_back(MOVE_BACK);
      losttimes = 2;
    }

    //正常迷失处理
    else
    {
      if(losttimes < 14)
        actions.push_back(TURN_LEFT);

      else
        actions.push_back(TURN_RIGHT);

    }
    losttimes++;
  }
}
void strategy_pointdc::hereturn()
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
