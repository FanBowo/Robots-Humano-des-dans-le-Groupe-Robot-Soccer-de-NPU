#ifndef STRATEGISTSSTUFF_H
#define STRATEGISTSSTUFF_H

#include "Strategists/samplestrategist.h"
#include "Strategists/Strategy_Ball.h"
#include "Strategists/Strategy_Guard.h"
#include "Strategists/robot_zap.h"
#include "Strategists/3v3/goalkeeper3v3.h"
#include "Strategists/3v3/striker3v3.h"
#include "Strategists/strategy_pointdc.h"

typedef enum{
    SAMPLE,KICK_BALL,GUARD_BALL,ZAP,POINT,GOALKEEPER,STRIKER,
}StrategistType;

static StrategistType assignStrategistType( int _index )
{
    StrategistType _temp;
    switch(_index)
    {
    case 0: _temp = SAMPLE; break;
    case 1: _temp = KICK_BALL;break;
    case 2: _temp = GUARD_BALL;break;
    case 3: _temp = ZAP;break;
    case 4: _temp = POINT;break;
    case 5: _temp = GOALKEEPER;break;
    case 6: _temp = STRIKER;break;
    default: break;
    }
    return _temp;
}

class StrategistFactory{
public:
    StrategistFactory() {}
    Robot_Strategist* newStrategist( StrategistType _type )
    {
        switch(_type)
        {
        case SAMPLE: return new SampleStrategist; break;
        case KICK_BALL: return new Strategy_Ball; break;
        case GUARD_BALL:return new Strategy_Guard; break;
        case ZAP: return new ZapStrategist; break;
        case POINT: return new strategy_pointdc;break;
        case GOALKEEPER: return new goalkeeper3v3;break;
        case STRIKER :return new striker3v3;break;
        default: return NULL;
        }
    }
};

#endif // STRATEGISTSSTUFF_H
