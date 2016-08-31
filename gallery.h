#ifndef GALLERY_H
#define GALLERY_H

#include <opencv2/opencv.hpp>

#include <iostream>
#include "utils.h"

class Gallery
{
public:
    Gallery();
    Gallery(const std::string& datafile, const std::string &labelfile, const std::string &rangelist_file, const std::string &imgpath);
    cv::Mat getGalleryData();
    std::vector<float> getGalleryLabel();
    cv::Mat getGalleryRangeList();
    std::vector<std::string> getGalleryImgSet();

private:
    cv::Mat m_galleryData;
    std::vector<float> m_galleryLabel;
    cv::Mat m_gallerRangeList;
    std::vector<std::string> m_imgPathSet;

};

#endif // GALLERY_H
