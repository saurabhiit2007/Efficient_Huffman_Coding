//
//  huffman_utils.cpp
//
//  Author : Saurabh Goyal
///////////////////////////////

#include "huffman_utils.hpp"


void print_symbolCode_map(map<char,string> &symbolCode_map){
    
    map<char,string>::iterator it;
    
    for (it = symbolCode_map.begin(); it != symbolCode_map.end() ; ++it){
        cout << "symbol : " << it->first << "    code : " << it->second << endl;
    }
    
}

struct TreeNode* newNode(int data, char sym){
    
    // Allocate memory for new node
    struct TreeNode* node = new TreeNode;
    
    // Assign freq and symbol to this node
    node->freq = data;
    node->symbol = sym;
    
    // Initialize left and right children as NULL
    node->left = NULL;
    node->right = NULL;
    
    return(node);
}


void inorder_traversal_symbolCode_map(struct TreeNode* root, map<char,string> &symbolCode_map, string code){
    if(root == NULL)
        return;
    
    if(root->left == NULL && root->right == NULL)
        symbolCode_map[root->symbol] = code;
    
    if(root->left != NULL)
        inorder_traversal_symbolCode_map(root->left, symbolCode_map, code+'0');
    if(root->right != NULL)
        inorder_traversal_symbolCode_map(root->right, symbolCode_map, code+'1');
    
}

void save_encoded_data_in_file(string filename, int* output, int output_size, int packingBitSize){
    
    ofstream myfile;
    myfile.open(filename, ios::out);
    
    myfile << packingBitSize;
    myfile << "\n";
    myfile << output_size;
    myfile << "\n";
    
    for(int i=0;i<output_size;i++){
        myfile << output[i];
        myfile << ",";
    }
    myfile << "\n";
    myfile.close();
}


int** tree_to_array(struct TreeNode* root){
    
    struct TreeNode* temp = root;
    struct TreeNode* node;
    vector<vector<int> > tree_vec;
    
    queue<struct TreeNode*> pq;
    pq.push(temp);
    
    int count = 1;
    while(!pq.empty()){
        node = pq.front();
        pq.pop();
        
        vector<int> tempvec;
        tempvec.push_back(node->symbol);
        tempvec.push_back(count);
        tempvec.push_back(count+1);
        
        if(node->left != NULL && node->right != NULL){
            pq.push(node->left);
            pq.push(node->right);
            count+=2;
        }else{
            tempvec[1] = -2;
            tempvec[2] = -2;
        }
        tree_vec.push_back(tempvec);
        
    }
    
    int** tree_arr = new int*[tree_vec.size()];
    for(int i=0;i<tree_vec.size();i++){
        tree_arr[i] = new int[3];
    }
    for(int i=0;i<tree_vec.size();i++){
        tree_arr[i][0] = tree_vec[i][0];
        tree_arr[i][1] = tree_vec[i][1];
        tree_arr[i][2] = tree_vec[i][2];
    }

    return tree_arr;
}


void create_Huffman_Tree_from_file(string filename, struct TreeNode* &root){
    
    assert(ifstream(filename));
    
    ifstream myfile;
    myfile.open(filename);
    
    root = newNode(-1,'^');
    struct TreeNode* temp;
    
    string str, line;
    char val;
    while (!myfile.eof()){
        getline(myfile,line,',');
        stringstream ss1(line);
        ss1 >> str;
        
        getline(myfile,line);
        stringstream ss2(line);
        ss2 >> val;
        
        temp = root;
        
        int idx = 0;
        int len = str.length();
        while(idx < len - 1){
            if(str[idx] == '1'){
                if(temp->right == NULL){
                    struct TreeNode* rt = newNode(-1,'^');
                    temp->right = rt;
                }
                temp = temp->right;
            }else{
                if(temp->left == NULL){
                    struct TreeNode* lt = newNode(-1,'^');
                    temp->left = lt;
                }
                temp = temp->left;
            }
            idx++;
        }
        if(str[len-1] == '1'){
            struct TreeNode* rt = newNode(-1,val);
            temp->right = rt;
        }else{
            struct TreeNode* lt = newNode(-1,val);
            temp->left = lt;
        }
    }

}


void print_avg_bit_length(map<char,string> &symbolCode_map, map<char,int> &freq_map){
    
    map<char,string>::iterator it;
    float wgt_sum = 0.0;
    float sum = 0.0;
    int max_len = INT_MIN;
    int min_len = INT_MAX;
    
    for (it = symbolCode_map.begin(); it != symbolCode_map.end() ; ++it){
        string code = it->second;
        wgt_sum += code.length()*freq_map[it->first];
        sum += freq_map[it->first];
        max_len = max(max_len,(int)code.length());
        min_len = min(min_len,(int)code.length());
    }
    
    cout << "Mean Length : " << wgt_sum/sum << endl;
    cout << "Max length : " << max_len << endl;
    cout << "Min length : " << min_len << endl;
    
}


void printTree(struct TreeNode* root){
    if (root == NULL)
        return;
    
    cout << root->symbol << " , ";
    if (root->left != NULL)
        printTree(root->left);
    if (root->right != NULL)
        printTree(root->right);
}

void save_Huffman_tree_in_file(map<char,string> &symbolCode_map, string filename){
    
    ofstream myfile;
    myfile.open(filename, ios::out);
    
    map<char,string>::iterator it;
    for(it=symbolCode_map.begin();it!=symbolCode_map.end();++it){
        myfile << it->second;
        myfile << ",";
        myfile << it->first;
        myfile << "\n";
    }
    myfile.close();
    
}
