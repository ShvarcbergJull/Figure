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

int ph(int i, int j, char** arr) 
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

void line(int n, queue<int> &numer, Num* numbers)
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

bool checkCircle(int &circle, int i, int j, char** arr)
{
	if (arr[i][j - 1] == '2' && arr[i][j + 1] == '2' && arr[i + 1][j] == '1' && arr[i + 1][j + 1] == '1' && arr[i + 1][j - 1] == '1')
	{
		circle++;
		return true;
	}
	return false;
}

bool checkTriang(int i, int j, char** &arr, Num* numbers, int &sumt)
{
	queue<int> need;
	int fi, fj, t, counter = 0, si = i, sj = j;
	line(5, need, numbers);
	bool hope = false;
	int n = 0;
	while (!need.empty())
	{
		n = need.front();
		need.pop();

		if (arr[i + numbers[n].i][j + numbers[n].j] == '3')
		{
			if (si == i && sj == j)
				return true;
			if (counter <= 4)
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

		if (arr[i + numbers[n].i][j + numbers[n].j] == '2')
		{
			fi = i + numbers[n].i;
			fj = j + numbers[n].j;
			i = fi;
			j = fj;
			arr[i][j] = '3';

			if (ph(i + numbers[0].i, j + numbers[0].j, arr) == 1)
			{
				fi = i + numbers[0].i;
				fj = j + numbers[0].j;
				while (ph(fi + numbers[0].i, fj + numbers[0].j, arr) != 2)
				{
					fi += numbers[0].i;
					fj += numbers[0].j;
					if (ph(fi, fj, arr) == 0)
						return false;
				}
				sumt++;
			}

			if (ph(i + numbers[4].i, j + numbers[4].j, arr) == 1)
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
	{
		rectang++;
		return true;
	}
	return false;
}

void limits(int i, int j, char** &arr, queue<int> numer, Num* numbers, int &triang, int &rectang, int &circle, int &count)
{	
	bool check = false;
	int gi, gj, mur = 0;
	int si = i, sj = j;
	while (!numer.empty())
	{
		int n = numer.front();
		numer.pop();
		int hope = ph(i + numbers[n].i, j + numbers[n].j, arr);
		if (hope == 2)
		{			
			int fi = i + numbers[n].i;
			int fj = j + numbers[n].j;
			if ((si != i || sj != j) && mur <= 4)
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
		if (hope == 1)
		{
			arr[i + numbers[n].i][j + numbers[n].j] = '2';

			line(n, numer, numbers);
			i += numbers[n].i;
			j += numbers[n].j;
		}
	}
}

void base(char** &arr, int &triang, int &rectang, int &circle)
{
	queue<int> numer;
	Num* numbers = new Num[8];
	numbers[0].i = 0;
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
	while (i < STR_LENGHT)
	{
		j = 0;
		while (j < STR_LENGHT)
		{
			switch (arr[i][j])
			{
			case '1':
				line(5, numer, numbers);
				limits(i, j, arr, numer, numbers, triang, rectang, circle, count);
				j++;
				break;
			case '2':
				j++;
				while (arr[i][j] == '1' && j < STR_LENGHT)
					j++;
				break;
			case '3':
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
