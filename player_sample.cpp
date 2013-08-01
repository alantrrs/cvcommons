#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

const char* keys = {
"{ v  |video     |null| video file }"
"{ i  |init_frame|0| initial frame }"
};

int main(int argc, char * argv[]){
  VideoCapture cam;
 //Read options
  CommandLineParser parser(argc, argv, keys);
  string video = parser.get<string>("v");
  int f0 = parser.get<int>("i");
  //Init cam
  if (video != "null")
    cam.open(video);
  else
    cam.open(0);
  if (!cam.isOpened()){
	  cout << "cam device failed to open!" << endl;
    return 1;
  }
  //Read first frame
  Mat frame;
  cam.set(CV_CAP_PROP_POS_FRAMES,f0);
  cam.read(frame);
  while(cam.read(frame)){
  	//Display
    imshow("Player", frame);
    char key = cvWaitKey(33);
    if (key == 'q')
      break;
  }
  cam.release();
  return 0;
}

