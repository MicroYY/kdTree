
#include "kdTree.h"
#include <iostream>

namespace kdTree
{
	kdTree::kdTree(kdTreeArray& inputData)
		:data(inputData),
		num(inputData.shape()[0]),
		dim(inputData.shape()[1]),
		root(NULL),
		leafIndex(num)
	{
		for (int i = 0; i < num; i++)
		{
			leafIndex[i] = i;
		}

		build_tree();
	}

	kdTree::~kdTree()
	{
		delete root;
	}

	void kdTree::build_tree()
	{
		root = build_sub_tree(0, num - 1, NULL);
	}

	kdTreeNode * kdTree::build_sub_tree(int lower, int upper, kdTreeNode* parent)
	{

		if (upper < lower)
			return  NULL;

		kdTreeNode* node = new kdTreeNode(dim);

		if ((upper - lower) <= bucketsize)
		{
			for (int i = 0; i < dim; i++)
			{
				RangeOfCoordinate(i, lower, upper, node->rangeVec[i]);
			}
			node->cut_dim = 0;
			node->cut_val = 0.0;
			node->lower = lower;
			node->upper = upper;
			node->left = node->right = NULL;
		}
		else
		{
			int cut_dim = 0;
			float max_range = 0.0;
			int middle_index;

			for (int i = 0; i < dim; i++)
			{
				if ((parent == NULL) || (parent->cut_dim == i))
				{
					RangeOfCoordinate(i, lower, upper, node->rangeVec[i]);
				}
				else
				{
					node->rangeVec[i] = parent->rangeVec[i];
				}
				float range_of_dim = node->rangeVec[i].ub - node->rangeVec[i].lb;
				if (range_of_dim > max_range)
				{
					max_range = range_of_dim;
					cut_dim = i;
				}
			}
			float sum = 0.0;
			float average;
			{
				for (int i = lower; i <= upper; i++)
				{
					sum += data[leafIndex[i]][cut_dim];
				}
				average = sum / static_cast<float>(upper - lower + 1);
				middle_index = MiddleIndex(cut_dim, average, lower, upper);
			}
			node->cut_dim = cut_dim;
			node->lower = lower;
			node->upper = upper;

			node->left = build_sub_tree(lower, middle_index, node);
			node->right = build_sub_tree(middle_index + 1, upper, node);
			///子树建立完成

			if (node->right == NULL)
			{
				for (int i = 0; i < dim; i++)
				{
					node->rangeVec[i] = node->left->rangeVec[i];
				}
				node->cut_val = node->left->rangeVec[cut_dim].ub;
				node->cut_left_val = node->cut_right_val = node->cut_val;
			}
			else if (node->left == NULL)
			{
				for (int i = 0; i < dim; i++)
				{
					node->rangeVec[i] = node->right->rangeVec[i];
				}
				node->cut_val = node->right->rangeVec[cut_dim].ub;
				node->cut_left_val = node->cut_right_val = node->cut_val;
			}
			else
			{
				node->cut_left_val = node->left->rangeVec[cut_dim].ub;
				node->cut_right_val = node->right->rangeVec[cut_dim].lb;
				node->cut_val = (node->cut_left_val + node->cut_right_val) / 2.0F;

				for (int i = 0; i < dim; i++)
				{
					node->rangeVec[i].ub = std::max(node->left->rangeVec[i].ub,
						node->right->rangeVec[i].ub);
					node->rangeVec[i].lb = std::min(node->left->rangeVec[i].lb,
						node->right->rangeVec[i].lb);
				}
			}
		}
		return node;
	}

	///计算每一维度的范围
	void kdTree::RangeOfCoordinate(int dimension, int lower, int upper, Range& range)
	{
		float fmin, fmax;
		float tempmin, tempmax;
		int i;

		fmin = data[leafIndex[lower]][dimension];
		fmax = fmin;

		for (i = lower + 2; i <= upper; i += 2)
		{
			tempmin = data[leafIndex[i - 1]][dimension];
			tempmax = data[leafIndex[i]][dimension];
			if (tempmax > tempmax)
				std::swap(tempmin, tempmax);
			if (fmin > tempmin)
				fmin = tempmin;
			if (fmax < tempmax)
				fmax = tempmax;
		}
		if (i == upper + 1)
		{
			float last = data[leafIndex[upper]][dimension];
			if (fmin > last)
				fmin = last;
			if (fmax < last)
				fmax = last;
		}
		range.lb = fmin;
		range.ub = fmax;
	}

	///计算中间值索引
	int kdTree::MiddleIndex(int dimension, float average, int lower, int upper)
	{
		int lb = lower, ub = upper;
		while (lb < ub)
		{
			if (data[leafIndex[lb]][dimension] <= average)
				lb++;
			else
			{
				std::swap(leafIndex[lb], leafIndex[ub]);
				ub--;
			}
		}
		if (data[leafIndex[lb]][dimension] <= average)
			return lb;
		else
			return lb - 1;
	}

	kdTreeNode::kdTreeNode(int dim) :rangeVec(dim)
	{
		left = right = NULL;
	}

	kdTreeNode::~kdTreeNode()
	{
		if (left != NULL)
			delete left;
		if (right != NULL)
			delete right;
	}

}
