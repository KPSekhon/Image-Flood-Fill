/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
 img = inputimg;
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{if(p.x > img.width() || p.y > img.height()){
  return HSLAPixel();
}
  PNG act = img;
  HSLAPixel* actH = act.getPixel(p.x, p.y);
  HSLAPixel negative = HSLAPixel();
  double temp;


  negative.a = actH->a;
  negative.s = actH->s;
  negative.l = 1.0 - actH->l;
  if(actH->h+180> 360.0){
     temp = 360 - actH->h;
    temp = 180 - temp;
  } else{
    temp = actH->h +180;
  }
  negative.h = temp;

  // complete your implementation below
  return negative;
}