//
//  huffman_utils.h
//  
//  Author : Saurabh Goyal
///////////////////////////////

#include <stdio.h>
#include <string>
#include <map>
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
#include <sstream>

using namespace std;

struct TreeNode{
    int freq;
    char symbol;
    TreeNode *left;
    TreeNode *right;
};


void printTree(struct TreeNode* root);

struct TreeNode* newNode(int data, char sym);

void print_symbolCode_map(map<char,string> &symbolCode_map);

void inorder_traversal_symbolCode_map(struct TreeNode* root, map<char,string> &symbolCode_map, string code);

void save_encoded_data_in_file(string filename, int* output, int output_size, int packingBitSize);

int** tree_to_array(struct TreeNode* root);

void create_Huffman_Tree_from_file(string filename, struct TreeNode* &root);

void print_avg_bit_length(map<char,string> &symbolCode_map, map<char,int> &freq_map);

void save_Huffman_tree_in_file(map<char,string> &symbolCode_map, string filename);
