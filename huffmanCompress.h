#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <iomanip>

using namespace std;

class Huffman
{
private:
	class HuffTreeNode
	{
	public :
		char data;
		int freq;
		HuffTreeNode *left, *right;
		
		HuffTreeNode(char data, int freq = 0, HuffTreeNode *left = NULL, HuffTreeNode *right = NULL)
		{
			this->data = data;
			this->left = left;
			this->right = right;
			this->freq = freq;
		}
		~HuffTreeNode()
		{
			delete left;
			delete right;
		}
	};

	class compareNode
	{
	public :
		bool operator()(HuffTreeNode *a, HuffTreeNode *b)
		{
			return a->freq > b->freq;
		}
	};

public :
	Huffman();
	typedef HuffTreeNode *pHuffNode;

	vector< pair<char, int> > createFreq(ifstream & input);
	pHuffNode compressTree(vector< pair<char, int> >freq_vector);
	map<char, string> makeKeyMap(pHuffNode htree);
	void writeKeyMap(map<char, string> keymap);

	string encode(ifstream & input, map<char, string> keymap);

	void Huffman::makeFile(string fileName, string encoded);
	int getFileSize(string fileName);

private:
	pHuffNode myRoot;
};

inline Huffman::Huffman()
{
	myRoot = new HuffTreeNode('*');
}