//
//  huffman_main.cpp
//
//  Author : Saurabh Goyal
///////////////////////////////


#include "huffman_main.h"

// Setting flags for support operations
bool DEBUG = false;
bool SAVE_HUFFMAN_TREE_IN_FILE = true;
bool SAVE_ENCODED_DATA_IN_FILE = true;



// This function compares the frequency of the symbols while creating Huffman Tree
struct compare_tree_nodes{
    
    bool operator () (struct TreeNode* const &p1, struct TreeNode* const &p2)
    {
        return p1->freq > p2->freq;
    }
};

bool comp(const pair<string,int> a, pair<string,int> b){
    if(a.first.length() < b.first.length())
        return true;
    return false;
}


// This function creates a Frequency map for unique symbols in the input file
map<char,int> HuffmanEncoding::make_frequency_dict(string &inputData){

    map<char,int> freq_map;
    
    for(int i=0;i<inputData.size();i++){
        
        if(freq_map.find(inputData[i]) != freq_map.end())
            freq_map[inputData[i]]++;
        else
            freq_map.insert(make_pair(inputData[i],1));
        
    }
    return freq_map;
}


// This is a support function that creates codebook in the form (symbol,huffman_code)
// which indirectly helps in encoding the input directly without traversing Huffman tree
map<char,string> HuffmanEncoding::create_symbolCode_map(struct TreeNode* root){
    
    map<char,string> symbolCode_map;
    string str = "";
    inorder_traversal_symbolCode_map(root,symbolCode_map,str);
    
    return symbolCode_map;
}


// This function creates Binary Huffman tree from the Frequency map of the input file
// Note : All Leaf Nodes contains Symbols and non-Leaf nodes contains '^' as a filler.
// Make sure your input file doesn't contain '^' symbol in it.
struct TreeNode* HuffmanEncoding::createHuffmanTree(map<char,int> &freq_map){
    
    priority_queue<struct TreeNode*, vector<struct TreeNode*>, compare_tree_nodes> pq;
    
    map<char,int>::iterator it;
    
    for (it = freq_map.begin(); it != freq_map.end() ; ++it){
        struct TreeNode* leaf = newNode(it->second, it->first);
        pq.push(leaf);
    }
    
    while(pq.size() != 1){
        struct TreeNode* node1 = pq.top();
        pq.pop();
        
        struct TreeNode* node2 = pq.top();
        pq.pop();
        
        struct TreeNode* merge_node = newNode(node1->freq + node2->freq, '^');
        
        merge_node->left = node1;
        merge_node->right = node2;
        
        pq.push(merge_node);
    }
    
    return pq.top();
    
}

// This function takes the SymbolCode map and input string to generate the encoded data
// Note : The encoded string is packed into integers with Bit size = packingBitSize
void HuffmanEncoding::encode_helper(HuffmanEncoding &huff_encode){
    
    // Encode the input string using the Symbol map generated before
    string output_str = "";
    
    for(int i=0 ; i<huff_encode.input_data.size() ; i++){
        string org_code = huff_encode.symbolCode_map[huff_encode.input_data[i]];
        output_str += org_code;
    }
    
    // Reverse the encoded string as this will help in bit shifting operations during decoding
    reverse(output_str.begin(),output_str.end());
    
    
    // Add extra padding if the encoded string is not a multiple of packingBitSize
    huff_encode.padding = huff_encode.packingBitSize - output_str.size() % huff_encode.packingBitSize;
    
    for(int i=0;i<padding;i++){
        output_str = '0' + output_str;
    }
    
    // Allocate space for output array
    huff_encode.encoded_output_size = output_str.size()/huff_encode.packingBitSize;
    huff_encode.encoded_output = new int[huff_encode.encoded_output_size];
    
    // Pack the output string into integer array where each integer is represented in packingBitSize Bits
    int idx=0;
    for(int i=0 ; i<output_str.size() ; i += huff_encode.packingBitSize){
        string strsub = output_str.substr(i,huff_encode.packingBitSize);
        huff_encode.encoded_output[idx] = stoi(strsub,nullptr,2);
        idx++;
    }
    
}


// This function calls various helper functions to generate the Huffman encoding of the input data
void HuffmanEncoding::encode(HuffmanEncoding &huff_encode){ //string &inputData, int* &encoded_output, int packingBitSize){
    
    // Create a frequency map for the symbols in the input string
    huff_encode.freq_map = make_frequency_dict(huff_encode.input_data);
    
    // Create Huffman Tree depending on the relative frequencies of the symbols
    huff_encode.tree_root = createHuffmanTree(huff_encode.freq_map);
    
    // Create a symbol --> codemap which will assist in encoding the input stream
    huff_encode.symbolCode_map = create_symbolCode_map(huff_encode.tree_root);

    // Encode the input string using symbolCode_map and packing them into integer array
    encode_helper(huff_encode);
    
}


// This function creates a lookup table of size 2^(lookup_bitsize) which helps in
// processing multiple bits at a time while decoding. This makes the decoding more efficient in
// terms of time consumed.
int** HuffmanEncoding::create_lookup_table(int lookup_bitsize, int** &huffman_array){
    
    int table_size = pow(2.0,lookup_bitsize);
    
    int** lookup_table = new int*[table_size];
    for(int i=0;i<table_size;i++){
        lookup_table[i] = new int[3];
    }
    
    for(int i=0;i<table_size;i++){
        
        int num = i;
        int idx = lookup_bitsize-1;
        int node = 0;
        vector<int> tempvec;
        while (idx >= 0){
            if ((num & (1 << idx)) > 0){
                node = huffman_array[node][2];
            }else{
                node = huffman_array[node][1];
            }
            if(char(huffman_array[node][0]) != '^'){
                tempvec.push_back(1);
                tempvec.push_back(huffman_array[node][0]);
                tempvec.push_back(lookup_bitsize-idx);
                break;
            }
            idx--;
        }
        
        // Signifies that the no symbol occures in tree before bitsize length
        if(idx<0){
            tempvec.push_back(0);
            tempvec.push_back(int('^'));
            tempvec.push_back(node);
        }
        
        lookup_table[i][0] = tempvec[0];
        lookup_table[i][1] = tempvec[1];
        lookup_table[i][2] = tempvec[2];
    }
    
    // Reverse the lookup table since while decoding the string we are not looking at lookup table in
    // reverse order
    for(int i=0;i<table_size;i++){
        string str_reverse = bitset<64>(i).to_string();
        string temp_str = str_reverse.substr(64-lookup_bitsize, lookup_bitsize);
        reverse(temp_str.begin(),temp_str.end());
        str_reverse = str_reverse.substr(0,64-lookup_bitsize);
        str_reverse += temp_str;
        int i_reverse = stoi(str_reverse, nullptr, 2);
        
        if (i_reverse < i)
            continue;
        
        int a = lookup_table[i][0];
        lookup_table[i][0] = lookup_table[i_reverse][0];
        lookup_table[i_reverse][0] = a;
        
        a = lookup_table[i][1];
        lookup_table[i][1] = lookup_table[i_reverse][1];
        lookup_table[i_reverse][1] = a;
        
        a = lookup_table[i][2];
        lookup_table[i][2] = lookup_table[i_reverse][2];
        lookup_table[i_reverse][2] = a;
        
    }
    
    return lookup_table;
    
}


string HuffmanEncoding::efficient_decode(int* encoded_input, int encoded_input_size, int** huffman_array, int** lookup_table, int packingBitSize, int lookup_bitsize, int padding){
    
    string decoded_output = "";
    int idx = 0;
    int jump = 0;
    int lookup_num;
    int treeNode = 0;
    int max_bit_num = pow(2.0,lookup_bitsize) - 1;
    
    int i = encoded_input_size-1;
    int diff = packingBitSize - lookup_bitsize;
    
    int int_bit = encoded_input[i];
    while(i >= 0){
        
        if(idx > diff){
            
            // This part of the code is executed when the lookup bits are divided among 2 consecutive integers
            int frag1_size = packingBitSize - idx;
            int frag1 = (int_bit >> idx);
            
            int next_int_bit = encoded_input[i-1];
            
            int frag2_size = lookup_bitsize - frag1_size;
            int max_bits_frag2 = (1 << frag2_size) - 1;
            int frag2 = next_int_bit & max_bits_frag2;
            frag2 = frag2 << frag1_size;
            lookup_num = frag2 + frag1;
            
        }else{
            lookup_num = ((int_bit >> idx) & max_bit_num);
        }
        
        if (lookup_table[lookup_num][0] == 1){
        
            // If we find the entry in the lookup table then directly jump to the bit in the lookup table
            decoded_output += lookup_table[lookup_num][1];
            jump = lookup_table[lookup_num][2];
            idx += jump;
        
        }else{
            
            // If we don't find the entry in the lookup table then we directly go to the treeNode that is saved
            // in the lookup table and start processing the encoded data bit-by-bit
            treeNode = lookup_table[lookup_num][2];
            idx += lookup_bitsize;
            
            if(idx >= packingBitSize){
                idx -= packingBitSize;
                i--;
                int_bit = encoded_input[i];
            }
            
            
            while(idx < packingBitSize){
                if ( (int_bit & (1 << idx)) > 0){
                    treeNode = huffman_array[treeNode][2];
                }else{
                    treeNode = huffman_array[treeNode][1];
                }
                
                if(char(huffman_array[treeNode][0]) != '^'){
                    decoded_output += char(huffman_array[treeNode][0]);
                    treeNode = 0;
                    idx++;
                    break;
                }
                idx++;
                if(idx >= packingBitSize){
                    idx -= packingBitSize;
                    i--;
                    int_bit = encoded_input[i];
                }
            }
            
        }
        if(idx >= packingBitSize){
            i--;
            int_bit = encoded_input[i];
            idx = idx - packingBitSize;
        }
        if(i==0 && idx >= packingBitSize - padding)
            break;
    }
    
    return decoded_output;
}


// This function implements naive huffman decoding where we process the encoded data bit-by-bit
string HuffmanEncoding::decode(int* encoded_input, int encoded_input_size, int** huffman_array, int packingBitSize){
    
    string decoded_output = "";

    int treeNode = 0;
    int idx = 0;
    int int_bit;
    for(int i=encoded_input_size-1; i>0; i--){
        
        int_bit = encoded_input[i];
        while (idx < packingBitSize){
            
            if ((int_bit & (1 << idx)) > 0){
                treeNode = huffman_array[treeNode][2];
            }else{
                treeNode = huffman_array[treeNode][1];
            }
            
            if(huffman_array[treeNode][0] != '^'){
                decoded_output += huffman_array[treeNode][0];
                treeNode = 0;
            }
            idx++;
        }
        idx=0;
    }
    
    // Since the last data point was padded with zeros
    idx=0;
    int_bit = encoded_input[0];
    for(int i=0;i<packingBitSize-padding;i++){
        
        if ((int_bit & (1 << idx)) > 0){
            treeNode = huffman_array[treeNode][2];
        }else{
            treeNode = huffman_array[treeNode][1];
        }
        
        if(char(huffman_array[treeNode][0]) != '^'){
            decoded_output += huffman_array[treeNode][0];
            treeNode = 0;
        }
        idx++;
    }

    return decoded_output;
}



int main(int argc, char const *argv[]){
    
    
    //Create object of the "huffman_encoding" class
    HuffmanEncoding huff_encode;
    
    
    // Reading input from the command line
    if(argc < 3){
        cout << "Not enough input" << endl;
        return 0;
    }
    
    huff_encode.lookup_bitsize = atoi(argv[1]);
    string input_filename = argv[2];
    huff_encode.packingBitSize = 16;
    
    // Check if the input file exists
    assert(ifstream(input_filename));
    
    
    // Reading input data from the file //
    ifstream myfile;
    myfile.open(input_filename);
    
    huff_encode.input_data = "";
    string line, val;
    while (!myfile.eof()){
        getline(myfile,line,' ');
        stringstream ss2(line);
        ss2 >> val;
        huff_encode.input_data += val;
    }
    
    
    //************************************************************************************************************//
    //********************************************  HUFFMAN ENCODING *********************************************//
    
    
    // Enode the input string into integers of size "packingBitSize"
    huff_encode.encode(huff_encode);
    
    
    
    //************************************************************************************************************//
    //******************************************* SUPPORT OPERATIONS *********************************************//
    
    // Print tree in inorder manner if the DEBUG mode is on
    if(DEBUG){
        cout << "*******  PRINTING HUFFMAN TREE  *********" << endl;
        cout << endl;
        printTree(huff_encode.tree_root);
        cout << endl << endl;
        cout << "*****************************************" << endl;
    }

    // Print Sybmol-Code map if the DEBUG mode is on
    if(DEBUG){
        cout << "*******  PRINTING SYMBOL CODE MAP  *********" << endl;
        cout << endl;
        print_symbolCode_map(huff_encode.symbolCode_map);
        cout << endl << endl;
        cout << "********************************************" << endl;
    }
    
    // Print Mean, Maximum and Minimum code length after Huffman coding
    if(DEBUG){
        cout << "*******  PRINTING MEAN, MAX & MIN CODE LENGTH  *********" << endl;
        cout << endl;
        print_avg_bit_length(huff_encode.symbolCode_map, huff_encode.freq_map);
        cout << endl << endl;
        cout << "********************************************************" << endl;
    }
    
    
    // Save the Huffman tree in (Code,Symbol) format into .txt file if it has to be read during Decoding
    string tree_file_path = "output_tree.txt";
    if(SAVE_HUFFMAN_TREE_IN_FILE){
        
        save_Huffman_tree_in_file(huff_encode.symbolCode_map, tree_file_path);
        
    }
    
    // Save the encoded data to txt file if it has to be read during Decoding
    string encoded_data_file_path = "encoded_output.txt";
    if(SAVE_ENCODED_DATA_IN_FILE){
        
        save_encoded_data_in_file(encoded_data_file_path, huff_encode.encoded_output, huff_encode.encoded_output_size, huff_encode.packingBitSize);
        
    }
    
    // If tree is read from the saved file then uncomment this function
    create_Huffman_Tree_from_file(tree_file_path, huff_encode.tree_root);
    
    
    // For Decoding convert the tree into array format for better cache utilization
    int** huffman_array = tree_to_array(huff_encode.tree_root);
    

    //************************************************************************************************************//
    //*************************************** NAIVE HUFFMAN DECODING *********************************************//
    
    clock_t stime = clock();
    
    string decoded_output = huff_encode.decode(huff_encode.encoded_output, huff_encode.encoded_output_size, huffman_array, huff_encode.packingBitSize);
    
    clock_t etime = clock();
    
    float elapsedTime = (etime-stime)*1000.0/CLOCKS_PER_SEC;
    
    assert(huff_encode.input_data == decoded_output);
    
    cout << "*****************  COMPARING DECODING TIME ***************** " << endl << endl;
    
    cout << "Naive Decoding Time : " << elapsedTime << " (ms)" << endl;
    
    //************************************************************************************************************//
    //*************************************** EFFICIENT HUFFMAN DECODING *****************************************//
    
    
    int** lookup_table = huff_encode.create_lookup_table(huff_encode.lookup_bitsize, huffman_array);
    
    if(DEBUG){
        cout << "********** PRINTING LOOKUP TABLE *********" << endl;
        cout << endl;
        int table_size = pow(2.0,huff_encode.lookup_bitsize);
        for(int i=0;i<table_size;i++){
            cout << "i : " << i <<  "   symbol exist : " << lookup_table[i][0] << "  symbol : " << char(lookup_table[i][1]) << "   Jump/TreeNode : " << lookup_table[i][2] << endl;
        }
        cout << endl;
        cout << "******************************************" << endl;
    }
    
    
    stime = clock();
    
    string efficient_decoded_output = huff_encode.efficient_decode(huff_encode.encoded_output, huff_encode.encoded_output_size, huffman_array, lookup_table, huff_encode.packingBitSize, huff_encode.lookup_bitsize, huff_encode.padding);
    
    etime = clock();
    
    elapsedTime = (etime-stime)*1000.0/CLOCKS_PER_SEC;
    
    assert(huff_encode.input_data == efficient_decoded_output);
    
    cout << "Efficient Decoding Time : " << elapsedTime << " (ms)" << endl << endl;

    cout << "************************************************************" << endl;
    
    return 0;
}











