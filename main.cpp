#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include"filew.h"

using namespace std;

int main()
{
	ifstream in("input1.txt");
	int n = 0;
	char buffer[STR_LENGHT + 1];
	buffer[STR_LENGHT] = 0;
	char** arr = new char*[STR_LENGHT+1];
	for (int i = 0; i < STR_LENGHT; i++)
	{
		arr[i] = new char[STR_LENGHT + 1];
		arr[i][STR_LENGHT] = 0;
	}

	int i = 0;
	while (readLine(in, buffer))
	{
		strcpy(arr[i], buffer);
		i++;
	}

	int triang = 0, rectang = 0, circle = 0;

	base(arr, triang, rectang, circle);

	ofstream out("out.txt");

	cout << "Trianges: " << triang << ", Rectangels: " << rectang << ", Circles: " << circle<<endl;

	for (int i = 0; i < STR_LENGHT; i++)
		out << arr[i] << '\n';

	system("pause");
	return 0;
}
