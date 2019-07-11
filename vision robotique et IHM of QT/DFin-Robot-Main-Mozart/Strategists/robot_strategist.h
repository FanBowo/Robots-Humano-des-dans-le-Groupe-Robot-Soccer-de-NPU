#ifndef ROBOT_STRATEGIST_H
#define ROBOT_STRATEGIST_H

/*
  策略模块 v1.0
    废话不多说,
    请继承该类并重载 preprocessing() 以及 makestrategy()
    图像处理结果被保存在 obj_inputs 里, 请在preprocessing()里
    将诸处理结果转型成为相应类型
    同样,在makestrategy()里,请将需要下达的命令
    push_back至actions,
    相应动作在global文件定义的robot_action枚举类型里

 */

#include "Robot_Strategist_global.h"
#include "../ImageDispose/robot_imagedispose.h"

class Robot_Strategist {
public:
    Robot_Strategist();
    bool getDisposeResult(vector<ImageDisposeResult*> &m_inputs,bool block,bool mover);
    vector<robot_action> orders() {return actions;}

protected:
    virtual bool preprocessing() = 0;
    virtual void makestrategy(bool block,bool mover) = 0;
    vector<robot_action> actions;
    vector<ImageDisposeResult*> obj_inputs;
};

#endif // ROBOT_STRATEGIST_H
