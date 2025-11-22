#include "MThrBST.h"
#include <thread>

void MThrBST::subInsert(const std::vector<std::pair<int, int>>& data)
{
	for (const auto& node : data)
	{
		insertNode(node);
	}
}

void MThrBST::insertNode(const std::pair<int, int>& data)
{
	int key = data.first;
	int value = data.second;

	Node* newNode = new Node(data);
	Node* expected = nullptr;

	if (root.compare_exchange_strong(expected, newNode))
	{
		return;
	}
	delete newNode;

	Node* current = root.load();

	while (true)
	{
		if (key == current->data.first)
		{
			current->data.second = value; 
			return;
		}
		else if (key < current->data.first)
		{
			expected = nullptr;
			Node* left = current->left.load();
			if (!left)
			{
				Node* n = new Node(data);
				if (current->left.compare_exchange_strong(expected, n))
					return;

				else
					delete n;
			}
			current = current->left.load();
		}
		else
		{
			expected = nullptr;
			Node* right = current->right.load();
			if (!right)
			{
				Node* n = new Node(data);
				if (current->right.compare_exchange_strong(expected, n))
					return;
				else
					delete n;
			}
			current = current->right.load();
		}
	}
}

void MThrBST::free(Node* node)
{
	if (!node) return;

	std::stack<Node*> stack;
	stack.push(root);

	while (!stack.empty()) {
		Node* node = stack.top();
		stack.pop();
		if (node->left) stack.push(node->left);
		if (node->right) stack.push(node->right);
		delete node;
	}
}

std::optional<int> MThrBST::searchOne(int key) const
{
	Node* curr = root.load();
	while (curr)
	{
		if (key == curr->data.first)
			return curr->data.second;
		else if (key < curr->data.first)
			curr = curr->left.load();
		else
			curr = curr->right.load();
	}
	return std::nullopt;
}

MThrBST::MThrBST(int threadCount) : root(nullptr)
{
	int realThreadCount = std::thread::hardware_concurrency();
	if (threadCount > realThreadCount || threadCount < 1)
	{
		throw std::invalid_argument("Invalid number of threads");
	}

	this->threadCount = threadCount;
}

MThrBST::~MThrBST()
{
	free(root.load());
}

void MThrBST::insert(const std::vector<std::pair<int, int>>& data)
{
	std::vector<std::thread> threads;
	int chunkSize = data.size() / threadCount;

	for (int i = 0; i < threadCount; ++i)
	{
		int startIdx = i * chunkSize;
		int endIdx = i != threadCount - 1 ? chunkSize * (i + 1) : data.size();
		threads.emplace_back(&MThrBST::subInsert, this, std::vector<std::pair<int, int>>(data.begin() + startIdx, data.begin() + endIdx));
	}

	for (auto& thread : threads) 
	{
		thread.join();
	}
}

std::vector<std::optional<int>> MThrBST::search(const std::vector<int>& keys)
{
	std::vector<std::optional<int>> results(keys.size());
	std::vector<std::thread> threads;
	int chunkSize = keys.size() / threadCount;

	for (int i = 0; i < threadCount; i++) 
	{
		int from = i * chunkSize;
		int to = i != threadCount - 1 ? chunkSize * (i + 1) : keys.size();

		threads.emplace_back([&, from, to]
			{
				for (size_t i = from; i < to; i++)
				{
					results[i] = searchOne(keys[i]);
				}
			});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	return results;
}

void MThrBST::print() const
{
	printSubtree(root.load(), 0);
}

void MThrBST::printSubtree(Node* node, int depth) const
{
	if (!node)
	{
		return;
	}

	printSubtree(node->left.load(), depth + 1);
	std::cout << node->data.first << "->" << node->data.second << "\n";
	printSubtree(node->right.load(), depth + 1);
}
