//
//  huffman_main.h
//
//  Author : Saurabh Goyal
///////////////////////////////

#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <set>
#include <fstream>
#include <algorithm>
#include <queue>
#include <sstream>
#include <cassert>
#include <functional>
#include "huffman_utils.h"

using namespace std;

class HuffmanEncoding{
    
public:
    
    int packingBitSize;
    int lookup_bitsize;
    
    int padding;
    string input_data;
    int encoded_output_size;
    int* encoded_output;
    map<char,int> freq_map;
    map<char,string> symbolCode_map;
    struct TreeNode* tree_root;
    
    
    void encode(HuffmanEncoding &huff_encode);
    
    void encode_helper(HuffmanEncoding &huff_encode);
    
    map<char,int> make_frequency_dict(string &inputData);
    
    map<char,string> create_symbolCode_map(struct TreeNode* root);
    
    struct TreeNode* createHuffmanTree(map<char,int> &freq_map);
    
    int** create_lookup_table(int lookup_bitsize, int** &huffman_array);
        
    string efficient_decode(int* encoded_input, int encoded_input_size, int** huffman_array, int** lookup_table, int packingBitSize, int lookup_bitsize, int padding);
    
    string decode(int* encoded_input, int encoded_input_size, int** huffman_array, int packingBitSize);

    
};
















