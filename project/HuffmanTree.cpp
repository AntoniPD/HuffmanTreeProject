#include "HuffmanTree.h"






HuffmanTree::HuffmanTree()
{
	tree = nullptr;
}

HuffmanTree::HuffmanTree(frequencyTable table)
{
	//*?
	vector<HuffmanTree*> trees;
	//create vector with with N huffman trees
	for (int i = 0; i < 256; ++i) {
		//if the char isn't '\0' => push a new huffman tree
		//invoking table[i], we create a new element in the map (for sure)
		if (table[i] > 0) {
			trees.push_back(new HuffmanTree(table[i], (unsigned char)i));
		}
	}
	while (trees.size() > 1) { 
		HuffmanTree min1 = findMinAndErase(trees);
		HuffmanTree min2 = findMinAndErase(trees);
		//the data of the new root 
		int sumRoots = (min1.tree->number + min2.tree->number);
		//it doesn't matter what is the symbol
		HuffmanTree* newTree = new HuffmanTree(sumRoots, min2.tree->symbol, min1.tree, min2.tree);
		trees.push_back(newTree);
	}
	tree = copyFrom(trees[0]->tree);
}

HuffmanTree::HuffmanTree(int num, unsigned char sym)
{
	tree = new Node(num, sym);
}

HuffmanTree::HuffmanTree(int num, unsigned char sym, Node * _left, Node * _right) 
{
	tree = new Node(num, sym, copyFrom(_left), copyFrom(_right));
}

HuffmanTree::HuffmanTree(const HuffmanTree& hfTree)
{
	tree = copyFrom(hfTree.tree);
}

HuffmanTree::~HuffmanTree()
{
	delete tree;
}

HuffmanTree HuffmanTree::findMinAndErase(vector<HuffmanTree*>& trees)
{
	int min = 1000;
	int index = 0;
	HuffmanTree* minTree = trees[0];
	//find the tree with smallest value
	for (int i = 0; i < trees.size(); ++i) {
		if (trees[i]->tree->number < min) {
			min = trees[i]->tree->number;
			minTree = trees[i];
			index = i;

		}
	}
	//save the tree to return it
	HuffmanTree* temp = new HuffmanTree(*minTree);
	//and remove it from the trees, so we can
	//continue with the others
	trees.erase(trees.begin() + index);
	return *temp;
}

//copies a tree in an other tree
Node * HuffmanTree::copyFrom(Node * root)
{
	if (!root) {
		return nullptr;
	}
	return new Node(root->number, root->symbol, copyFrom(root->left), copyFrom(root->right));
}

//all paths to the leafs exspressed in bits
vector<pair<unsigned char, string>> HuffmanTree::getPaths() 
{
	vector<pair<unsigned char, string>>paths;
	getPathsHelper(paths, tree, "");
	return paths;
}

//helper function
void HuffmanTree::getPathsHelper(vector<pair<unsigned char, string>>& paths, Node * root, string code)
{
	if (isLeaf(root)) {
		//a char is found, write the path
		paths.push_back({root->symbol, code});
		return;
	}

	if (root == nullptr) {
		//there is no path
		return;
	}

	getPathsHelper(paths, root->left, code + "0");
	getPathsHelper(paths, root->right, code + '1');

}

void HuffmanTree::printHelper(Node* curr) {
	if (curr) {
		std::cout << curr->symbol << " - " << curr->number << "  ";
		printHelper(curr->left);
		printHelper(curr->right);
	}
}
void HuffmanTree::print() {
	printHelper(tree);
	std::cout << "\n";
}

bool HuffmanTree::isLeaf(Node* root) {
	return (root != nullptr && root->left == nullptr && root->right == nullptr);
}

string HuffmanTree::compress(string input)
{
	//path to each char, in a string
	string binString;
	//fill binTable
	fillTable();
	for (int i = 0; i < input.size(); ++i) {
		binString.append(binTable[input[i]]);
	}
	//split the string into 8 portions of bits
	int iter = binString.size() / 8;
	string result;
	for (int i = 0; i < iter; ++i) {
		//pass each portion to convertToNum
		unsigned char ch = convertToNum(binString.substr(8 * i, 8));
		result.push_back(ch);
	}
	//takes the remained bits
	string remainder = binString.substr(8 * iter);

	//the binary code is needed for the decompression
	//saving it in a string, and return it with the already compressed input
	string printedTree = binTableStr();
	return remainder + printedTree + result;
}

//input is the data from binTable in string
string HuffmanTree::decompress(string bits, string input)
{
	//remove the bits from the begining
	string result;
	string reconverted = reconvert(input);
	finalString((reconverted + bits), result);
	return result;
}

//remove the sentinels from the string
vector<pair<unsigned char, string>> HuffmanTree::readTable(string & input)
{
	vector<pair<unsigned char, string>> result;
	//because input[0] = '(', that must be skipped
	int i = 1;
	unsigned char sym;
	//if ')' is reached, the string is read
	while (input[i] != ')') {
		sym = input[i];
		//skip '-'
		i += 2;
		string temp;
		//could be more than one bit
		while (input[i] != ' ') {
			temp += input[i];
			i++;
		}
		//push the symbol and his path(bits)
		result.push_back({ sym, temp });
		//skip the ' '
		i++;
	}
	//skip the ')'
	i++;
	//only the compressed string remains
	input = input.substr(i, input.size());
	return result;
}

HuffmanTree HuffmanTree::createDecompressedTree(vector<pair<unsigned char, string>> vec)
{
	HuffmanTree hfTree;
	for (int i = 0; i < vec.size(); ++i) {
		hfTree.addLeaf(vec[i].first, vec[i].second, hfTree.tree);
		//hfTree.print();
	}
	return hfTree;
}

bool HuffmanTree::insertElement(string trace, unsigned char x, Node *&crr)
{
	if (trace == "" && crr == nullptr)
	{
		crr = new Node(0, x, nullptr, nullptr);
		return true;
	}
	if (trace != "" && crr == nullptr)
	{
		return false;
	}
	//=> trace != "" && current != nullptr
	if (trace[0] == '0')
	{
		trace.erase(trace.begin());
		return insertElement(trace, x, crr->left);
	}
	if (trace[0] == '1')
	{
		trace.erase(trace.begin());
		return insertElement(trace, x, crr->right);
	}
	return false;
}

//convert char to bits
string toBinary(unsigned char ch) {
	string result;
	std::stack<unsigned char> st;
	while (ch != 0) {
		st.push(ch % 2 + '0');
		ch /= 2;
	}
	//fill
	while (st.size() < 8) {
		st.push('0');
	}
	while (!st.empty()) {
		result.push_back(st.top());
		st.pop();
	}
	return result;
}

//convert the chars to bits
string HuffmanTree::reconvert(string str)
{
	string result = "";
	unsigned char ch;
	for (int i = 0; i < str.size(); i++) {
		ch = str[i];
		result.append(toBinary(ch));
	}
	return result;
}


//the pair from tableToString is passed
void HuffmanTree::addLeaf(unsigned char ch, string str, Node *& curr)
{
	if (str == "") {
		//leaf => set char
		curr = new Node(0, ch);
		return;
	}

	if (curr == nullptr) {
		//create a temporary Node
		curr = new Node(0, 'g');
	}

	if (str[0] == '0') {
		//      char     bit
		addLeaf(ch, str.substr(1), curr->left);
	}
	else {
		//      char     bit
		addLeaf(ch, str.substr(1), curr->right);
	}
}

void HuffmanTree::finalString(string bin, string & result)
{
	unsigned char ch = 0;
	while (bin.size() != 0) {
		ch = getChar(tree, bin);
		result.push_back(ch);
	}
}

//going through tree, finding the char
unsigned char HuffmanTree::getChar(Node * root, string& file)
{
	if (isLeaf(root)) {
		return root->symbol;
	}

	if (file[0] == '0') {
		file = file.substr(1);
		return getChar(root->left, file);
	}else{
		file = file.substr(1);
		return getChar(root->right, file);
	}
}

void HuffmanTree::createTreeFromBinTableStr(string input)
{
	vector<pair<unsigned char, string>> vec;
	//read the '('
	int index = 1;
	while (input[index] != ')' && input.substr(index).size() > 2) {
		
		//char is at input[index]
		int i = index + 2;
		string bits;
		while (input[i] != ' ') {
			bits.push_back(input[i]);
			i++;
		}
		//std::cout << "first: " << input[index] << " second: " << bits;
		vec.push_back({ input[index], bits });
		index = 1 + i;
	}
	tree = copyFrom(createDecompressedTree(vec).tree);
	//print();
}

//table which contains a char and a path(in bits)
void HuffmanTree::fillTable()
{
	vector<pair<unsigned char, string>> paths = getPaths();
	for (int i = 0; i < paths.size(); ++i) {
		binTable.insert(paths[i]);
		tableToString.push_back(paths[i]);
	}
	
}

//convert bin to dec
unsigned char HuffmanTree::convertToNum(string str)
{
	int res = 0;
	for (int i = 0; i < str.size(); ++i) {
		res += (str[i] - '0')*pow(2, (str.size() - i - 1));
	}
	return (unsigned char)res;
}

//type the data from binTable into string
string HuffmanTree::binTableStr()
{
	string result;
	result.push_back('(');
	for (int i = 0; i < tableToString.size(); ++i) {
		result.push_back(tableToString[i].first);
		result.push_back('-');
		result.append(tableToString[i].second);
		result.push_back(' ');
	}
	result.push_back(')');
	return result;
}


