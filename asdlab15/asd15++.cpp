#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <string>


//Дан произвольный полностью неориентированный плоский граф G. Можно ли каждую вершину графа G раскрасить с помощью одной из четырех красок так,
//чтобы никакие две смежные вершины (вершины, соединенные хотя бы одним ребром) не были раскрашены в один цвет

//Граф обязательно должен быть полностью неориентированным без кратных ребер и петель.
//Наименьшее число цветов, необходимое для раскраски графа G, называется его хроматическим числом
//Сам последовательный алгоритм позволяет получить оценку сверху для хроматического числа.
//Найденное количество цветов для раскраски будет верхней границей, т.е хроматическое число меньше либо равно. 

//шаги 1. Упорядочим вершины по невозрастанию степени   2. Окрасим первую вершину в цвет 1   3. Выберем цвет окраски 1    
//4. Пока не покрашены все вершины, повторять: Окрасить в выбранный цвет всякую вершину, которая не смежная с другой, уже окрашенной  в этот цвет
//иначе выбрать следующий цвет



typedef std::vector<std::vector<int>> graph;

int fillMatrix(graph& matrix, std::ifstream& file, int n)
{
	if (file.is_open())
	{
		file.seekg(0); //указатель на начало файла.
		std::string line;
		int i = 0;

		while (std::getline(file, line)) // заполение матрицы смежности
		{
			for (int j = 0; j < n; j++)
			{
				matrix[i][j] = line.at(j) - 48;
			}

			i++;
		}
	}
	else
	{
		std::cout << "Error open file!" << std::endl;
	}

	return matrix.size();
}


int countingTops(std::ifstream& file)
{
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line);
		return line.size();
	}
	else
	{
		std::cout << "Error open file!" << std::endl;
		return -1;
	}
}


bool isNear(graph& matrix, std::vector <std::pair<int, int>>& colorTops, int t, int cnt) // Если рядом уже есть такой цвет
{
	bool result = false;

	for (int i = 0; i < colorTops.size(); i++)
	{
		int j = colorTops[i].first;

		if (matrix[t][j] != 0 && t != j && colorTops[i].second == cnt)
		{
			result = true;
		}
	}

	return  result;
}


bool isAllChecked(graph& matrix, std::vector <std::pair<int, int>>& powerTops)
{
	bool result = true;

	for (int i = 0; i < powerTops.size(); i++)
	{
		for (int j = 0; j < powerTops.size(); j++)
		{
			int k = powerTops[i].first;
			int g = powerTops[j].first;

			if (matrix[k][g] != 0)
			{
				result = false;
			}
		}
	}

	return result;
}


int color(graph& matrix) //  раскраска графа
{
	std::vector<std::pair<int, int>> powerTops; //Вершина, степень

	for (int i = 0; i < matrix.size(); i++) // Заполнение массива вершин-степеней
	{
		int power = 0;

		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] != 0)
			{
				power++;
			}
		}

		powerTops.push_back(std::pair<int, int>(i, power));
	}

	bool swapped = false; // Соритровка пузырьком вершин по их степеням
	do
	{
		swapped = false;

		for (int i = 0; i < powerTops.size() - 1; i++)
		{
			if (powerTops[i].second < powerTops[i + 1].second)
			{
				std::swap(powerTops[i], powerTops[i + 1]);
				swapped = true;
			}
		}
	} while (swapped);

	int cnt = 1;
	std::vector<std::pair<int, int>> colorTops; // Вершина, цвет

	colorTops.push_back({ powerTops[0].first, cnt });

	while (!powerTops.empty()) // Пока можем красить одним цветом( нет соседних  вершин с таким же цветом), то красим, после меняем цвет 
	{
		std::vector<std::pair<int, int>> del;

		for (int i = 0; i < powerTops.size(); i++)
		{
			if (!isNear(matrix, colorTops, powerTops[i].first, cnt))
			{
				colorTops.push_back({ powerTops[i].first,cnt });
				del.push_back(powerTops[i]);
			}
		}


		for (int i = 0; i < del.size(); i++)
		{

			powerTops.erase(std::find(powerTops.begin(), powerTops.end(), del[i]));
		}

		cnt++;
	}

	return cnt - 1; // Вычитаем едиинцу, тк после завершения раскраски увеличивается на 1 счетчик
}

void main()
{

	std::ifstream file;
	file.open("simplegraph.txt");
	int n = countingTops(file); //кол-во вершин в графе
	graph matrix(n, std::vector<int>(n));
	fillMatrix(matrix, file, n);  //Заполнение матрицы смежности

	std::cout << color(matrix) << std::endl; // Вывод максимального кол-ва цветов для раскраски графа
}