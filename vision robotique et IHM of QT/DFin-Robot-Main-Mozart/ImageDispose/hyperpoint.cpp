#include "hyperpoint.h"

void HyperPoint:: operator=(ImageDisposeResult &_res)
{
    HyperPoint *p_res = dynamic_cast<HyperPoint*>(&_res);
    this->circle_centre = p_res->circle_centre;
    this->circle_radius = p_res->circle_radius;
    this->rect_centre = p_res->rect_centre;
    this->rect = p_res->rect;
    this->min_rect = p_res->min_rect;
    this->gate_centre = p_res->gate_centre;
    this->gate_valid = p_res->gate_valid;
}

QString HyperPoint::toString() {
    QString _temp;
    if (!valid)//
        _temp = "not found";
    else
    {
        _temp = "x: "+QString::number(circle_centre.x) + "y: "+QString::number(circle_centre.y)/*center[most_possible]*/;
    }
    return _temp;
}
void HyperPoint::hyperres(ImageDisposeResult &res1, ImageDisposeResult &res2)
{
        HawksPrey *p_res1 = dynamic_cast<HawksPrey*>(&res1);
            HawksPrey *p_res2 = dynamic_cast<HawksPrey*>(&res2);
    valid = p_res1->valid;
    circle_centre = p_res1->circle_centre;
    circle_radius = p_res1->circle_radius;
    rect_centre = p_res1->rect_centre;
    rect = p_res1->rect;
    min_rect = p_res1->min_rect;
    gate_centre = p_res2->circle_centre;
    gate_valid = p_res2->valid;
}
