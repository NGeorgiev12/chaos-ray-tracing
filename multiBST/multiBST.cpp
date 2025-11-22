// multiBST.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include "MThrBST.h"

void task1()
{
	MThrBST tree(4);
	tree.insert({ {5,50}, {3,30}, {7,70}, {2,20}, {4,40}, {6,60}, {8,80} });
	tree.print();
	auto res = tree.search({ 5, 4, 10, 6 });
	for (auto& el : res)
	{
		if (el.has_value())
		{
			std::cout << el.value() << "\n";
		}
	}
}

void task2()
{
	MThrBST tree(2);

	tree.insert({ {1,100}, {2,200}, {3,300} });
	tree.print();

	tree.insert({ {2,222} });

	tree.print();
}

void task3() 
{
	MThrBST tree(4);

	std::vector<std::pair<int, int>> bulkData;
	for (int i = 1; i <= 1000; i++) {
		bulkData.emplace_back(i, i * 10);
	}

	tree.insert(bulkData);
	std::vector<int> keys;
	for (int i = 0; i < 20; i++)
	{
		keys.push_back(i);
	}

	auto results = tree.search(keys);

	for (size_t i = 0; i < results.size(); i++) 
	{
		if (results[i])
			std::cout << "Key " << keys[i] << " => " << results[i].value() << "\n";
		else
			std::cout << "Key " << keys[i] << " not found\n";
	}
}

void task4() 
{

	MThrBST single(1);
	MThrBST multi(4);

	std::vector<std::pair<int, int>> bigData;
	for (int i = 1; i <= 50000; i++)
		bigData.emplace_back(i, i * 2);

	auto start = std::chrono::high_resolution_clock::now();
	single.insert(bigData);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Single-thread insert: "
		<< std::chrono::duration<double>(end - start).count() << " sec\n";

	start = std::chrono::high_resolution_clock::now();
	multi.insert(bigData);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Multi-thread insert (4 threads): "
		<< std::chrono::duration<double>(end - start).count() << " sec\n";

}

int main()
{
	task4();
}
