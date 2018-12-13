#include <vector>
#include <algorithm>
#include <iostream>

int main()
{
	std::vector<double> v{ 1.0, -2.0, 3.0, 4.0, 5.0, 1.0, 2.0, 3.0, 4.0, 5.0 };

	std::vector<double>::iterator biggest = std::max_element(std::begin(v), std::end(v));
	std::cout << "Max element is " << *biggest << " at position " << std::distance(std::begin(v), biggest) << std::endl;

	auto smallest = std::min_element(std::begin(v), std::end(v));
	std::cout << "min element is " << *smallest << " at position " << std::distance(std::begin(v), smallest) << std::endl;

	system("pause");
	return 0;
}

