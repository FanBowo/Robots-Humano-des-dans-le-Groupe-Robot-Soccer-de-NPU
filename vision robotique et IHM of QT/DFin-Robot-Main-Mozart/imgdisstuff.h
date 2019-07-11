#ifndef IMGDISSTUFF_H
#define IMGDISSTUFF_H

#include "ImageDispose/image_circle.h"
#include "ImageDispose/hawkseye.h"
#include "ImageDispose/hyperpoint.h"
#include <string>

const int DISPOSE_RESULT_COUNT = 4;

typedef enum{
    BALL,HAWK
}DisResType;

typedef struct{
    bool start_using;
    std::string obj_path;
    std::string obj_path_2;
    int res_mod_switcher;   //rgb, hs, synthese
    DisResType type;
    bool show;
    bool test;
}DisResStatus;

class DisFactory
{
public:
    DisFactory() {}
    Robot_ImageDispose* newDis( DisResType _type )
    {
        switch(_type)
        {
        case BALL: return new Image_Circle; break;
        case HAWK: return new HawksEye; break;

        default: return NULL;
        }
    }
};

class DisResFactory
{
public:
    DisResFactory() {}
    ImageDisposeResult* newDisRes( DisResType _type )
    {
        switch(_type)
        {
        case BALL: return new Image_BallCentre; break;
        case HAWK: return new HawksPrey; break;
        default: return NULL;
        }
    }
};

#endif // IMGDISSTUFF_H
