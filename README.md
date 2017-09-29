# Efficient_Huffman_Coding

This repository contains C++ implementation of Huffman Encoding and Decoding. It contains 2 implementations for decoding the encoded data:
1) Naive implementation : The data is decoded bit-by-bit through Huffman Tree traversal
2) Efficient implementation : Create a lookup table for a given bit size (< 16bits) and store the first encoded symbol contained within that bit array

# Contents of the repository
1) huffman_main.cpp + huffman_main.h - Contains the basic code for encoding and decoding (both naive & efficient).
2) huffman_utils.cpp + huffman_utils.h - Contains the helper functions that provides additional functionality like printing the Huffman Tree and SymbolCode table, printing average encoding length, saving the tree and encoded data into txt files etc.
3) SampleTextFile_500kb.txt +  SampleTextFile_1000kb.txt - Text files for testing the code and the efficiency
4) compile.sh - compile the code with -O3 optimization and generate object file "a.out"

# Running the code
1) The code can be compiled using following command :

    $ sh compile.sh

2) The code requires 2 commandline inputs : 

   a) Bit size for Lookup table (usually 4 or 8)
   
   b) Input File name (for testing use SampleTextFile_500kb.txt or SampleTextFile_1000kb.txt)
    
3) The code can be run using following command :

    $ ./a.out 8 SampleTextFile_500kb.txt
    
   # Optional
   
4) If you want to observe intermediate results like Huffman Tree, Symbol-Code map, Average-Max-Min code length and Lookup Table then set the DEBUG flag to "true" at line:11 in the huffman_main.cpp

5) If you want to save the Huffman tree and encoded data into the text file for later usage or debugging then set the SAVE_HUFFMAN_TREE_IN_FILE & SAVE_ENCODED_DATA_IN_FILE flags respectively to true in the huffman_main.cpp

