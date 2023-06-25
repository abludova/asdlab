#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

typedef std::vector<std::vector<int>> graph;
//выбираем одну вершину с меткой 0, а все остальные пусть будут с отметкой бесконечность
// Т.к используется метод динамического программирования, то будем вычислять сначала кратчайшие пути содержащие не более 1 ребра, потом кратчайшие пути содержащие
// не более 2х ребер и тд
//Если в графе нет циклов отрицательного веса, то кратчайший путь между любыми двумя вершинами содержит не более  ребра
//поэтому нужно вычислить значения F[n-1][i], которые и будут длинами кратчайших путей от вершины start до вершины i).
//О(n^3)

int to_int(std::string str) // Перевод числа  в инт
{
	int result = 0;
	if (str.size() == 2)
	{
		result = (str[0] - 48) * 10 + str[1] - 48;
	}
	else if (str.size() == 1)
	{
		result = str[0] - 48;
	}

	return result;
}


void fillMatrix(graph& matrix, std::ifstream& file, int n)
{
	if (file.is_open())
	{
		std::string line;
		int  k = 0;

		while (std::getline(file, line)) // заполение матрицы смежности
		{
			int cnt = 0;

			for (int i = 0; i < n - 1; i++)
			{
				int g = 0;

				if (line[i] == ' ')
				{
					for (int j = i + 1; j < line.size(); j++)
					{
						if (line[j] == ' ')
						{
							std::string test = line.substr(i + 1, j - i - 1);
							matrix[k][g] = to_int(line.substr(i + 1, j - i - 1));
							i = j;
							g++;
						}
					}
				}
			}

			k++;
		}
	}
	else
	{
		std::cout << "Error open file!" << std::endl;
	}
}


int countingTops(std::ifstream& file)
{
	int tops = 0;

	if (file.is_open())
	{
		file.seekg(0); //указатель на начало файла.
		std::string line;

		while (std::getline(file, line))
		{
			tops++;
		}
	}
	else
	{
		std::cout << "Error open file!" << std::endl;
	}

	return tops;
}


std::vector<int> BF(graph& matrix, int s) // Интерпретация графа как двумерного массива  ( в матрице смежности) и уже из этого  считаем мин расстояние
{									     // Здесь упрощенный вариант с массивом ребер 
	std::vector<int> ways(matrix.size(), 10000);

	ways[s] = 0;

	for (int k = 0; k < matrix.size(); k++)
	{
		for (int i = 0; i < matrix.size(); ++i)
		{
			for (int j = 0; j < matrix[i].size(); ++j)
			{
				if (matrix[i][j] != 0)
				{
					if (ways[j] > ways[i] + matrix[i][j])
					{
						ways[j] = ways[i] + matrix[i][j];
					}
				}
			}
		}
	}


	return ways;
}

void main()
{
	std::ifstream file;
	file.open("weigthed_graph.txt");
	int n = countingTops(file);//кол-во вершин в графе
	file.close();
	file.open("weigthed_graph.txt");
	graph matrix(n, std::vector<int>(n));
	fillMatrix(matrix, file, n);

	std::vector<std::vector<int>> allWays;

	for (int i = 0; i < matrix.size(); i++)
	{
		std::vector<int> ways = BF(matrix, i);
		allWays.push_back(ways);
	}

	for (auto way : allWays)
	{
		for (auto w : way)
		{
			std::cout << w << "\t";
		}

		std::cout << "\n";
	}


}