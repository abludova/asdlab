#include <iostream>
#include <vector>
#include <algorithm>



bool subset(std::vector<int>& s, int t) // Жадный алгоритм - приближенное решение. t-  ожидаемая сумма подмножества
{
	std::vector<int> S = {}; // Искомое подмножество
	std::sort(s.rbegin(), s.rend()); // СОритруем от большего к меньшему

	for (int i = 0; i < s.size(); i++)
	{
		if (t >= s[i])
		{
			S.push_back(s[i]); // Заполняем подмножества пока можем
			t -= s[i];

		}
	}

	return  t == 0 ? true : false;
}

void main()
{
	std::vector<int> s = { 20,1,12,2,15 };

	std::cout << subset(s, 6) << std::endl;
}