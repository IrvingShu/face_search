#include "utils.h"

Utils::Utils()
{

}

std::vector<std::string> Utils::loadTxt(const std::string &path)
{
    std::ifstream testImgFile(path.c_str());
    std::string imgName;
    std::vector<std::string> imgPathSet;
    while(std::getline(testImgFile, imgName))
    {
        imgPathSet.push_back(imgName);
    }
    return imgPathSet;
}
