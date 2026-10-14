// CodeDecodeCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

void LoadFile(std::string);
void SaveResults(std::string);

void ParseFile(int);
void FileToBinary(int);
void BinaryToCrypted();
void AddDictionaryToCrypted();
int TranslateBinaryToInt(std::string);

void SetupBinaryTree();
std::vector<char> SubMergeSort(std::vector<char>, std::vector<char>);

struct BinaryTree
{
	bool isRoot = false;
	int root;
	char ascii = 0;
	BinaryTree* leftTree = nullptr;
	BinaryTree* rightTree = nullptr;
	
	void SetupDictionary(std::map<char, std::string>& dictionary, std::string binary)
	{
		if (ascii == 0)
		{
			leftTree->SetupDictionary(dictionary, binary + "0");
			rightTree->SetupDictionary(dictionary, binary + "1");
		}
		dictionary[ascii] = binary;
	}

	void Delete()
	{
		if (leftTree != nullptr)	leftTree->Delete();
		if (rightTree != nullptr)	rightTree->Delete();
		leftTree =	nullptr;
		rightTree = nullptr;
		if(!isRoot) delete(this);
	}
};

std::map<char, std::string> dictionary;
std::map<char, int> occurences;
std::vector<std::string> basicFile;
std::string binaryFile;
std::string cryptedFile;
BinaryTree tree;
int main()
{
	LoadFile("StartingFile.txt");
	for (int i = 0; i < 1; i++)
	{
		dictionary.clear();
		ParseFile(i);
		SetupBinaryTree();
		tree.SetupDictionary(dictionary, "");
		FileToBinary(i);
		BinaryToCrypted();
		AddDictionaryToCrypted();
		tree.Delete();
		std::cout << "Finished iteration n°" + std::to_string(i) + " / 20" << std::endl;
	}
	SaveResults("CryptedFile.txt");
}

void LoadFile(std::string name)
{
	std::fstream file;

	file.open(std::string(name).c_str(), std::ios::in);

	std::vector<std::string> row;
	std::string line, word, temp;
	std::string writingLine;

	while (getline(file, line))
	{
		row.clear();
		writingLine.clear();

		std::stringstream s(line);

		basicFile.push_back(line);
	}

	file.close();
}

void SaveResults(std::string name)
{
	//Writing on file
	std::fstream fout;
	fout.open("temp.txt", std::ios::out | std::ios::app);

	fout << cryptedFile;

	fout.close();
	remove(name.c_str());
	rename("temp.txt", name.c_str());

	std::cout << "Results SAVED!\n";
}

void ParseFile(int fileNumber)
{
	std::string line;
	std::vector<std::string>* usingFile;
	std::vector<std::string> secondpass;
	if (fileNumber == 0)
	{
		usingFile = &basicFile;
	}
	else 
	{
		secondpass.push_back(cryptedFile);
		usingFile = &secondpass;
	}
	for (int i = 0; i < usingFile->size(); i++)
	{
		line = usingFile->at(i);
		if (!line.empty())
		{
			for (char c : line)
			{
				if (occurences.find(c) == occurences.end())
				{
					occurences[c] = 0;
				}
				occurences[c]++;
			}
		}
	}
}

void FileToBinary(int fileNumber)
{
	std::string writingLine;
	std::string line;
	std::vector<std::string>* usingFile;
	std::vector<std::string> secondpass;
	if (fileNumber == 0)
	{
		usingFile = &basicFile;
	}
	else
	{
		secondpass.push_back(cryptedFile);
		usingFile = &secondpass;
	}
	binaryFile.clear();
	for (int i = 0; i < usingFile->size(); i++)
	{
		line = usingFile->at(i);
		writingLine.clear();
		if (!line.empty())
		{
			for (char c : line)
			{
				writingLine += dictionary[c];
			}
			binaryFile += writingLine;
		}
	}
}

void BinaryToCrypted()
{
	std::string writingLine;
	std::string octet;
	cryptedFile.clear();

	for (int i = 0; i < binaryFile.size(); i++)
	{
		octet += binaryFile[i];
		if (octet.size() >= 8)
		{
			writingLine += std::stoi(octet);
			octet.clear();
		}
	}
	for (char c : octet)
	{
		writingLine += c;
	}

	cryptedFile+= writingLine;
}

void AddDictionaryToCrypted()
{
	std::string dictionaryLine;
	for (auto m : dictionary)
	{
		dictionaryLine += m.first;
		dictionaryLine += "=";
		dictionaryLine += m.second;
		dictionaryLine += ";";
	}
	dictionaryLine += '\n';
	cryptedFile = dictionaryLine + cryptedFile;
}

int TranslateBinaryToInt(std::string octet)
{
	int result = 0;
	int addition = 128;
	for (char c : octet)
	{
		if (c == '1') result += addition - 1;
		addition / 2;
	}
	return result;
}

void SetupBinaryTree()
{
	char lowestChar;
	int lowestInt;
	std::vector<BinaryTree*> trees;
	for (auto c : occurences)
	{
		BinaryTree* newTree = new BinaryTree;
		newTree->root = c.second;
		newTree->ascii = c.first;
		trees.push_back(newTree);
	}

	BinaryTree* tempTreeA;
	BinaryTree* tempTreeB;
	while (trees.size() > 1)
	{
		tempTreeA = nullptr;
		tempTreeB = nullptr;
		for (BinaryTree* b : trees)
		{
			if (tempTreeA == nullptr)
			{
				tempTreeA = b;
				continue;
			}
			if (tempTreeB == nullptr)
			{
				tempTreeB = b;
				continue;
			}
			if (b->root < tempTreeA->root)
			{
				tempTreeA = b;
				continue;
			}
			if (b->root < tempTreeB->root)
			{
				tempTreeB = b;
				continue;
			}
		}
		BinaryTree* newRoot = new BinaryTree;
		newRoot->root = tempTreeA->root + tempTreeB->root;
		newRoot->leftTree = tempTreeA;
		newRoot->rightTree = tempTreeB;

		trees.erase(std::find(trees.begin(), trees.end(), tempTreeA));
		trees.erase(std::find(trees.begin(), trees.end(), tempTreeB));
		trees.push_back(newRoot);
	}
	tree = *trees[0];
	tree.isRoot = true;

	delete(trees[0]);
	trees[0] = nullptr;
	trees.clear();
}
