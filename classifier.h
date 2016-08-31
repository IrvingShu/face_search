#ifndef CLASSIFIER
#define CLASSIFIER

#include "caffe/include/caffe/caffe.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

#include <iostream>

using namespace caffe;


using std::string;

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class Classifier {
 public:
  Classifier(const string& model_file,
             const string& trained_file);

  float getImgDistance(const cv::Mat& img1, const cv::Mat &img2);
   std::vector<float> getFeature(const cv::Mat& img);

 private:

  std::vector<float> Predict(const cv::Mat& img);

  void WrapInputLayer(std::vector<cv::Mat>* input_channels);

  void Preprocess(const cv::Mat& img,
                  std::vector<cv::Mat>* input_channels);
  float getEucDistance(const std::vector<float> &feature1,const std::vector<float> &feature2);

  //get feature


 private:
  shared_ptr<Net<float> > net_;
  cv::Size input_geometry_;
  int num_channels_;
  cv::Mat mean_;
  std::vector<string> labels_;
};

#endif // CLASSIFIER

