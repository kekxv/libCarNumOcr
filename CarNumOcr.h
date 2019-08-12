//
// Created by caesar on 2019-08-12.
//

#ifndef CARNUM_CARNUMOCR_H
#define CARNUM_CARNUMOCR_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace cv {
    class Mat;
}
namespace pr {
    class PlateDetection;

    class FineMapping;

    class SegmentationFreeRecognizer;

    class PlateSegmentation;

    class CNNRecognizer;
}

class CarNumOcr {
public:
    explicit CarNumOcr(const string &modelPath = "model");
    ~CarNumOcr();

    vector<std::pair<std::string, float>> GetCarNum(std::string &image);
    vector<std::pair<std::string, float>> GetCarNum(unsigned char *image,size_t imageLen);

private:
    vector<std::pair<std::string, float>> GetCarNum(cv::Mat &image);


    pr::PlateDetection *plateDetection = nullptr;//(modelPath + "/cascade.xml");
    pr::FineMapping *fineMapper = nullptr;//(modelPath + "/HorizonalFinemapping.prototxt",
    //modelPath + "/HorizonalFinemapping.caffemodel");
    pr::SegmentationFreeRecognizer *recognizr = nullptr;//(modelPath + "/SegmentationFree.prototxt",
    //modelPath + "/SegmentationFree.caffemodel");
    pr::PlateSegmentation *plateSegmentation = nullptr;//(modelPath + "/Segmentation.prototxt",
    //modelPath + "/Segmentation.caffemodel");
    pr::CNNRecognizer *cnnRecognizer = nullptr;//(modelPath + "/CharacterRecognization.prototxt",
    //modelPath + "/CharacterRecognization.caffemodel");
};


#endif //CARNUM_CARNUMOCR_H
