/*
*  File: censorColorPicker.cpp
*  Implements the censor color picker for CPSC 221 PA2.
*
*/

#include "censorColorPicker.h"

#include <math.h> // gives access to sqrt function

/*
*  Useful function for computing the Euclidean distance between two PixelPoints
*/
double PointDistance(PixelPoint a, PixelPoint b) {
  unsigned int diff_x, diff_y;
  if (a.x > b.x)
    diff_x = a.x - b.x;
  else
    diff_x = b.x - a.x;
  if (a.y > b.y)
    diff_y = a.y - b.y;
  else
    diff_y = b.y - a.y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG& inputimage)
{
  // complete your implementation below
  blockwidth = b_width;
  center = ctr;
  radius = rad;
  img = inputimage;
  blockyimg = PNG(img.width(), img.height());
   mosiacImageMaker();
}

HSLAPixel CensorColorPicker::operator()(PixelPoint p)
{
  double rad = radius;
  if(PointDistance(p,center)>rad){
    return *(img.getPixel(p.x,p.y));
  }
  return *(blockyimg.getPixel(p.x,p.y));
  
}
// might want to make it reference variable
// debugging hypothesis: the y value peaks out at y=x
void CensorColorPicker::mosiacImageMaker(){
  unsigned int maxX,maxY;
  if(img.height()==0&& img.width()==0){
    return;
  }
  for(unsigned int y = 0; y <blockyimg.height(); y+=blockwidth){
    for(unsigned int x = 0; x < blockyimg.width(); x+=blockwidth){
      maxY = blockAdjusterHeight(y);
      maxX = blockAdjusterWidth(x);
      blockMaker(x,y,maxX,maxY);
    }
  }
}

void CensorColorPicker::blockMaker(unsigned int x, unsigned int y, unsigned int maxX, unsigned int maxY){
  HSLAPixel average = averageColorFinder(x,y, maxX, maxY);
  HSLAPixel* curr;
  for(unsigned int y1 = y; y1 < maxY; y1++){
  for(unsigned int x1 =x; x1<maxX; x1++){
    curr = blockyimg.getPixel(x1, y1);
    curr->a = average.a;
    curr->h = average.h;
    curr->s = average.s;
    curr->l = average.l;
  }
}

 curr = NULL;
}

unsigned int CensorColorPicker::blockAdjusterWidth(unsigned int b){
unsigned int val = b + blockwidth;
// while(val>input.width()){
//   val-=1;
// }
//std::cout<<"The value of maxX is "<<val<<std::endl;
if(val>img.width()){
  val = img.width() - 1;
}
return val;
}
unsigned int CensorColorPicker::blockAdjusterHeight(unsigned int b){
unsigned int val = b + blockwidth;
// while(val>input.height()){
//   val-=1;
// }
if(val>img.height()){
  val = img.height() - 1;
}
//std::cout<<"The value of maxY is "<<val<<std::endl;
return val;
}

HSLAPixel CensorColorPicker:: averageColorFinder(unsigned int x, unsigned int y, unsigned int maxX, unsigned int maxY){
HSLAPixel average;
HSLAPixel curr;
double count = 0.0;
double hue = 0.0;
double sat = 0.0;
double lum = 0.0;
double alp = 0.0;
for(unsigned int y1 = y; y1 < maxY; y1++){
  for(unsigned int x1 =x; x1<maxX; x1++){
    count++;
    curr = *(img.getPixel(x1, y1));
    hue+= curr.h;
    sat += curr.s;
    lum += curr.l;
    alp += curr.a;
  }
}
average.a = alp/count;
average.h = hue/count;
average.l = lum/count;
average.s = sat/count;
return average;
}