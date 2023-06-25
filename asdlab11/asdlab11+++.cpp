#include <iostream> 
#include <string>
#include <map>
#include <fstream>


//первоначально конечный автомат находится в состоянии 0. Затем он по очереди читает символы из входной строки
//С конечным состоянием связана функция , называемая функцией конечного состояния, определяемая следующим образом: 
//есть состояние, в которое придет автомат (из начального состояния), прочитав строку 

//Простейший способ решения задачи – построить n конечных автоматов, по одному на каждый образец, и "пропустить" текст последовательно через эти автоматы

//сложность O(N+M*|Alphabet|)



using namespace std;

struct state
{
	int len, link;
	map<char, int> next;
};


const int MAXLEN = 100000;
state st[MAXLEN * 2];
int sz, last;



void sa_init()
{
	sz = last = 0;
	st[0].len = 0;
	st[0].link = -1;
	++sz;
}


void sa_extend(char c)
{
	int cur = sz++;
	st[cur].len = st[last].len + 1;
	int p;
	for (p = last; p != -1 && !st[p].next.count(c); p = st[p].link)
	{
		st[p].next[c] = cur;
	}

	if (p == -1)
	{
		st[cur].link = 0;
	}

	else
	{
		int q = st[p].next[c];
		if (st[p].len + 1 == st[q].len)
			st[cur].link = q;
		else
		{
			int clone = sz++;
			st[clone].len = st[p].len + 1;
			st[clone].next = st[q].next;
			st[clone].link = st[q].link;
			for (; p != -1 && st[p].next[c] == q; p = st[p].link)
				st[p].next[c] = clone;
			st[q].link = st[cur].link = clone;
		}
	}

	last = cur;
}


void getData(std::string& data, std::ifstream& file)
{
	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line))
		{
			data += line;
		}
	}
	else
	{
		std::cout << "File can't open!" << std::endl;
	}
}

bool find(string& text, string& pattern)
{
	int n = pattern.size();
	sa_init();

	for (int i = 0; i < text.size(); i++) // Сначала строим автомат для всего текста 
	{
		sa_extend(text[i]);
	}

	int cur = 0;
	bool result = true;

	for (int i = 0; i < pattern.size(); i++) // Бежим по строке и если не можем пройти по автомату в новое состояние, то строки не существует, если дошли до конца, то она есть
	{
		if (st[cur].next[pattern[i]])
		{
			cur = st[cur].next[pattern[i]];
		}
		else
		{
			result = false;
		}
	}

	return result;

}

void main()
{
	std::ifstream file;
	file.open("text_dm.txt");

	std::string text = "";
	getData(text, file);

	setlocale(LC_ALL, "rus");

	std::cout << "Введите строку для поиска: ";

	std::string pattern;
	std::cin >> pattern;



	std::cout << find(text, pattern) << std::endl;

}