#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#define DLIB_JPEG_SUPPORT

#include "dlib-18.18/dlib/image_processing/frontal_face_detector.h"
#include "dlib-18.18/dlib/image_processing/render_face_detections.h"
#include "dlib-18.18/dlib/image_processing.h"
#include "dlib-18.18/dlib/gui_widgets.h"
#include "dlib-18.18/dlib/image_io.h"
#include "dlib-18.18/dlib/opencv.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

//using namespace dlib;
using std::string;

class FaceDetector
{
public:
    FaceDetector(const string& shape_predictor_model);
    std::vector<cv::Mat> getFaceImg(const string& imgName);
private:
    dlib::frontal_face_detector detector;
    dlib::shape_predictor sp;

};

#endif // FACEDETECTOR_H
