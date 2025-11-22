#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include <atomic>
#include <stack>

struct Node
{
	std::pair<int, int> data;
	std::atomic<Node*> left;
	std::atomic<Node*> right;
	Node(const std::pair<int, int>& data) : data(data), left(nullptr), right(nullptr) {}
};

class MThrBST
{
	std::atomic<Node*> root;
	int threadCount;

	void subInsert(const std::vector<std::pair<int, int>>& data);
	void insertNode(const std::pair<int, int>& data);
	void free(Node* node);
	std::optional<int> searchOne(int key) const;
	void printSubtree(Node* node, int depth) const;

public:

	MThrBST(int threadCount = 1);
	~MThrBST();

	void insert(const std::vector<std::pair<int, int>>& data);
	std::vector<std::optional<int>> search(const std::vector<int>& keys);
	void print() const;
};

