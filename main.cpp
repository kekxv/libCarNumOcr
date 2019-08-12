
#include <getopt.h>
#include <string>
#include <cstdio>
#include <CarNumOcr.h>
#include <fstream>

using namespace std;


void help() {
    printf("使用说明：\n"
           "\t -f 输入的图片文件\n"
           "\t -m OpenCV xml 模型\n"
           "\t -c CaffeModel 模型 HorizonalFinemapping\n"
           "\t -s FreeRecognizer 模型 SegmentationFree \n"
    );
}


int main(int argc, char *argv[]) {
    int opt;
    string imagePath;
    string modelPath = "model";

    while ((opt = getopt(argc, argv, "f:m:")) != -1) {
        switch (opt) {
            case 'f':
                imagePath = optarg;
                break;
            case 'm':
                modelPath = optarg;
                break;
            default:
                help();
                return 0;
        }
    }
    if (imagePath.empty()) {
        printf("图片地址错误\n");
        return 1;
    }
    CarNumOcr carNumOcr(modelPath);
    auto carNums = carNumOcr.GetCarNum(imagePath);
    if (!carNums.empty()) {
        printf("该图片包含的车牌号有：\n");
        for (const auto &res : carNums) {
            printf("车牌识别结果：%12s ； 车牌准确度：%12lf;\n",
                   res.first.c_str(),
                   res.second
            );
        }
    } else {
        printf("该图片没有发现车牌！\n");
    }

    ifstream in(imagePath, ios::in | ios::binary);

    streampos pos = in.tellg();   //  save  current  position
    in.seekg(0, ios::end);
    size_t len = in.tellg();
    in.seekg(pos);   //  restore  saved  position
    unsigned char *_s = new unsigned char[len];
    in.read((char *) _s, len);


    carNums = carNumOcr.GetCarNum(_s,len);
    if (!carNums.empty()) {
        printf("该图片包含的车牌号有：\n");
        for (const auto &res : carNums) {
            printf("车牌识别结果：%12s ； 车牌准确度：%12lf;\n",
                   res.first.c_str(),
                   res.second
            );
        }
    } else {
        printf("该图片没有发现车牌！\n");
    }
    delete[] _s;

    in.close();

    return 0;
}