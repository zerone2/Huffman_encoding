#include "huffmanCompress.h"

// Building Frequencies from text
//읽어온 파일의 처음부터 끝까지 읽으면서 map에 문자열을 key로 해서 freq를 넣는다. 
vector< pair<char, int> > Huffman::createFreq(ifstream & input) {
	map<char, int> freq_map;
	char ch;
	vector < pair<char, int> > freq_vector;
	cout << "creating Frequencies...wait\n";
	//cout << "\ntable map : [ ";		//테스트용
	for (;;) {
		input >> ch;
		if (input.eof()) break;	//문자를 끝까지 다읽었으면 break
								//freq_map에서 문자를 찾아서 없으면 freq_map에 그 문자를 key, freq =1을 넣고, 없으면 해당 문자에 대한 freq 1증가 
		if (freq_map.find(ch) == freq_map.end()) {
			freq_map.insert(make_pair(ch, 1));
		}
		else {
			freq_map[ch] += 1;	//insert가 아닌 operator 사용. ch를 키로 가지는 value에 1을 더해줌
		}
		//cout << "{" << ch << ":" << freq_map[ch] << "}" << ",";		//테스트용1
	}
	//cout << " ]";	//테스트용
	cout << "Done!!! making frequencies for each characters\n" << endl;
	/*반복자 iterator를 사용해 처음부터 끝까지 검색하면서 freq_vec 큐안에 key값과 freq값을 넣어줌, end()는 컨테이너의 끝부분 보다 하나 더 나간 위치!!*/
	/* <,> 를 사용해서 비교 못함... vector,deque는 가능함(random access가 가능해야함). 따라서 != 으로 비교해야됨!!*/
	for (map<char, int>::iterator it = freq_map.begin(); it != freq_map.end(); ++it) {
		freq_vector.push_back(make_pair(it->first, it->second));
	}

	return freq_vector;
}

//Building an encoding tree with the character & frequency pair	
/*문자와 빈도수로 허프만 트리 생성.*/
Huffman::pHuffNode Huffman::compressTree(vector< pair<char, int> > freq_vector) {

	/*freq_vec안에 있던 값을 priority queue안으로 넣어줌으로써 freq에 의해 정렬되게 함*/
    priority_queue<Huffman::HuffTreeNode , vector<Huffman::pHuffNode >, Huffman::compareNode > comp_tree;
    for (vector< pair<char, int> >::iterator it = freq_vector.begin(); it != freq_vector.end(); ++it) {
        Huffman::pHuffNode leaf = new Huffman::HuffTreeNode(it->first, it->second);
        comp_tree.push(leaf);
    }

	int i = 1; //테스트용2

	/*comp_tree priority queue에서 처음부터 값을 빼와 허프만트리 노드를 만들어 값을 집어넣어준다.*/
    Huffman::pHuffNode root = NULL;
    while (comp_tree.size() > 1) {
        Huffman::pHuffNode L, R;
        L = comp_tree.top();
        comp_tree.pop();
        R = comp_tree.top();
        comp_tree.pop();
		//cout << "i : " << i << " / LEFT : " << L->freq << "(freq) , " << L->data << "(data) / " << "RIGHT : " << R->freq << "(freq) , " << R->data << "(data)" << endl;	//테스트용
		i++;
        root = new Huffman::HuffTreeNode(0, L->freq + R->freq, L, R);
		//cout << "\t" << "root : " << root->freq << " / L-child : " <<  root->left->data << "(data) , " << root->left->freq << "(freq) / " 
		//	<< "R-child : " << root->right->data << "(data) , " << root->right->freq << "(freq)" << "\n" << endl;	//테스트용
        comp_tree.push(root);
    }

    return root;
}

// Build the lookup table, 정렬된 상태로 자료저장(map 사용)
/*map에서 1번째 인자는 key값, 2번째 인자는 data. 각각 first,second 형태로 저장.*/
map<char, string> Huffman::makeKeyMap(Huffman::pHuffNode htree) {
	map<char, string> key_map;
	deque< pair<Huffman::pHuffNode, string> > huff_queue;
	//int i = 0, j = 0, n = 2;	//테스트용7
	cout << "Making key map now...wait\n";
	huff_queue.push_back(make_pair(htree, ""));		//make_pair 가 쌍으로 큐에 집어넣는 함수. 2차원 배열 같은 개념.

	/*huff_queue에 노드가 존재하지 않을때까지 노드를 key_map 맵 안에 넣어준다.*/
	while (!huff_queue.empty()) {
		Huffman::pHuffNode node, left, right;
		string key;
		node = huff_queue.front().first;
		key = huff_queue.front().second;
		huff_queue.pop_front();
		left = node->left;
		right = node->right;

		//if (n - i == 1)j++;  //테스트용11


		if (left) {							//left child가 존재시.
			string tmp_key = key;
			huff_queue.push_back(make_pair(left, key.append("0")));			//left로 빠지면 0을 붙이고
			huff_queue.push_back(make_pair(right, tmp_key.append("1")));		//right로 빠지면 1을 붙인다.
			//cout << "==" << j << "층 , " << i << "번째 부모노드==" << endl;																//테스트용8
			//cout << "node L-child : " << node->left->data <<"(data) , "<< node->left->freq << "(freq)" << " , code" << code << endl;		//테스트용5
			//cout << "node R-child : " << node->right->data << "(data) , " << node->right->freq << "(freq)" << "\n" << endl;				//테스트용6
			//i++;		//테스트용10


		}
		else {
			key_map.insert(make_pair(node->data, key));		//더이상 자식 노드가 없으면 그게 끝이므로 data(문자)와 code(빈도수)를 key_map map 안에 넣어준다.
		}
		//if (i > n) n *= 2;	//테스트용12
	}

	cout << "encoding map had built!!! \n" << endl;
	return key_map;
}

//실제로 인코딩체계를 가진 파일을 만들어줌.
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
	//cout << "\nbefore encoding : "; //테스트용
	for (;;) {
		input >> ch;
		if (input.eof()) break;
		string key = keymap[ch];
		//cout << ch << "";				//테스트용
		encoded.append(key);
	}
	//cout << "\nafter encoding : " << encoded_res << endl;		//테스트용
	cout << "Encoding Finished!!\n" << endl;
	return encoded;
}

/*파일사이즈 구하는 함수*/
int Huffman::getFileSize(string fileName)
{
	ifstream file(fileName, ios_base::in | ios_base::binary);
	file.seekg(0, ios_base::end);
	long size = file.tellg();
	//cout << file.tellg() << endl;
	return size;
}

/*압축된 파일을 생성하는 함수*/
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