#include <stdio.h>
#include <string>

#include "cv.h"
#include "highgui.h"
#include <ctype.h>

void setup();
void log(std::string str);
void run();
void cleanup();
// STUFF
void detect_and_draw(IplImage* img);

void cvOverlayImage(IplImage* src, IplImage* overlay, CvPoint location, CvScalar S, CvScalar D);