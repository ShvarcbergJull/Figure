#pragma once
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

const int STR_LENGHT = 200;

struct Num
{
	int i = 0;
	int j = 0;
};

bool readLine(ifstream& in, char* buffer)
{
	buffer[0] = 0;
	in.getline(buffer, STR_LENGHT + 1, '\n');
	if (in.fail())
	{
		if (buffer[0] > 0)
			cout << "Слишком длинная строка во входном файле" << endl;
		return false;
	}
	return true;
}

int ph(int i, int j, char** arr) //Проверяет не выходит ли за границы массива и возращает номер
{
	if (i > 0 && i < STR_LENGHT && j > 0 && j < STR_LENGHT)
	{
		if (arr[i][j] == '2')
			return 2;
		if (arr[i][j] == '1')
			return 1;
		return 0;
	}
	return -1;
}

void line(int n, queue<int> &numer)// создаёт порядок прохождения клеток
{
	while (!numer.empty())
		numer.pop();
	n -= 3;
	if (n < 0)
		n = 8 + n;
	for (int i = n; i < 8; i++)
		numer.push(i);
	for (int i = 0; i < n; i++)
		numer.push(i);
}

bool checkCircle(int &circle, int i, int j, char** arr)// проверка на круг
{
	if (arr[i][j - 1] == '2' && arr[i][j + 1] == '2' && arr[i + 1][j] == '1' && arr[i + 1][j + 1] == '1' && arr[i + 1][j - 1] == '1')
	{// если слева и справа двойки, и внизу все единицы, тогла круг
		circle++;
		return true;
	}
	return false;
}

bool checkTriang(int i, int j, char** &arr, Num* numbers, int &sumt)// проверка на треугольнки
{
	queue<int> need;
	int fi, fj, t, counter = 0, si = i, sj = j;
	line(5, need, numbers);// создаём порядок прохождения клеток(начинаем с верхней)
	int n;
	while (!need.empty())// пока не пройдём все точки вокруг, пройденные точки помечаем 3
	{
		n = need.front();
		need.pop();

		if (arr[i + numbers[n].i][j + numbers[n].j] == '3')// если вчтречаем тройку
		{
			if (si == i && sj == j)// проверяем обошли ли мы весь периметр фигуры
				return true;
			if (counter <= 4)// это для случаев, когда дольши до последней двойки, но от неё не идёт продолжение, поэтому мы возвращкемся назад
			{
				line(n, need, numbers);
				i += numbers[n].i;
				j += numbers[n].j;
				counter++;
				continue;
			}
			else
				return false;

		}

		if (arr[i + numbers[n].i][j + numbers[n].j] == '2')//если встретилась 2
		{
			i += numbers[n].i;// переходим в эту точку
			j += numbers[n].j;
			arr[i][j] = '3';// изменяет её

			if (ph(i + numbers[0].i, j + numbers[0].j, arr) == 1)// проверяем есть ли единицы слева
			{
				fi = i + numbers[0].i;
				fj = j + numbers[0].j;
				while (ph(fi + numbers[0].i, fj + numbers[0].j, arr) != 2)// идём по единицм, пока не встретим двойку
				{
					fi += numbers[0].i;
					fj += numbers[0].j;
					if (ph(fi, fj, arr) == 0)// если встречаем 0, то это не треугольник
						return false;
				}
				sumt++;// считаем сколько таких строк
			}

			if (ph(i + numbers[4].i, j + numbers[4].j, arr) == 1)// смотрим есть ли справа единицы, это для другой стороны треугольника(по идее этого можно было не делать наверное)
			{
				fi = i + numbers[4].i;
				fj = j + numbers[4].j;
				while (arr[fi + numbers[4].i][fj + numbers[4].j] != '3')
				{
					fi += numbers[4].i;
					fj += numbers[4].j;
					if (ph(fi, fj, arr) == 0)
						return false;
				}
				sumt++;
			}
			line(n, need, numbers);
		}

	}
	if (si == i && sj == j)
		return true;
	return false;
}

bool checkRectang(int &rectang, int i, int j, char** arr)
{
	char mt = arr[i + 1][j + 1];
	while(arr[i][j] == '2')
		j++;
	j--;
	if (mt == '1' && arr[i + 1][j] == '2' && arr[i + 1][j - 1] == '1')
	{// смотрим по 2 сторонам если относительно вверха по карям 2 и в углах единицы
		rectang++;
		return true;
	}
	return false;
}

void limits(int i, int j, char** &arr, queue<int> numer, Num* numbers, int &triang, int &rectang, int &circle)
{	
	bool check = false;
	int gi, gj, mur = 0;
	int si = i, sj = j;
	while (!numer.empty())// пока не обошли все точки вокруг
	{
		int n = numer.front();
		numer.pop();
		int hope = ph(i + numbers[n].i, j + numbers[n].j, arr);
		if (hope == 2)// если втретилась двойка
		{			
			int fi = i + numbers[n].i;
			int fj = j + numbers[n].j;
			if ((si != i || sj != j) && mur <= 4)// для треугольников с острыми углами
			{
				line(n, numer, numbers);
				i = fi;
				j = fj;
				mur++;
				continue;
			}

			if (si != i || sj != j)
				break;

			switch (arr[i + 1][j])
			{
			case '1':
				if (checkCircle(circle, i + 1, j, arr))
					break;
			case '2':
				if (checkRectang(rectang, i, j, arr))
					break;
			default:
				int sum = 1;

				if (checkTriang(i, j, arr, numbers, sum) && sum >= 2)
					triang++;

				cout << sum << endl;
				break;
			}
			break;
		}
		if (hope == 1)// если 1
		{
			arr[i + numbers[n].i][j + numbers[n].j] = '2';// изменяем на 2

			line(n, numer, numbers);// переделываем очередь для новой точки
			i += numbers[n].i;
			j += numbers[n].j;
		}
	}
}

void base(char** &arr, int &triang, int &rectang, int &circle)
{
	queue<int> numer;
	Num* numbers = new Num[8];
	numbers[0].i = 0;// заполнение массива numbers, который содержит на сколько изменить координаты
	numbers[0].j = -1;
	numbers[1].i = -1;
	numbers[1].j = -1;
	numbers[2].i = -1;
	numbers[2].j = 0;
	numbers[3].i = -1;
	numbers[3].j = 1;
	numbers[4].i = 0;
	numbers[4].j = 1;
	numbers[5].i = 1;
	numbers[5].j = 1;
	numbers[6].i = 1;
	numbers[6].j = 0;
	numbers[7].i = 1;
	numbers[7].j = -1;
	int i = 0, j = 0, count = 0;
	while (i < STR_LENGHT)// идём по всему массиву
	{
		j = 0;
		while (j < STR_LENGHT)
		{
			switch (arr[i][j])// смотрим что встречается
			{
			case '1':// начинаем очерчивать фигуру и проверять что это за фигура
				line(5, numer, numbers);
				limits(i, j, arr, numer, numbers, triang, rectang, circle, count);
				j++;
				break;
			case '2':// считаем что это фигура и пропускем её
				j++;
				while (arr[i][j] == '1' && j < STR_LENGHT)
					j++;
				break;
			case '3':// так же как и для 2
				j++;
				while (arr[i][j] == '1' && j < STR_LENGHT)
					j++;
				break;
			default:
				j++;
				break;
			}
		}
		i++;
	}
}
