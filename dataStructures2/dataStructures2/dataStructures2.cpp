// dataStructures2.cpp : Defines the entry point for the console application.
//





#include "stdafx.h"
# include <iostream>
#include "UnorderedArray.h"
using namespace std;
int main()
{
	UnorderedArray<int>array1(5, 1 );
	array1.remove(3);
	//for (int i = 0; i < 5; i++)
	//{
		//array1[i].remove(i);
	//}
	int index = array1.search(0);
	std::cout << "index is equal to = " <<index << endl;
	system("pause");

	return 0;
}

