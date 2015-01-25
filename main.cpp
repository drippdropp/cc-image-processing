#include <iostream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

typedef struct z_Point z_Point;
typedef struct Pixel Pixel;
typedef struct InputArgs InputArgs;

enum PxlChan {
  Blue,
  Green,
  Red,
  Alpha
};

struct z_Point {
  int* x;
  int* y;
  z_Point(int* x_, int* y_)
      : x{x_}, y{y_}{}
};

struct Pixel {
  int blue;
  int green;
  int red;
  Pixel(){}
  Pixel(int& blue_, int& green_, int& red_)
      : blue{ blue_ }, green{ green_ }, red{ red_ }{}
};

struct InputArgs {
  char* file_name;
  InputArgs()
      : file_name{ NULL }{}
  InputArgs(char* fn)
      : file_name{ fn }{}
  ~InputArgs() {
    if (this->file_name != NULL)
      delete[] file_name;
  }
};

void changeImageSize(Mat* src, Mat* dst, int width, int height);
void displayImageInWindow(Mat* dst);
void printPixel(const z_Point& point, const Pixel& pixel);
Pixel* constructPixel(const z_Point& point, Mat* img);
InputArgs* processArguments(int arg_count, char** arg_values);
int sumChannelValues(const PxlChan& channel, Mat* img);

int main(int argc, char** argv) {
  InputArgs inputArgs = *processArguments(argc, argv);
  if (inputArgs.file_name == NULL) {
    std::cerr << "You must include a filename as an argument." << std::endl;
    return 1;
  }

  Mat img;
  img = imread(inputArgs.file_name, 1);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < img.cols; j++) {
      z_Point activePoint{ &j, &i };
      Pixel* activePixel = constructPixel(activePoint,&img);
      printPixel(activePoint, *activePixel);
    }
  }

  return 0;
}

void changeImageSize(Mat* src, Mat* dst, int width, int height) {
  Size sz(width, height);
  resize(*src, *dst, sz);
}

void displayImageInWindow(Mat* img) {
  namedWindow("Image", CV_WINDOW_AUTOSIZE);
  imshow("Display Image", *img);
  waitKey(0);
}

void printPixel(const z_Point& point, const Pixel& pixel) {
  std::cout << *point.x << " "
            << *point.y << ": "
            << pixel.red << " "
            << pixel.green << " "
            << pixel.blue << std::endl;
}

Pixel* constructPixel(const z_Point& point, Mat* img) {
  Pixel* pixel = new Pixel();
  pixel->blue = (*img).at<cv::Vec3b>(*point.y, *point.x)[PxlChan::Blue];
  pixel->green = (*img).at<cv::Vec3b>(*point.y, *point.x)[PxlChan::Green];
  pixel->red = (*img).at<cv::Vec3b>(*point.y, *point.x)[PxlChan::Red];

  return pixel;
}

InputArgs* processArguments(int arg_count, char** arg_values) {
  InputArgs* input_args = new InputArgs();
  if (arg_count >= 2) {
    input_args->file_name = arg_values[1];
  }
  return input_args;
}