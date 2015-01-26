#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  VideoCapture cap(0);

  if (!cap.isOpened()) {
    std::cout << "Cannot open the webcam" << std::endl;
    return 1;
  }

  namedWindow("Control", CV_WINDOW_AUTOSIZE);
  int iLowH = 0, iHighH = 179, iLowS = 0, iHighS = 255, iLowV = 0, iHighV = 255;

  cvCreateTrackbar("LowH", "Control", &iLowH, 179);
  cvCreateTrackbar("HighH", "Control", &iHighH, 179);
  cvCreateTrackbar("LowS", "Control", &iLowS, 255);
  cvCreateTrackbar("HighS", "Control", &iHighS, 255);
  cvCreateTrackbar("LowV", "Control", &iLowV, 255);
  cvCreateTrackbar("HighV", "Control", &iHighV, 255);

  while (true)
  {
    Mat imgOriginal;
    bool bSuccess = cap.read(imgOriginal);

    if (!bSuccess) {
      std::cout << "Cannot read a frame from video stream" << std::endl;
      break;
    }

    Mat imgHSV;

    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

    Mat imgThresholded;

    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)) );

    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5,5)) );

    imshow("Thresholded Image", imgThresholded);
    imshow("Original", imgOriginal);

    if (waitKey(30) == 27) {
      std::cout << "esc key was pressed by the user" << std::endl;
      break;
    }

  }
  return 0;
}