#include <iostream>
#include "span.h"
#include <vector>
#include <array>
#include <span>
void print(michael::span<int> span) {
	for (auto&& elem : span)
		std::cout << elem << " ";
	std::cout << "\n";
}
void print2(std::span<int> span) {
	for (auto&& elem : span)
		std::cout << elem << " ";
	std::cout << "\n";
}
int main() {
	//comparing its API and behavior with that of std::span
	int numbers[]{ 1,2,3,4, 5, 6};
	michael::span<int> v(numbers);
	michael::span m(numbers);
	std::span<int> w(numbers);
	std::span n(numbers);
	std::vector<int> numbers1{ 1,2,3,4,5,6 };
	std::array<int, 6> numbers2{ 1,2,3,4,5,6 };
	michael::span<int> a = numbers1;
	std::span<int> b = numbers1;
	michael::span c{ numbers2 };
	std::span d{ numbers2 };
	b = d;
	a = c;
	print2(numbers1);
	print(numbers1);
	std::span f(numbers2.data(), 3);
	michael::span g(numbers2.data(), 3);
	std::span<int> t = d.subspan(2, 4);
	michael::span<int> s = c.subspan(2, 4);
	print(f);
	print(g);
	print2(t);
	print(s);
	print2(d.subspan(2, 4));
	print(c.subspan(2, 4));
}