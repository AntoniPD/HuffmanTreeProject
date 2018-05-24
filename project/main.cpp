#include <iostream>
#include <fstream>
#include "frequencyTable.h"
#include "HuffmanTree.h"
#include "BinTable.h"

using std::ifstream;
using std::ofstream;

void compress(string inputFileName, string outputFileName = "text1.txt") {
	ifstream file;
	file.open(inputFileName, std::ios::binary);
	string input;
	while (!file.eof()) {
		char buffer;
		file.get(buffer);
		
		if (!file.eof()) {
			input.push_back(buffer);
		}
	}
	frequencyTable table(input);
	HuffmanTree tree(table);

	string compressed = tree.compress(input);

	ofstream outFile;
	outFile.open(outputFileName, std::ios::binary);
	outFile << compressed;
	outFile.close();
	file.close();
}

void decompress(string inFileName, string outFileName = "text2.txt") {
	ifstream file;
	file.open(inFileName, std::ios::binary);
	string input;
	while (!file.eof()) {
		char buffer;
		file.get(buffer);
		if (!file.eof()) {
			input.push_back(buffer);
		}
	}
	file.close();
	int start = 0;
	while (input[start] != '(') {
		start++;
	}
	int end = start;
	while (input[end] != ')') {
		end++;
	}
	string tableString = input.substr(start, end - start + 1);
	HuffmanTree hfTree;
	hfTree.createTreeFromBinTableStr(tableString);

	//std::cout << input.substr(end);
	string decompressed = hfTree.decompress(input.substr(0, start), input.substr(end + 1));
	ofstream ofFile;
	ofFile.open(outFileName, std::ios::binary);
	ofFile << decompressed;

	ofFile.close();
}

void UI() {
	string input;
	getline(std::cin, input);
	int compressIndex = input.find("-c ");
	int decompressIndex = input.find("-d ");
	bool isCompress = compressIndex != string::npos;
	bool isDecompress = decompressIndex != string::npos;
	string inputFile;
	string outputFile;
	int inputFileIndex = input.find("-i ");
	int outputFileIndex = input.find("-o ");
	bool foundInput = false;
	bool foundOutput = false;

	if (inputFileIndex != string::npos) {
		foundInput = true;
		inputFileIndex += 3;
		int i = inputFileIndex;
		//until the end of the file
		while (input[i] != ' ' && i < input.size()) {
			//take the name of the file
			inputFile.push_back(input[i]);
			i++;
		}
	}
	if (foundInput && outputFileIndex != string::npos) {
		foundOutput = true;
		outputFileIndex += 3;
		int i = outputFileIndex;
		while (input[i] != ' ' && i < input.size()) {
			outputFile.push_back(input[i]);
			i++;
		}
	}

	if (!foundInput) {
		std::cout << "No file" << std::endl;
	}
	else {
		if (!foundOutput) {
			if (isCompress) {
				compress(inputFile);
			}
			else if (isDecompress) {
				decompress(inputFile);
			}
			else {
				std::cout << "There's no command" << std::endl;
			}
		}
		else {
			if (isCompress) {
				compress(inputFile, outputFile);
			}
			else if (isDecompress) {
				decompress(inputFile, outputFile);
			}
			else {
				std::cout << "There's no command" << std::endl;
			}
		}
	}

}

int main() {
	//UI();
	compress("text.txt");
	decompress("text1.txt");
	return 0;
 }