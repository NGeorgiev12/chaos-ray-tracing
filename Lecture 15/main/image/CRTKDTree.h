#pragma once
#include "CRTAxisAlignedBB.h"
#include "../scene/CRTMesh.h"

struct CRTKDTreeNode
{
	CRTKDTreeNode() = default;
	CRTKDTreeNode(const CRTAxisAlignedBB& aabb, int leftChild, int rightChild,
		const std::vector<int>& triangleIndices)
		: aabb(aabb), leftChild(leftChild), rightChild(rightChild),
		triangleIndices(triangleIndices)
	{
	}

	CRTKDTreeNode(CRTAxisAlignedBB&& aabb, int leftChild, int rightChild,
		std::vector<int>&& triangleIndices)
		: aabb(std::move(aabb)), leftChild(leftChild), rightChild(rightChild),
		triangleIndices(std::move(triangleIndices))
	{
	}

	CRTAxisAlignedBB aabb;
	int parent = -1;
	int leftChild = -1;
	int rightChild = -1;
	std::vector<int> triangleIndices;
};

class CRTKDTree
{
public:

	CRTKDTree() = default;
	void build(const CRTAxisAlignedBB& box, const std::vector<CRTriangle>& triangles);

	const std::vector<CRTKDTreeNode>& getNodes() const;
private:

	std::vector<CRTKDTreeNode> nodes;
	const std::vector<CRTriangle>* triangles = nullptr;

	int addNode(const CRTAxisAlignedBB& box, int leftChild, int rightChild,
		std::vector<int>&& triangleIndices);
	void buildRec(int parent, int depth, const std::vector<int>& triangleIndices,
		const std::vector<CRTAxisAlignedBB>& triangleAABBs);

};

