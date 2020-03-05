#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

#define M_PI 3.14159265358979323846

double* createKernel(double* kernel, double sigma)
{
	double sum = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			kernel[i + j + 2 * (i + 2)] = (double)(exp(-(i*i + j * j) / (sigma*sigma))) *(1 / (sigma*sqrt(2 * M_PI)));
			sum += kernel[i + j + 2 * (i + 2)];
		}
	}
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			kernel[i + j + 2 * (i + 2)] /= sum;
		}
	}
	return kernel;
}

cv::Mat duplicateBorder(cv::Mat image)
{

	cv::Mat new_image(image.rows + 2, image.cols + 2, CV_8UC1);
	new_image.at<uchar>(0, 0) = image.at<uchar>(0, 0);
	new_image.at<uchar>(0, new_image.cols - 1) = image.at<uchar>(0, image.cols - 1);
	new_image.at<uchar>(new_image.rows - 1, 0) = image.at<uchar>(image.rows - 1, 0);
	new_image.at<uchar>(new_image.rows - 1, new_image.cols - 1) = image.at<uchar>(image.rows - 1, image.cols - 1);
	for (int i = 0; i < image.cols; i++)
	{
		new_image.at<uchar>(0, i + 1) = image.at<uchar>(0, i);
		new_image.at<uchar>(new_image.rows - 1, i + 1) = image.at<uchar>(image.rows - 1, i);
	}
	for (int i = 0; i < image.rows; i++)
	{
		new_image.at<uchar>(i + 1, 0) = image.at<uchar>(i, 0);
		new_image.at<uchar>(i + 1, new_image.cols - 1) = image.at<uchar>(i, image.cols - 1);
	}
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
		{
			new_image.at<uchar>(i + 1, j + 1) = image.at<uchar>(i, j);
		}
	return new_image;
}

int main(int argc, char** argv) 
{
	
	std::string path_to_image;
	cv::Mat original, filter;
	std::cout << argc << " " << argv[0] << " "  << std::endl;
	if (argc < 2)
		path_to_image += "../../Image/test.png";
	else
		path_to_image += argv[1];
	original = cv::imread(path_to_image);
	if (!original.data)
	{
		std::cout << "Error load image" << std::endl;
		return -1;
	}
	if (original.channels()>1)
	{
		cv::cvtColor(original, original, cv::COLOR_BGR2GRAY);
		std::cout << "image to grey color" << std::endl;
	}
	namedWindow("Original",cv::WINDOW_NORMAL);
	namedWindow("Filter",cv::WINDOW_NORMAL);
	filter = duplicateBorder(original);
	cv::imshow("Original",original);
	cv::imshow("Filter", filter);

	cv::waitKey();
	return 0;
}