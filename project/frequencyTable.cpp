#include "frequencyTable.h"

frequencyTable::frequencyTable(string word)
{
	for (int i = 0; i < word.length(); ++i) {
		table[word[i]]++;
	}
}

frequencyTable::frequencyTable()
{
}

int & frequencyTable::operator[](unsigned char ch)
{
	return table[ch];
}

void frequencyTable::print() {
	for (int i = 0; i < table.size(); i++) {
		if (table[i] > 0) {
			std::cout << (unsigned char)i << table[i];
			std::cout << ' ' << std::endl;
		}
	}
	std::cout << "\n";
}

