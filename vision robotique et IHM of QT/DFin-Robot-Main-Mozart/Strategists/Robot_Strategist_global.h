#ifndef ROBOT_STRATEGIST_GLOBAL_H
#define ROBOT_STRATEGIST_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>
#include <vector>

using namespace std;

typedef enum robot_action
{
  STAND_POSE,//站直
  MOVE_FORWARD,//前1
  MOVE_FORWARD2,//前2
  MOVE_FORWARD_LITTLE,//前小
  MOVE_FORWARD_MINI,

  MOVE_FORWARD_TINY,
  MOVE_LEFT,    //左移
  MOVE_LEFT_LITTLE, //左移小
  MOVE_RIGHT, //右移
  MOVE_RIGHT_LITTLE,//右移小
  MOVE_BACK,  //后1
  MOVE_BACK2, //后2
  TURN_RIGHT, //右转
  TURN_RIGHT_big,
  TURN_LEFT,  //左转
  TURN_LEFT_big,
  KICK_LEFT,  //左踢
  KICK_RIGHT,  //右踢
  CLIMB_DOWN,//劈叉
  HAND_UP_CENTER,
  HAND_UP_LEFT,
  HAND_UP_RIGHT,
  HAND_DOWN_CENTER,
  HAND_DOWN_LEFT,
  HAND_DOWN_RIGHT,
  BOW,
  HEAD_DOWN,//低头
  HEAD_UP,//抬头
  HEAD_RIGHT,//头右转
  HEAD_LEFT,//头左转
  JU_FORWARD,//向前微跳
  JU_BACKWARD,//向后微跳
  JU,//原地微跳

  END
}robot_action;

/******************delete by Ding 2012-12-13***************

static const char* convertOrder(robot_action _type)
{
    const char* _temp;

    switch(_type)
    {
    case STAND_POSE : _temp = "1";break;
    case MOVE_FORWARD: _temp = "2"; break;
    case MOVE_FORWARD_LITTLE:_temp = "B";break;
    case MOVE_FORWARD_MINI:_temp = "C";break;
    case MOVE_FORWARD_TINY:_temp = "H";break;
    case MOVE_BACK: _temp = "3"; break;
    case MOVE_LEFT: _temp = "4"; break;
    case MOVE_LEFT_LITTLE:_temp = "F";break;
    case MOVE_RIGHT: _temp = "5"; break;
    case MOVE_RIGHT_LITTLE:_temp = "D";break;
    case TURN_LEFT: _temp = "6"; break;
    case TURN_RIGHT: _temp = "7"; break;
    case KICK_LEFT: _temp ="G"; break;
    case KICK_RIGHT: _temp = "A"; break;

    default: break;
    }
    return _temp;
}
/**********************************************/


/*static QByteArray convertOrder(robot_action _type)
{
    char _temp;

    switch(_type)
    {
    case STAND_POSE : _temp = 0x01;break;
    case MOVE_FORWARD: _temp = 0x02; break;
    case MOVE_FORWARD_LITTLE:_temp =0x07;break;
    case MOVE_FORWARD_MINI:_temp = 0x06;break;
    case MOVE_FORWARD_TINY:_temp = 0x05;break;
    case MOVE_BACK: _temp = 0x0A; break;
    case MOVE_LEFT: _temp = 0x0F; break;
    case MOVE_LEFT_LITTLE:_temp = 0x10; break;
    case MOVE_RIGHT: _temp = 0x11; break;
    case MOVE_RIGHT_LITTLE:_temp = 0x12;break;
    case TURN_LEFT: _temp = 0x13; break;
    case TURN_RIGHT: _temp = 0x14; break;
    case KICK_LEFT: _temp =0x1B; break;
    case KICK_RIGHT: _temp = 0x1C; break;

    default: break;
    }

    QByteArray temp;
    temp.push_back(_temp);
    return temp;
}*/


static QByteArray convertOrder(robot_action _type)
{
  QByteArray _temp;
  char sign;

  switch(_type)
  {
    case STAND_POSE : sign=0x00/*2e*/; break;
    case MOVE_FORWARD:sign=0x01; break;//gai-01
    case MOVE_FORWARD_LITTLE:sign=0x12;break;//gai-01
    case MOVE_FORWARD_MINI:sign=0x13;break;
    case MOVE_FORWARD_TINY:sign=0x12;break;
    case MOVE_BACK:sign=0x0A; break;//gai0a
    case MOVE_BACK2: sign=0x0A;break;
    case MOVE_LEFT: sign=0x04; break;
    case MOVE_LEFT_LITTLE:sign=0x4; break;
    case MOVE_RIGHT: sign=0x05; break;
    case MOVE_RIGHT_LITTLE:sign=0x05;break;
    case TURN_LEFT: sign=0x02; break;//GAI-02
    case TURN_RIGHT: sign=0x03; break;//GAI-03
    case KICK_LEFT: sign=0x07; break;
    case KICK_RIGHT:sign=0x06; break;
    case BOW: sign=0x00;break;
    case TURN_RIGHT_big: sign=0x03;break;
    case TURN_LEFT_big:  sign=0x02;break;
    case HEAD_DOWN: sign = 0x0e;break;
    case HEAD_UP : sign = 0x0d;break;
    case HEAD_RIGHT: sign = 0x0c;break;
    case HEAD_LEFT : sign = 0x0b;break;
    default: break;
  }
  char f=0x55;
  _temp.push_back(f);
  _temp.push_back(f);
  f=0xff-sign;
  _temp.push_back(sign);
  _temp.push_back(f);
  f=0x00;
  _temp.push_back(f);
  f=0xff;
  _temp.push_back(f);
  return _temp;
}



/******************add by Ding 2012-12-13***************

static QByteArray convertOrder(robot_action _type)
{
    QByteArray _temp;
    _temp.push_back(-1);

    switch(_type)
    {
    case MOVE_LEFT: _temp.push_back(13);
    default: break;
    }
    return _temp;
}
/**********************************************/

static QString actionToString( robot_action _type )
{
  QString _temp;
  switch(_type)
  {
    case STAND_POSE:_temp = "STAND_POSE";break;
    case MOVE_FORWARD: _temp = "MOVE_FORWARD"; break;
    case MOVE_FORWARD2: _temp = "MOVE_FORWARD2"; break;
    case MOVE_FORWARD_LITTLE: _temp = "MOVE_FORWARD_LITTLT";break;
    case MOVE_FORWARD_MINI:_temp = "MOVE_FORWARD_MINI";break;
    case MOVE_FORWARD_TINY:_temp = "MOVE_FORWARD_TINY";break;
    case MOVE_LEFT: _temp = "MOVE_LEFT"; break;
    case MOVE_LEFT_LITTLE: _temp = "MOVE_LEFT_LITTLE";break;
    case MOVE_RIGHT: _temp = "MOVE_RIGHT"; break;
    case MOVE_RIGHT_LITTLE:_temp = "MOVE_RIGHT_LITTLE";break;
    case MOVE_BACK: _temp = "MOVE_BACK"; break;
    case MOVE_BACK2: _temp = "MOVE_BACK2"; break;
    case TURN_RIGHT: _temp = "TURN_RIGHT"; break;
    case TURN_LEFT: _temp = "TURN_LEFT"; break;
    case KICK_LEFT: _temp = "KICK_LEFT"; break;
    case KICK_RIGHT: _temp = "KICK_RIGHT"; break;
    case CLIMB_DOWN : _temp = "CLIMB_DOWN";break;
    case END: _temp = "END"; break;
    case HEAD_DOWN: _temp = "HEAD_DOWN";break;
    case HEAD_LEFT: _temp = "HEAD_LEFT";break;
    case HEAD_RIGHT: _temp = "HEAD_RIGHT";break;
    case HEAD_UP: _temp = "HEAD_UP";break;
    default: break;
  }
  return _temp;
}

#endif // ROBOT_STRATEGIST_GLOBAL_H
