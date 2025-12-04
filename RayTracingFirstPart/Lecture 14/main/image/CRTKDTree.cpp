#include "CRTKDTree.h"

void CRTKDTree::build(const CRTAxisAlignedBB& box, const std::vector<CRTriangle>& triangles)
{
	nodes.clear();

	this->triangles = &triangles;
	addNode(box, -1, -1, {});

	std::vector<int> triangleIndices(triangles.size());
	for (unsigned int i = 0; i < triangles.size(); i++) 
	{
		triangleIndices[i] = i;
	}

	std::vector<CRTAxisAlignedBB> triangleBoxes(triangles.size());
	for (unsigned int i = 0; i < triangles.size(); i++) 
	{
		CRTAxisAlignedBB aabb;

		aabb.includePoint(triangles[i].getVertices()[0]);
		aabb.includePoint(triangles[i].getVertices()[1]);
		aabb.includePoint(triangles[i].getVertices()[2]);

		triangleBoxes.push_back(std::move(aabb));
	}


	addNode(box, -1, -1, {});
	buildRec(0, 0, triangleIndices, triangleBoxes);
}

const std::vector<CRTKDTreeNode>& CRTKDTree::getNodes() const
{
	return nodes;
}

int CRTKDTree::addNode(const CRTAxisAlignedBB& box, int leftChild, int rightChild, std::vector<int>&& triangleIndices)
{
	nodes.emplace_back(box, leftChild, rightChild, std::move(triangleIndices));
	return static_cast<unsigned int>(nodes.size() - 1);
}

void CRTKDTree::buildRec(int parent, int depth, const std::vector<int>& triangleIndices, const std::vector<CRTAxisAlignedBB>& triangleBoxes)
{
	if (depth >= MAX_KD_TREE_DEPTH || triangleIndices.size() >= MAX_BOX_TRIANGLE_COUNT)
	{
		nodes[parent].triangleIndices = triangleIndices;
		return;
	}

	auto pair = nodes[parent].aabb.split(depth % AXIS_COUNT);
	CRTAxisAlignedBB leftBox = pair.first;
	CRTAxisAlignedBB rightBox = pair.second;

	CRTKDTreeNode leftNode, rightNode;
	leftNode.aabb = leftBox;
	rightNode.aabb = rightBox;

	for (int i = 0; i < triangleIndices.size(); i++)
	{
		int index = triangleIndices[i];

		if (intersects(leftBox, triangleBoxes[index]))
		{
			leftNode.triangleIndices.push_back(index);
		}

		if (intersects(rightBox, triangleBoxes[index]))
		{
			rightNode.triangleIndices.push_back(index);
		}
	}

	if (leftNode.triangleIndices.size() > 0) 
	{
		int leftIdx = addNode(leftNode.aabb, -1, -1, {});
		nodes[parent].leftChild = leftIdx;
		buildRec(leftIdx, depth + 1, leftNode.triangleIndices, triangleBoxes);
	}

	if (rightNode.triangleIndices.size() > 0) 
	{
		int rightIdx = addNode(rightNode.aabb, -1, -1, {});
		nodes[parent].rightChild = rightIdx;
		buildRec(rightIdx, depth + 1, rightNode.triangleIndices, triangleBoxes);
	}
}
