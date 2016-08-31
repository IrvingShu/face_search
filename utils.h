#ifndef UTILS_H
#define UTILS_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>


class Utils
{
public:
    Utils();
    std::vector<std::string> loadTxt(const std::string &path);

};

#endif // UTILS_H
