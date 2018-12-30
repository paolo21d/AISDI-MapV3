#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include<chrono>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{
template <typename K, typename V>
using Tree = aisdi::TreeMap<K, V>;
template <typename K, typename V>
using Hash = aisdi::HashMap<K, V>;

void testTree(const std::size_t repeatCount) {
	Tree<int, int> treeMap;
	//auto start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		treeMap[i] = i;
	}
	auto start = std::chrono::system_clock::now();
	treeMap[repeatCount] = repeatCount;
	auto done = std::chrono::system_clock::now();
	std::cout << "TreeMap dodanie elementow: " << (done - start).count() << '\n';
	//Wyszukiwanie
	//start = std::chrono::system_clock::now();
	/*for (std::size_t i = 0; i < repeatCount; ++i) {
		treeMap.find(i);
	}*/
	start = std::chrono::system_clock::now();
	treeMap.find(repeatCount);
	done = std::chrono::system_clock::now();
	std::cout << "TreeMap wyszukiwanie elementow: " << (done - start).count() << '\n';
}
void testHash(const std::size_t repeatCount) {
	Hash<int, int> hashMap;
	//auto start = std::chrono::system_clock::now();
	for (std::size_t i = 0; i < repeatCount; ++i) {
		hashMap[i] = i;
	}
	auto start = std::chrono::system_clock::now();
	hashMap[repeatCount] = repeatCount;
	auto done = std::chrono::system_clock::now();
	std::cout << "HashMap dodanie elementow: " << (done - start).count() << '\n';
	//Wyszukiwanie
	//start = std::chrono::system_clock::now();
	/*for (std::size_t i = 0; i < repeatCount; ++i) {
		hashMap.find(i);
	}*/
	start = std::chrono::system_clock::now();
	hashMap.find(repeatCount);
	done = std::chrono::system_clock::now();
	std::cout << "HashMap wyszukiwanie elementow: " << (done - start).count() << '\n';
}
} // namespace

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1000000;
  std::cout << "Test dla powtorzen: " << repeatCount << std::endl;

  testTree(repeatCount);
  testHash(repeatCount);
  return 0;
}