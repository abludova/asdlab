#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


//выбираем одну вершину с меткой 0, а все остальные пусть будут с отметкой бесконечность
// просматриваем ее соседей, путь до них равен сумме пути до 1 (0) и указанному пути до них
//меняем их отметки на длину пути из вершины один до этих вершин
// переходим на следующую минимальную вершину и рассматриваем ее соседей
// путь до них равен сумме пути до вершины 1, пути от 1+2 и пути от вершины 2 до них, проставляем новые значения, если сосед 2  
//является соседом 1 и путь от 1 до соседа короче, то значение не меняем
// продолжаем цикл
//записываем кратчайшие пути до указанной вершины в массивчик и узнаем длину кратчайшего пути
// в ответе указаны кратчайшие длины до всех вершин и самый последний соответсвенно до 6 вершины
//О(n^2+m)

typedef std::vector<std::vector<int>> graph;

int to_int(std::string str)
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
		file.seekg(0); // указатель на начало файла.
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

bool isAllChecked(std::vector<bool> used)
{
	bool result = true;

	for (auto c : used)
	{
		if (c == false)
		{
			result = false;
		}
	}

	return result;
}

void dijstra(graph& matrix, int s, std::vector<int>& d, std::vector<bool>& used)
{
	int minValue = 10000;
	int c = s; //Текущая обрабатываемая верщина
	d[s] = 0;

	for (int i = 0; i < d.size(); i++) // Ставим очень  большое расстояние для всех вершин (так как ищем min)
	{
		if (used[i] == false && d[i] < minValue)
		{
			c = i;
			minValue = d[i];
		}
	}

	for (int i = 0; i < matrix[c].size(); i++) //Релаксация для соседей (выбираем лучший вариант для min расстояния)
	{
		if (matrix[c][i] != 0 && d[i] > d[c] + matrix[c][i])
		{
			d[i] = d[c] + matrix[c][i];
		}
	}

	used[c] = true;
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
	std::vector<bool> used(n);
	std::vector<int> d(n);

	for (int i = 0; i < d.size(); i++)
	{
		d[i] = 10000;
	}
	int s = 0; //Расстояние от текущей вершины

	while (!isAllChecked(used)) // Пока не посетили все вершины
	{
		dijstra(matrix, s, d, used); //Ищем для выбранной вершины min расстояние, при желании можно  пробегаться циклом по веришнам и записывать расстояния в двумерный массив
	}

	for (auto dist : d)
	{
		std::cout << dist << std::endl;
	}
}