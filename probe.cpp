#include "probe.h"

Probe::Probe(std::string labelfile)
{
    cv::Ptr<cv::ml::TrainData> label_data = cv::ml::TrainData::loadFromCSV(labelfile,0,-2,0);
    cv::Mat tempLabel = label_data->getTrainSamples();
    //mat to vector<float>
    for(int i=0; i<tempLabel.rows; ++i)
    {
        const float *data = tempLabel.ptr<float>(i);
        this->probelLabels.push_back(data[0]);
    }
}

std::vector<float> Probe::getProbeLabels()
{
    return this->probelLabels;
}

