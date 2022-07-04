/*
 *  File:        filler.cpp
 *  Description: Implementation of functions in the filler namespace.
 *
 */

/*
*  Performs a flood fill using breadth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
queue works
*/
animation filler::FillBFS(FillerConfig &config)
{
  // complete your implementation below
  // You should replace the following line with a
  // correct call to fill.
  return Fill<Queue>(config);
}

/*
*  Performs a flood fill using depth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
stack works
*/
animation filler::FillDFS(FillerConfig &config)
{
  // complete your implementation below
  // You should replace the following line with a
  // correct call to fill.
  return Fill<Stack>(config);
}

/*
 *  Run a flood fill on an image starting at the seed point
 *
 *  PARAM:  config - FillerConfig struct with data for flood fill of image
 *  RETURN: animation object illustrating progression of flood fill algorithm
 */
template <template <class T> class OrderingStructure>
animation filler::Fill(FillerConfig &config)
{
  /*
   * You need to implement this function!
   *
   * This is the basic description of a flood-fill algorithm: Every fill
   * algorithm requires an ordering structure, which is passed to this
   * function via its template parameter. For a breadth-first-search
   * fill, that structure is a Queue, for a depth-first-search, that
   * structure is a Stack. To begin the algorithm, you simply place the
   * given point in the ordering structure, marking it processed
   * (the way you mark it is a design decision you'll make yourself).
   * We have a choice to either change the color, if appropriate, when we
   * add the point to the OS, or when we take it off. In our test cases,
   * we have assumed that you will change the color when a point is added
   * to the structure.
   * Until the structure is empty, you do the following:
   *
   * color of the seed point in calculating the distance is from original image
   *
   * 1.     Remove a point from the ordering structure, and then...
   *
   *        1.    add its unprocessed neighbors (up/down/left/right) whose color values are
   *              within (or equal to) tolerance distance from the center,
   *              to the ordering structure, and
   *              mark them as processed.
   * this actually means within (or equal to) tolerance distance from the seed point.
   * So we don't want to colour pixels that are too different from the initial location,
   * even if they are similar to their immediate neighbours.
   *
   *        2.    if it is an appropriate frame, send the current PNG to the
   *              animation (as described below).
   *
   * 2.     When implementing your breadth-first-search and
   *        depth-first-search fills, you will need to explore neighboring
   *        pixels (up/down/left/right) in some order.
   *
   *        While the order in which you examine neighbors does not matter
   *        for a proper fill, you must use the same order as we do for
   *        your animations to come out like ours! The order you should put
   *        neighboring pixels **ONTO** the queue or stack is based on the
   *        following priority condition:
   *        ** MINIMUM COLOR DISTANCE FROM THE CURRENT PIXEL **
   *        Ties are broken first by minimum y-coordinate, then by minimum x-coordinate.
   *        The HSLAPixel.dist() function will be useful, and you should
   *        take advantage of the functionality in your PriorityNeighbours class.
   *
   *        If you process the neighbours in a different order, your fill may
   *        still work correctly, but your animations will be different
   *        from the grading scripts!
   *
   * 3.     For every k pixels filled, **starting at the kth pixel**, you
   *        must add a frame to the animation, where k = frameFreq.
   *
   *        For example, if frameFreq is 4, then after the 4th pixel has
   *        been filled you should add a frame to the animation, then again
   *        after the 8th pixel, etc.  You must only add frames for the
   *        number of pixels that have been filled, not the number that
   *        have been checked. So if frameFreq is set to 1, a pixel should
   *        be filled every frame.
   *
   * 4.     Finally, as you leave the function, send one last frame to the
   *        animation. This frame will be the final result of the fill, and
   *        it will be the one we test against.
   *
   */

  // complete your implementation below
  // HINT: you will likely want to declare some kind of structure to track
  //       which pixels have already been visited
   int framecount = 0; // increment after processing one pixel; used for producing animation frames (step 3 above)
  animation anim = animation();
  anim.addFrame(config.img);
  OrderingStructure<PixelPoint> os = OrderingStructure<PixelPoint>();
   
   

  vector<vector<unsigned int>> processed;
  for(unsigned int x = 0; x <config.img.width(); x++){
    vector<unsigned int> initial;
    for(unsigned int y = 0; y <config.img.height(); y++){
      initial.push_back(0);
    }
    processed.push_back(initial);
  }

  HSLAPixel* currPixel;
  os.Add(config.seedpoint);
  processed[config.seedpoint.x][config.seedpoint.y] = 1;
  HSLAPixel seedPixel = *config.img.getPixel(config.seedpoint.x, config.seedpoint.y);
  currPixel = config.img.getPixel(config.seedpoint.x, config.seedpoint.y);
  PixelPoint tempCurr = PixelPoint(config.seedpoint.x, config.seedpoint.y,*currPixel);

  *currPixel = (*config.picker)(tempCurr);

  PixelPoint actualCurr;
  PixelPoint top;
  PixelPoint left;
  PixelPoint bottom;
  PixelPoint right;


  while(!os.IsEmpty()){
    actualCurr = os.Remove();
    top = PixelPoint(actualCurr.x, actualCurr.y+1);
    bottom = PixelPoint(actualCurr.x, actualCurr.y-1);
    right = PixelPoint(actualCurr.x+1, actualCurr.y);
    left = PixelPoint(actualCurr.x-1, actualCurr.y);
    vector<PixelPoint> newPixels = {top, bottom, right, left};
    for (int i = 0; i<4; i++) {
      PixelPoint temp = newPixels[i];
      if(0<= temp.x && temp.x < config.img.width() && 0 <= temp.y && temp.y < config.img.height()){
        PixelPoint pxp = PixelPoint(temp.x,temp.y, *config.img.getPixel(temp.x,temp.y));
        config.neighbourorder.Insert(pxp);
      }
    }
    while(!config.neighbourorder.IsEmpty()) {
      PixelPoint pxp = config.neighbourorder.Remove();
      if(0<= pxp.x && pxp.x < config.img.width() && 0 <= pxp.y && pxp.y < config.img.height()){
        int val = processed[pxp.x][pxp.y];
        if(val == 0){
          processed[pxp.x][pxp.y] = 1;
          
          if(pxp.color.dist(seedPixel) <= config.tolerance){
            os.Add(pxp);
            framecount++;
            currPixel = config.img.getPixel(pxp.x,pxp.y);
            *currPixel = (*config.picker)(pxp);
            if(framecount% config.frameFreq == 0){
              anim.addFrame(config.img);
            }
          }
        }
      }
    }
  }
    
  anim.addFrame(config.img);
  return anim;
}

