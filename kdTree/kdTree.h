#pragma once

#include <vector>

#include <boost/multi_array.hpp>
#include <boost/array.hpp>


namespace kdTree
{
	typedef boost::multi_array<float, 2> kdTreeArray;

	class kdTree;
	class kdTreeNode;

	struct Range
	{
		float lb, ub;
	};


	class kdTree
	{
	public:
		kdTree(kdTreeArray& inputData);
		~kdTree();

		const kdTreeArray& data;
		const int num;
		const int dim;


	private:
		static const int bucketsize = 12;

		kdTreeNode* root;
		std::vector<int> leafIndex;

		void build_tree();
		kdTreeNode* build_sub_tree(int lower, int upper, kdTreeNode* parent);
		void RangeOfCoordinate(int dimension, int lower, int upper, Range& range);
		int MiddleIndex(int dimension, float average, int lower, int upper);
	};

	class kdTreeNode
	{
	public:
		kdTreeNode(int dim);
		~kdTreeNode();

	private:
		friend class kdTree;

		int cut_dim;	//´ýÇÐ¸îÎ¬¶È
		float cut_val, cut_left_val, cut_right_val;
		int lower, upper;

		std::vector<Range> rangeVec;

		kdTreeNode* left;
		kdTreeNode* right;
	};
}

