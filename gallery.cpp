#include "gallery.h"

Gallery::Gallery()
{

}

Gallery::Gallery(const std::string& datafile, const std::string &labelfile,
                    const std::string &rangelist_file, const std::string &imgpath)
{
    cv::Ptr<cv::ml::TrainData> train_data;
    cv::Ptr<cv::ml::TrainData> label_data;
    train_data = cv::ml::TrainData::loadFromCSV(datafile, 0, -2, 0);
    this->m_galleryData = train_data->getTrainSamples();

    label_data = cv::ml::TrainData::loadFromCSV(labelfile,0,-2,0);
    cv::Mat tempLabel = label_data->getTrainSamples();
    //mat to vector<float>
    for(int i=0; i<tempLabel.rows; ++i)
    {
        const float *data = tempLabel.ptr<float>(i);
        this->m_galleryLabel.push_back(data[0]);
    }
    //get range list
    cv::Ptr<cv::ml::TrainData> rangelist_data;
    rangelist_data = cv::ml::TrainData::loadFromCSV(rangelist_file,0,-2,0);
    this->m_gallerRangeList = rangelist_data->getTrainSamples();
    // get img path
    Utils *utils = new Utils();
    this->m_imgPathSet= utils->loadTxt(imgpath);


}

cv::Mat Gallery::getGalleryData()
{
    return this->m_galleryData;
}

std::vector<float> Gallery::getGalleryLabel()
{
    return this->m_galleryLabel;
}

cv::Mat Gallery::getGalleryRangeList()
{
    return this->m_gallerRangeList;
}

std::vector<std::string> Gallery::getGalleryImgSet()
{
    return this->m_imgPathSet;
}

















