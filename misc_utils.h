#include <opencv2/opencv.hpp>
#pragma once


float median(std::vector<float> v){
  int n = floor(v.size() / 2);
  nth_element(v.begin(), v.begin()+n, v.end());
  return v[n];
}

//Bounding Boxes
struct BoundingBox : public cv::Rect {
  BoundingBox(){}
  BoundingBox(cv::Rect r): cv::Rect(r){}
public:
  int sidx;             //scale index
  float confidence;
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
