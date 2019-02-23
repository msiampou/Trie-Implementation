#pragma once

/*
  Keeps where and how many times
  a word was found
*/
struct Posting_List{
    int frequency;
    int line;
    struct Posting_List *next;
};

/*
  Keeping every word into a Trie
  only the last letter of each word
  has a PList
  all the others point to NULL
*/
struct TrieNode{
    char val;
    struct TrieNode *sibling;
    struct TrieNode *child;
    struct Posting_List *PList;
};

struct Score{
    int w_fr;    //word freq
    int doc_fr;  //doc freq
};

/*Constructs a Trie with '/0' as root*/
void Trie(struct TrieNode**);

/*Inserts a word into Trie*/
void Insert(struct TrieNode**,char*, int, char*);

/*Searches for dublicate words and updates Posting List when necessary*/
int Search(struct TrieNode*, const char*,int);

/*Returns document and term frequency of a word*/
struct Score TrieSearch(struct TrieNode*, const char*, int);

/*Prints Trie*/
void Print(struct TrieNode*);

/*Destroys Trie*/
void TrieDestroy(struct TrieNode*);
