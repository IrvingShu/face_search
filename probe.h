#ifndef PROBE_H
#define PROBE_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

class Probe
{
public:
    Probe(std::string labelfile);
    std::vector<float> getProbeLabels();
private:
    std::vector<float> probelLabels;
};

#endif // PROBE_H
