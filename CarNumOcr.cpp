//
// Created by caesar on 2019-08-12.
//

#include "CarNumOcr.h"
#include <opencv2/opencv.hpp>
#include <PlateDetection.h>
#include <FineMapping.h>
#include <SegmentationFreeRecognizer.h>
#include <Pipeline.h>

using namespace cv;
using namespace pr;


static const std::vector<std::string> chars{"京", "沪", "津", "渝", "冀", "晋", "蒙", "辽", "吉", "黑", "苏", "浙", "皖", "闽", "赣",
                                            "鲁", "豫", "鄂",
                                            "湘", "粤", "桂", "琼", "川", "贵", "云", "藏", "陕", "甘", "青", "宁", "新", "0", "1",
                                            "2", "3", "4",
                                            "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "J", "K",
                                            "L", "M", "N",
                                            "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};


string CarNumOcr(Mat &ImgCarNum, pr::PlateSegmentation &plateSegmentation, CNNRecognizer &cnnRecognizer) {
    cv::Mat demo = ImgCarNum;
    cv::resize(demo, demo, cv::Size(136, 36));
    cv::Mat respones;
    pr::PlateInfo plate;
    plate.setPlateImage(demo);
    std::vector<cv::Rect> rects;
    plateSegmentation.segmentPlatePipline(plate, 1, rects);
    plateSegmentation.ExtractRegions(plate, rects);
    cnnRecognizer.SegmentBasedSequenceRecognition(plate);
    return plate.decodePlateNormal(chars);
}

vector<std::pair<std::string, float>> CarNumOcr::GetCarNum(Mat &image) {
    vector<std::pair<std::string, float>> CarNum;
    if (image.empty()) {
        return CarNum;
    }

    std::vector<pr::PlateInfo> plates;
    plateDetection->plateDetectionRough(image, plates);
    if (plates.empty()) {
        int pr = 0;
        if (image.rows > image.cols) {
            pr = image.rows / image.cols;
        } else {
            pr = image.cols / image.rows;
        }
        printf("%d", pr);
        if (pr > 2 && pr < 15) {
            std::pair<std::string, float> res = recognizr->SegmentationFreeForSinglePlate(image, pr::CH_PLATE_CODE);
            CarNum.push_back(res);
        }
    } else {
        for (pr::PlateInfo platex:plates) {
            Mat img = platex.getPlateImage();
            cv::Mat ImgCarNum = pr::FineMapping::FineMappingVertical(img);

            ImgCarNum = fineMapper->FineMappingHorizon(ImgCarNum, 0, 0);
//        string _t = getCarNum(ImgCarNum, plateSegmentation, cnnRecognizer);
//        CarNum.push_back(_t);
            std::pair<std::string, float> res = recognizr->SegmentationFreeForSinglePlate(ImgCarNum, pr::CH_PLATE_CODE);
            CarNum.push_back(res);
        }
    }
    return CarNum;
}

CarNumOcr::CarNumOcr(const string &modelPath) {
    plateDetection = new PlateDetection(modelPath + "/cascade.xml");
    fineMapper = new pr::FineMapping(modelPath + "/HorizonalFinemapping.prototxt",
                                     modelPath + "/HorizonalFinemapping.caffemodel");
    recognizr = new pr::SegmentationFreeRecognizer(modelPath + "/SegmentationFree.prototxt",
                                                   modelPath + "/SegmentationFree.caffemodel");
    plateSegmentation = new pr::PlateSegmentation(modelPath + "/Segmentation.prototxt",
                                                  modelPath + "/Segmentation.caffemodel");
    cnnRecognizer = new pr::CNNRecognizer(modelPath + "/CharacterRecognization.prototxt",
                                          modelPath + "/CharacterRecognization.caffemodel");
}

CarNumOcr::~CarNumOcr() {
    delete plateDetection;
    delete fineMapper;
    delete recognizr;
    delete plateSegmentation;
    delete cnnRecognizer;
}

vector<std::pair<std::string, float>> CarNumOcr::GetCarNum(std::string &image) {
    auto img = imread(image);
    return GetCarNum(img);
}

vector<std::pair<std::string, float>> CarNumOcr::GetCarNum(unsigned char *image, size_t imageLen) {
    std::vector<unsigned char> buf(image, image + imageLen);
    auto img = imdecode(buf, cv::ImreadModes::IMREAD_ANYCOLOR);
    return GetCarNum(img);
}
