#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
const char* keys = {
"{ v  |video     |null| video file }"
};

int main(int argc, char * argv[]){
  VideoCapture capture;
 //Read options
  CommandLineParser parser(argc, argv, keys);
  string video = parser.get<string>("v");
  //Init capture
  if (video != "null")
    capture.open(video);
  else
    capture.open(0);
  if (!capture.isOpened()){
	  cout << "Capture device failed to open!" << endl;
    return 1;
  }
  
  Mat frame;
  while(capture.read(frame)){
  	//Display
    imshow("Player", frame);
    char key = cvWaitKey(33);
    if (key == 'q')
      break;
  }
  capture.release();
  return 0;
}

