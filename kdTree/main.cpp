#include <opencv2/opencv.hpp>

#include <boost/multi_array.hpp>
#include <boost/array.hpp>

#include "kdTree.h"

#define WIDTH 600
#define HEIGHT 400

typedef boost::multi_array<float, 2> MyArray;
typedef cv::Point2d point;

void DrawPoints(point* pointlist, int num, cv::Mat& mat)
{
	for (int i = 0; i < num; i++)
	{
		point temp_point(pointlist[i].x + 50, 350 - pointlist[i].y);
		cv::circle(mat, temp_point, 1, cv::Scalar(255, 0, 0), -1);
	}
}


int main()
{
	int dim = 2;
	int num = 50;

	double* M = (double*)calloc(dim*num, sizeof(double));
	int k = 0;
	for (double i = 0; i < 500; i += 500.0 / num)
	{
		M[2 * k] = i;
		M[2 * k + 1] = rand() / (double)(RAND_MAX / 300);
		k++;
	}

	MyArray my_data;
	my_data.resize(boost::extents[num][dim]);
	for (int m = 0; m < num; m++)
		for (int n = 0; n < dim; n++)
			my_data[m][n] = (float)M[m*dim + n];

	kdTree::kdTree* my_tree;
	my_tree = new kdTree::kdTree(my_data);

	int query = 10;

	kdTree::kdTreeResult result;
	my_tree->NearestAroundTreeNode(query, 0, result);
	//kdTree::kdTreeResultVector resultVector;
	//my_tree->NNearestAroundTreeNode(query, 0, 2, resultVector);

	//std::vector<float> qv = { 100,52.3394f };
	//kdTree::kdTreeResultVector result;
	//my_tree->NNearestAroundPoint(qv, 2, result);



	cv::Mat my_image = cv::Mat::zeros(400, 600, CV_8UC3);

	point* my_point = (point*)malloc(num * sizeof(point));
	for (int i = 0; i < num; i++)
	{
		my_point[i].x = M[2 * i];
		my_point[i].y = M[2 * i + 1];
		std::cout << my_point[i] << std::endl;
	}
	DrawPoints(my_point, num, my_image);

	cv::Point pt1(my_point[query].x + 50, 350 - my_point[query].y);
	cv::Point pt2(my_point[result.idx].x + 50, 350 - my_point[result.idx].y);
	cv::line(my_image, pt1, pt2, cv::Scalar(255, 0, 0));

	cv::imshow("", my_image);
	cv::waitKey();

	system("pause");
	return 0;
}