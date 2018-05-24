#pragma once
#include <vector>
#include "frequencyTable.h"
#include <utility>
#include <iostream>
#include <cmath>
#include <stack>
//#include "BinTable.h"

using std::pair;
using std::vector;

struct Node {
	int number;
	unsigned char symbol;
	Node* left;
	Node* right;

	Node(int num, unsigned char sym) : number(num), symbol(sym), left(nullptr), right(nullptr) {}

	Node(int num, unsigned char sym, Node* _left, Node* _right) : number(num), symbol(sym), left(_left), right(_right) {}

	~Node() {
		delete left;
		delete right;
	}
};

class HuffmanTree
{
private:
	Node* tree;
	//could not make binTable as a class
	map<unsigned char, string> binTable;
	//in the map, the char and the string can not be reached
	vector<pair<unsigned char, string>> tableToString;


	//helper functions
	void getPathsHelper(vector<pair<unsigned char, string>>& paths, Node* root, string code);

	void printHelper(Node* curr);

	void fillTable();

	unsigned char convertToNum(string str);

public:
	HuffmanTree();
	HuffmanTree(frequencyTable table);
	HuffmanTree(int num, unsigned char sym);
	HuffmanTree(int num, unsigned char sym, Node* _left, Node* _right);
	HuffmanTree(const HuffmanTree& hfTree);
	~HuffmanTree();
	//TODO
	HuffmanTree findMinAndErase(vector<HuffmanTree*>& trees);

	Node* copyFrom(Node* root);


	vector<pair<unsigned char, string>> getPaths();
	

	bool isLeaf(Node* root);

	string compress(string input);

	string decompress(string bits, string input);

	vector<pair<unsigned char, string>> readTable(string& input);

	HuffmanTree createDecompressedTree(vector<pair<unsigned char, string>> vec);

	void addLeaf(unsigned char ch, string str, Node*& curr);

	bool insertElement(string trace, unsigned char x, Node*& crr);

	string reconvert(string str);

	void finalString(string bin, string& result);

	unsigned char getChar(Node* root, string& file);
	
	void createTreeFromBinTableStr(string input);

	string binTableStr();

	void print();
};

