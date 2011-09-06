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

// Globals smobals
static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;
CvCapture *capture = 0;

IplImage *replacementFace = 0;
const char* replacementFaceFile = "./config/monkey.png";

// Images to capture the frame from video or camera or from file
IplImage *frame, *frame_copy = 0;
int       key = 0;

const char* cascade_name = "./config/haarcascade_frontalface_alt.xml";

int main (int argc, char const *argv[]) { 
  setup();
  
  run();
  return 0;
}

void run() {
  log("[0] run");
  if (capture) {
    for(;;) {
      if (!cvGrabFrame(capture)) break;
      frame = cvRetrieveFrame(capture);
      if (!frame) break;
      if(!frame_copy) frame_copy = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, frame->nChannels);
      
      if (frame->origin == IPL_ORIGIN_TL) cvCopy(frame, frame_copy, 0);
      else cvFlip( frame, frame_copy, 0 );
      
      detect_and_draw(frame_copy);
      
      if (cvWaitKey(1) >= 0) break;
    }
    cleanup();
  }
}

void detect_and_draw(IplImage* img) {
  int scale = 1;
  int i;
  
  IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3 );
  // Load the HaarClassifierCascade
  cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
  
  if (!cascade) {
    log("[1] Could not create a cascade");
    return;
  }

  cvClearMemStorage( storage );
  
  // Detect faces loop
  CvSeq* faces = cvHaarDetectObjects(img, cascade, storage, 1.1, 3, 0, cvSize(40, 40));
  for(i = 0; i < (faces ? faces->total : 0); i++)
  {
    // Create a new rectangle for drawing the face
    CvRect* face = (CvRect*)cvGetSeqElem( faces, i );
    
    /* draw a red rectangle */
    cvCircle(img, cvPoint(face->x, face->y), 1, CV_RGB(255,0,0), 1, 8, 0);
    
    CvPoint faceStart   = cvPoint(face->x, face->y);
    CvPoint faceCenter  = cvPoint(face->x + face->width,face->y + face->height);
    
    // cvRectangle(img, faceStart, faceCenter, CV_RGB(255, 0, 0), 1, 8, 0);
    
    cvOverlayImage(img, replacementFace, faceStart, 
        cvScalar(0.1,0.5,0.5,0.5), 
        cvScalar(0.1,0.5,0.5,0.5)
      );
  }
  
  // Show the image in the window named "result"
  cvShowImage( "result", img );

  // Release the temp image created.
  cvReleaseImage( &temp );
}

void log(std::string str) {
  fprintf(stderr, "%s\n", str.c_str());
}

void setup() {
  log("[0] Setup");
  // Allocate the memory storage
  storage = cvCreateMemStorage(0);
  capture = cvCaptureFromCAM(0);
  
  // replacementFace;
  replacementFace = cvLoadImage(replacementFaceFile);
  if(!replacementFace) printf("Could not load image file: %s\n",replacementFaceFile);
  
  cvNamedWindow( "result", 1 );
}

void cvOverlayImage(IplImage* src, IplImage* overlay, CvPoint location, CvScalar S, CvScalar D)
{
 int x,y,i;

  for(x=0;x < overlay->width -10;x++)
    {
        if(x+location.x>=src->width) continue;
        for(y=0;y < overlay->height -10;y++)
        {
            if(y+location.y>=src->height) continue;
            CvScalar source = cvGet2D(src, y+location.y, x+location.x);
            CvScalar over = cvGet2D(overlay, y, x);
            CvScalar merged;
            for(i=0;i<4;i++)
            merged.val[i] = (S.val[i]*source.val[i]+D.val[i]*over.val[i]);
            cvSet2D(src, y+location.y, x+location.x, merged);
        }
    }
}

void cleanup() {
  // Release the images, and capture memory
  cvReleaseImage( &frame_copy );
  cvReleaseCapture( &capture );
  cvReleaseHaarClassifierCascade( &cascade );
  // Destroy the window previously created with filename: "result"
  cvDestroyWindow("result");  
}