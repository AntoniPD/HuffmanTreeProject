#pragma once
#include <iostream>
#include <map>
#include <string>

using std::string;
using std::map;

class frequencyTable
{
private:
	map<unsigned char, int> table;
public:
	frequencyTable(string word);

	frequencyTable();

	int& operator[](unsigned char ch);

	void print();
};

