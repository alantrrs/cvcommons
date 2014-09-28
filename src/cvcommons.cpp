
#include <cvcommons.h>

using namespace cv;

void boxDrawerCB(int event, int x, int y, int flags, void *param){
  boxDrawerParams *p = (boxDrawerParams*)param;
  switch( event ){
    case EVENT_MOUSEMOVE:
      if (p->drawing_box){
        p->box.width = x-p->box.x;
        p->box.height = y-p->box.y;
      }
      break;
    case EVENT_LBUTTONDOWN:
      p->drawing_box = true;
      p->box = cv::Rect( x, y, 0, 0 );
      break;
    case EVENT_LBUTTONUP:
      p->drawing_box = false;
      if( p->box.width < 0 ){
        p->box.x += p->box.width;
        p->box.width *= -1;
      }
      if( p->box.height < 0 ){
        p->box.y += p->box.height;
        p->box.height *= -1;
      }
      p->gotBB = true;
      break;
  }
}

//Get a bounding box
cv::Rect tagObject(std::string window,const cv::Mat& img){
  boxDrawerParams params;
  cv::setMouseCallback(window,boxDrawerCB,(void*)&params);
  cv::Mat canvas;
  while(!params.gotBB){
    img.copyTo(canvas);
    if (params.drawing_box)
      cv::rectangle(canvas,params.box,cv::Scalar(0,255,0));
    cv::imshow(window,canvas);
    cv::waitKey(30);
  }
  cv::setMouseCallback(window,NULL,NULL);
  return params.box;
}

// Clicks
void clickCB(int event, int x, int y, int flags, void *param){
  mouseParams *p = (mouseParams*)param;
  switch( event ){
    case EVENT_MOUSEMOVE:
      if (p->isDown){
        p->pt.x = x;
        p->pt.y = y;
      }
      break;
    case EVENT_LBUTTONDOWN:
      p->pt.x = x;
      p->pt.y = y;
      p->isDown = true;
      break;
    case EVENT_LBUTTONUP:
      p->isDown = false;
      break;
  }
}

//Add points
void addPointCB(int event, int x, int y, int flags, void *points){
  std::vector<cv::Point2f> *p = (std::vector<cv::Point2f>*)points;
  switch( event ){
  case EVENT_LBUTTONDOWN:
      p->push_back(cv::Point2f(x,y));
      break;
    }
}

//Get a 4 points
void getPoints(cv::Mat img,std::string view,std::vector<cv::Point2f>& imgPts){
  cv::setMouseCallback(view,addPointCB,(void*)&imgPts);
  int c=0;
  while(c<4){
    if (imgPts.size()>c){
      cv::circle(img,imgPts[c],3,cv::Scalar(0,255,0),2);
      c++;
    }
    cv::imshow(view,img);
    if (imgPts.size()==4)
      break;
    char key = cv::waitKey(30);
    if (key == 'r'){
      imgPts.clear();
      c=0;
    }
  }
  cv::setMouseCallback(view,NULL,NULL);
}

// Mask
cv::Mat getMask(const cv::Mat& img){
  mouseParams params;
  cv::namedWindow("Paint Mask");
  cv::setMouseCallback("Paint Mask",clickCB,(void*)&params);
  cv::Mat mask = cv::Mat::zeros(img.rows,img.cols,CV_8U);
  cv::Mat canvas;
  img.copyTo(canvas);
  while (true){
    if (params.isDown){
     cv::circle(mask,params.pt,7,cv::Scalar::all(255),-1);
     cv::circle(canvas,params.pt,7,cv::Scalar::all(255),-1);
    }
    cv::imshow("Paint Mask",canvas);
    char key = cv::waitKey(10);
    if (key == 'q')
      break;
  }
  cv::destroyWindow("Paint Mask");
  return mask;
}

// MISCELLANEOUS FUNCTIONS
cv::Point2f getCenter(const cv::Rect& box){
  return cv::Point2f(box.tl().x+box.width*0.5,box.tl().y+box.height*0.5);
}

float median(std::vector<float> v){
  int n = floor(v.size() / 2);
  nth_element(v.begin(), v.begin()+n, v.end());
  return v[n];
}

void Grid::build(){
  //Set Scales
  scales_.resize(2*no_scales_+1);
  scales_[no_scales_]=1;
  for (int i=1;i<=no_scales_;i++){
    scales_[no_scales_+i]=scales_[no_scales_+i-1]*scale_factor_;
    scales_[no_scales_-i]=scales_[no_scales_-i+1]/scale_factor_;
  }
  //Build Grid
  BoundingBox bbox;
  for (int s=0;s<scales_.size();s++){
    int width = round(box_size_.width*scales_[s]);
    int height = round(box_size_.height*scales_[s]);
    int min_bb_side = cv::min(height,width);
    if (min_bb_side < min_side_ || width > img_size_.width || height > img_size_.height)
      continue;
    for (int y=1;y<img_size_.height-height;y+=round(shift_*height)){
      for (int x=1;x<img_size_.width-width;x+=round(shift_*width)){
        bbox.x = x;
        bbox.y = y;
        bbox.width = width;
        bbox.height = height;
        bbox.sidx = s;
        boxes_.push_back(bbox);
      }
    }
  }
}
