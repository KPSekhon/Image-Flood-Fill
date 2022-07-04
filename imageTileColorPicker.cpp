/*
*  File: imageTileColorPicker.cpp
*  Implements the image tile color picker for CPSC 221 PA2.
*
*/

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& otherimage) {
  // complete your implementation below
  if(otherimage.height() >= 1 && otherimage.width()>=1 ){
    this->img_other = otherimage;
  }
}

HSLAPixel ImageTileColorPicker::operator()(PixelPoint p) {
  if(this->img_other.height() > p.y && this->img_other.width()> p.x){
    return *(this->img_other.getPixel(p.x,p.y));
  }
  unsigned int x = p.x;
  unsigned int y= p.y;

  if(this->img_other.height()-1 < y){
  y= (y%this->img_other.height());
  }

  if(this->img_other.width() -1< x){
  x = (x%this->img_other.width());
  }
  return *(this->img_other.getPixel(x,y));
}