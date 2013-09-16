
#include <opencv2/opencv.hpp>

// Draw box with the mouse
struct  boxDrawerParams{
  bool drawing_box;
  cv::Rect box;
  bool gotBB;
  boxDrawerParams(){
    gotBB = false;
    drawing_box = false;
  }
};

// Mouse Position params
struct mouseParams{
 cv::Point2f pt;
 bool isDown;
 mouseParams(){ 
   isDown = false;
 }
};

// GUI Functions
void boxDrawerCB(int event, int x, int y, int flags, void *param);
cv::Rect tagObject(std::string window,const cv::Mat& img);
void clickCB(int event, int x, int y, int flags, void *param);
void addPointCB(int event, int x, int y, int flags, void *points);
void getPoints(cv::Mat img,std::string view,std::vector<cv::Point2f>& imgPts);
cv::Mat getMask(const cv::Mat& img);


// MISC Functions

cv::Point2f getCenter(const cv::Rect& box);

float median(std::vector<float> v);

//Bounding Boxes
struct BoundingBox : public cv::Rect {
  BoundingBox(){}
  BoundingBox(cv::Rect r): cv::Rect(r){}
public:
  int sidx;             //scale index
  int object_id;
  float confidence;
  float density;
  float overlap;
};

struct Grid{
  std::vector<BoundingBox> boxes_;
  std::vector<float> scales_;
  cv::Size img_size_;
  cv::Size box_size_;
  int min_side_;
  float scale_factor_;
  int no_scales_;
  float shift_;
  Grid(){};
  Grid(cv::Size img_size, cv::Size box_size, int min_side=15,
       float scale_factor=1.2,int no_scales=5, float shift=0.1):
       img_size_(img_size),box_size_(box_size),min_side_(min_side),
       scale_factor_(scale_factor),no_scales_(no_scales),shift_(shift){ }
  void build();
};
