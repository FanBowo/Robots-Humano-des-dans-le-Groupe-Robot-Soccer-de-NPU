#include "hawkseye.h"

void HawksPrey:: operator=(ImageDisposeResult &_res)
{
  HawksPrey *p_res = dynamic_cast<HawksPrey*>(&_res);
  this->valid = p_res->valid;
  this->circle_centre = p_res->circle_centre;
  this->circle_radius = p_res->circle_radius;
  this->rect_centre = p_res->rect_centre;
  this->rect = p_res->rect;
  this->min_rect = p_res->min_rect;
  //  qDebug()<<"x:"<<circle_centre.x<<" y:"<<circle_centre.y;
}

QString HawksPrey::toString() {
  QString _temp;
  if (!valid)//
    _temp = "not found";
  else
  {
    _temp = "x: "+QString::number(circle_centre.x) + "y: "+QString::number(circle_centre.y)/*center[most_possible]*/;
  }
  return _temp;
}

void HawksEye::dispose()
{
  temp_res = new HawksPrey;
  HawksPrey *p_res =
      dynamic_cast<HawksPrey*>(temp_res);
  cv::namedWindow("ball",WINDOW_NORMAL);
  cv::resizeWindow("ball",320,240);
  cv::namedWindow("gate",WINDOW_NORMAL);
  cv::resizeWindow("gate",320,240);
  if(seq==0) imshow("ball",img);
  else imshow("gate",img);
  threshold(img,img,60,255,THRESH_BINARY);
  blur(img,img,Size(11,11));



  contours.clear();
  contours_poly.clear();
  boundRect.clear();
  center.clear();
  radius.clear();
  minRect.clear();

  findContours(img,contours,
               cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE,
               Point(0,0));
//使用树形拓扑形式来存储边界
  minRect.resize(contours.size());
  contours_poly.resize(contours.size());
  boundRect.resize(contours.size());
  center.resize(contours.size());
  radius.resize(contours.size());
  for( int i = 0; i < contours.size(); i++ )
  {
    minRect[i] = minAreaRect( Mat(contours[i]));
    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    minEnclosingCircle( contours_poly[i], center[i], radius[i] );
  }
  most_possible = -1;
  if (contours.size() == 0)
  {
    p_res->valid = false;
  }
  else
  {
    float max=0;

    for( int i = 0; i< contours.size(); i++ )
    {
      if (radius[i]>max)
      {
        most_possible = i;
        max = radius[i];
      }
    }

    p_res->valid = true;
    p_res->circle_centre = center[most_possible];
    p_res->circle_radius = radius[most_possible];
    p_res->min_rect = minRect[most_possible];
    p_res->rect = boundRect[most_possible];
    p_res->rect_centre.x = p_res->rect.x + p_res->rect.width/2;
    p_res->rect_centre.y = p_res->rect.y + p_res->rect.height/2;

    if (show_mid_result)
      showResult();
  }
}
void HawksEye::showResult()
{
  Mat drawing = Mat::zeros( img.size(), CV_8UC3 );
  int a,b,c,d,e,f,left,Cleft,right,Cright,buffer1,buffer2;
  int moveX,moveY;

  moveX=10;
  moveY=-5;
  //x:
  a=160+moveX;
  b=260+moveX;
  c=360+moveX;
  d=460+moveX;
  left=200+moveX;
  right=400+moveX;
  Cleft=300+moveX;
  Cright=320+moveX;
  //y:
  buffer1=410+moveY;
  buffer2=440+moveY;
  e=270;
  f=380;
  //distance:
  left=left-20;
  right=right+40;
  Cleft=Cleft+0;
  Cright=Cright+10;

  rectangle( drawing, Point(b,e), Point(c,480), Scalar(250,0,0), 1, 8 );
  if(most_possible < 0) {
    if (seq==1) imshow( "gate-dis", drawing );
    else imshow("ball-dis",drawing);
  }
  if (most_possible >= 0)
  {
    drawContours( drawing, contours_poly, most_possible,Scalar( 255, 255, 255 ), 1, 8, vector<Vec4i>(), 0, Point() );
    rectangle( drawing, boundRect[most_possible].tl(), boundRect[most_possible].br(), Scalar( 0, 255, 0 ), 2, 8, 0 );
    circle( drawing, center[most_possible], (int)radius[most_possible], Scalar( 255, 0, 0 ), 2, 8, 0 );
    circle( drawing, center[most_possible],3, Scalar( 255, 0, 0 ), 2, 8, 0 );
    Scalar color_red = Scalar(0,0,255);
    Point2f rect_points[4]; minRect[most_possible].points( rect_points );
    for( int j = 0; j < 4; j++ )
      line( drawing, rect_points[j], rect_points[(j+1)%4], color_red, 1, 8 );

    cv::namedWindow("ball-dis",WINDOW_NORMAL);
    cv::resizeWindow("ball-dis",320,240);
    cv::namedWindow("gate-dis",WINDOW_NORMAL);
    cv::resizeWindow("gate-dis",320,240);
    if (seq==1) imshow( "gate-dis", drawing );
    else imshow("ball-dis",drawing);
  }

}
