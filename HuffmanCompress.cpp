#include "huffmanCompress.h"

// Building Frequencies from text
//�о�� ������ ó������ ������ �����鼭 map�� ���ڿ��� key�� �ؼ� freq�� �ִ´�. 
vector< pair<char, int> > Huffman::createFreq(ifstream & input) {
	map<char, int> freq_map;
	char ch;
	vector < pair<char, int> > freq_vector;
	cout << "creating Frequencies...wait\n";
	//cout << "\ntable map : [ ";		//�׽�Ʈ��
	for (;;) {
		input >> ch;
		if (input.eof()) break;	//���ڸ� ������ ���о����� break
								//freq_map���� ���ڸ� ã�Ƽ� ������ freq_map�� �� ���ڸ� key, freq =1�� �ְ�, ������ �ش� ���ڿ� ���� freq 1���� 
		if (freq_map.find(ch) == freq_map.end()) {
			freq_map.insert(make_pair(ch, 1));
		}
		else {
			freq_map[ch] += 1;	//insert�� �ƴ� operator ���. ch�� Ű�� ������ value�� 1�� ������
		}
		//cout << "{" << ch << ":" << freq_map[ch] << "}" << ",";		//�׽�Ʈ��1
	}
	//cout << " ]";	//�׽�Ʈ��
	cout << "Done!!! making frequencies for each characters\n" << endl;
	/*�ݺ��� iterator�� ����� ó������ ������ �˻��ϸ鼭 freq_vec ť�ȿ� key���� freq���� �־���, end()�� �����̳��� ���κ� ���� �ϳ� �� ���� ��ġ!!*/
	/* <,> �� ����ؼ� �� ����... vector,deque�� ������(random access�� �����ؾ���). ���� != ���� ���ؾߵ�!!*/
	for (map<char, int>::iterator it = freq_map.begin(); it != freq_map.end(); ++it) {
		freq_vector.push_back(make_pair(it->first, it->second));
	}

	return freq_vector;
}

//Building an encoding tree with the character & frequency pair	
/*���ڿ� �󵵼��� ������ Ʈ�� ����.*/
Huffman::pHuffNode Huffman::compressTree(vector< pair<char, int> > freq_vector) {

	/*freq_vec�ȿ� �ִ� ���� priority queue������ �־������ν� freq�� ���� ���ĵǰ� ��*/
    priority_queue<Huffman::HuffTreeNode , vector<Huffman::pHuffNode >, Huffman::compareNode > comp_tree;
    for (vector< pair<char, int> >::iterator it = freq_vector.begin(); it != freq_vector.end(); ++it) {
        Huffman::pHuffNode leaf = new Huffman::HuffTreeNode(it->first, it->second);
        comp_tree.push(leaf);
    }

	int i = 1; //�׽�Ʈ��2

	/*comp_tree priority queue���� ó������ ���� ���� ������Ʈ�� ��带 ����� ���� ����־��ش�.*/
    Huffman::pHuffNode root = NULL;
    while (comp_tree.size() > 1) {
        Huffman::pHuffNode L, R;
        L = comp_tree.top();
        comp_tree.pop();
        R = comp_tree.top();
        comp_tree.pop();
		//cout << "i : " << i << " / LEFT : " << L->freq << "(freq) , " << L->data << "(data) / " << "RIGHT : " << R->freq << "(freq) , " << R->data << "(data)" << endl;	//�׽�Ʈ��
		i++;
        root = new Huffman::HuffTreeNode(0, L->freq + R->freq, L, R);
		//cout << "\t" << "root : " << root->freq << " / L-child : " <<  root->left->data << "(data) , " << root->left->freq << "(freq) / " 
		//	<< "R-child : " << root->right->data << "(data) , " << root->right->freq << "(freq)" << "\n" << endl;	//�׽�Ʈ��
        comp_tree.push(root);
    }

    return root;
}

// Build the lookup table, ���ĵ� ���·� �ڷ�����(map ���)
/*map���� 1��° ���ڴ� key��, 2��° ���ڴ� data. ���� first,second ���·� ����.*/
map<char, string> Huffman::makeKeyMap(Huffman::pHuffNode htree) {
	map<char, string> key_map;
	deque< pair<Huffman::pHuffNode, string> > huff_queue;
	//int i = 0, j = 0, n = 2;	//�׽�Ʈ��7
	cout << "Making key map now...wait\n";
	huff_queue.push_back(make_pair(htree, ""));		//make_pair �� ������ ť�� ����ִ� �Լ�. 2���� �迭 ���� ����.

	/*huff_queue�� ��尡 �������� ���������� ��带 key_map �� �ȿ� �־��ش�.*/
	while (!huff_queue.empty()) {
		Huffman::pHuffNode node, left, right;
		string key;
		node = huff_queue.front().first;
		key = huff_queue.front().second;
		huff_queue.pop_front();
		left = node->left;
		right = node->right;

		//if (n - i == 1)j++;  //�׽�Ʈ��11


		if (left) {							//left child�� �����.
			string tmp_key = key;
			huff_queue.push_back(make_pair(left, key.append("0")));			//left�� ������ 0�� ���̰�
			huff_queue.push_back(make_pair(right, tmp_key.append("1")));		//right�� ������ 1�� ���δ�.
			//cout << "==" << j << "�� , " << i << "��° �θ���==" << endl;																//�׽�Ʈ��8
			//cout << "node L-child : " << node->left->data <<"(data) , "<< node->left->freq << "(freq)" << " , code" << code << endl;		//�׽�Ʈ��5
			//cout << "node R-child : " << node->right->data << "(data) , " << node->right->freq << "(freq)" << "\n" << endl;				//�׽�Ʈ��6
			//i++;		//�׽�Ʈ��10


		}
		else {
			key_map.insert(make_pair(node->data, key));		//���̻� �ڽ� ��尡 ������ �װ� ���̹Ƿ� data(����)�� code(�󵵼�)�� key_map map �ȿ� �־��ش�.
		}
		//if (i > n) n *= 2;	//�׽�Ʈ��12
	}

	cout << "encoding map had built!!! \n" << endl;
	return key_map;
}

//������ ���ڵ�ü�踦 ���� ������ �������.
void Huffman::writeKeyMap(map<char, string> keymap) {
	std::ofstream key_file("encoding.key");
	if (!key_file)
	{
		cout << "ofstream failed" << endl;
		return;
	}
	for (map<char, string>::iterator it = keymap.begin(); it != keymap.end(); ++it) {
		key_file << it->first << " " << it->second << "\n";
	}
	key_file.close();
}

//encodes the input using code lookup table
string Huffman::encode(ifstream & input, map<char, string> keymap) {

	string encoded;
	char ch;
	cout << "Encoding Started.." << endl;
	//cout << "\nbefore encoding : "; //�׽�Ʈ��
	for (;;) {
		input >> ch;
		if (input.eof()) break;
		string key = keymap[ch];
		//cout << ch << "";				//�׽�Ʈ��
		encoded.append(key);
	}
	//cout << "\nafter encoding : " << encoded_res << endl;		//�׽�Ʈ��
	cout << "Encoding Finished!!\n" << endl;
	return encoded;
}

/*���ϻ����� ���ϴ� �Լ�*/
int Huffman::getFileSize(string fileName)
{
	ifstream file(fileName, ios_base::in | ios_base::binary);
	file.seekg(0, ios_base::end);
	long size = file.tellg();
	//cout << file.tellg() << endl;
	return size;
}

/*����� ������ �����ϴ� �Լ�*/
void Huffman::makeFile(string fileName, string encoded) {

	std::ofstream output_file(fileName, ios::out | ios::binary);
	unsigned char temp;
	int count = 0;
	cout << "making encoded file..." << endl;
	//
	while (count < encoded.length()) {
		temp = temp & 0x00;
		for (int i = 0; i<8; i++) {
			if (count >= encoded.length()) {
				break;
			}
			if (encoded[count] == '1') {
				temp ^= 0x01;
			}
			if (i != 7) {
				temp <<= 1;
			}
			count++;
		}
		output_file << temp;
	}
	cout << "everything finished!!!" << endl;
	/*output_file << encoded << endl;
	output_file.close();*/
}