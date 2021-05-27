/*
Red Black Tree Insertion by Nabhan Abedin
5.27.2021
*/

#include <iostream>
#include <cstring>
#include <fstream>
//set colors of tree nodes
//color codes from https://pkg.go.dev/github.com/whitedevops/colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREY    "\033[90m"

#include "node.h"

using namespace std;

//define trunks to represent each level of tree
struct Trunk 
{
  Trunk *prev;
  char* str;

  Trunk(Trunk *prev, char* str) 
  {
    this->prev = prev;
    this->str = str;
  }
};

//for printing each level of tree
void showTrunks(Trunk *p) 
{
  if (p == NULL)
  {
      return;
  }

  showTrunks(p -> prev);

  cout << p -> str;
}

//function prototypes
void addNumber(Node* &head, Node* &curr, Node*& prev, int val);
void readFile(Node* &head);
void printTree(Node* root, Trunk *prev, bool isLeft);
void parse(char* in, int* parsedInput, int &count);

//balancing prototypes
void balance(Node* &head, Node* &curr);
void rotateLeft(Node* &head, Node* &curr);
void rotateRight(Node* &head, Node* &curr);

int main()
{
  char input[10]; //user input
  char read[10000];  //file input
  int parsedInput[100]; //parsed input for insertion
  Node* head = NULL;
  bool running = true;
  while (running) 
  {
    cout << endl << "What do you want to do? ('add', 'read', 'print', or 'quit')" << endl;
    cin.get(input, 10);
    cin.clear();
    cin.ignore(10000, '\n');
    if (strcmp(input, "add") == 0) 
    {
      int val;
      cout << "Enter a value: ";
      cin >> val;
      cin.clear();
      cin.ignore(10000, '\n');
      Node* curr = head;
      Node* prev = NULL;
      addNumber(head, curr, prev, val);
      if(curr != head) balance(head, curr);
      cout << endl << val << " has been added:" << endl;
    }
    else if (strcmp(input, "read") == 0) 
    {
      readFile(head);
    }
    else if (strcmp(input, "print") == 0) 
    {
      printTree(head, NULL, false);
    }
    else if (strcmp(input, "quit") == 0) 
    {
      cout << endl << "Goodbye." << endl;
      running = false;
    }
    else 
    {
      cout << endl << "Invalid input. Try again." << endl;
    }
  }
  return 0;
}

//balancing from https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
void balance(Node* &head, Node* &curr) 
{
  Node* parent = NULL;
  Node* grandparent = NULL;
  while ((curr != head) && (curr->getColor() != 0) && ((curr->getParent())->getColor() == 1)) 
  {
    parent = curr->getParent();
    grandparent = parent->getParent();
    //Case 1: parent is the left child of grandparent
    if (parent == grandparent->getLeft()) 
    {
      Node* uncle = grandparent->getRight();
      //Case 1.1: uncle is red, then recolor
      if (uncle != NULL && uncle->getColor() != 0) 
      {
        grandparent->setColor(1);//red
        parent->setColor(0);//black
        uncle->setColor(0);//black
        curr = grandparent;
      }
      else 
      {
        //Case 1.2: curr is the right child of parent, then rotate left
        if (curr == parent->getRight()) 
        {
        rotateLeft(head, parent);
        curr = parent;
        parent =  curr->getParent();
        }
        //Case 1.3: curr is the left child of parent, then rotate right
        rotateRight(head, grandparent);
        //swap colors of parent and grandparent
        int tempC = parent->getColor();
        parent->setColor(grandparent->getColor());
        grandparent->setColor(tempC);
        curr = parent;
      }
    }
    //Case 2: parent is the right child of grandparent
    else 
    {
      Node* uncle = grandparent->getLeft();
      //Case 2.1: uncle is red, then recolor
      if (uncle != NULL && uncle->getColor() != 0) 
      {
        grandparent->setColor(1);//red
        parent->setColor(0);//black
        uncle->setColor(0);//black
        curr = grandparent;
      }
      else 
      {
        //Case 2.2: curr is the left child of parent, then rotate right
        if (curr == parent->getLeft()) 
        {
        rotateRight(head, parent);
        curr = parent;
        parent = curr->getParent();
    	}
        //Case 2.3: curr is the right child of parent, then rotate left
        rotateLeft(head, grandparent);
        //swap color of parent and grandparent
        int tempC = parent->getColor();
        parent->setColor(grandparent->getColor());
        grandparent->setColor(tempC);
        curr = parent;
      }
    }
  }
  head->setColor(0);
}

void rotateLeft(Node* &head, Node* &curr) 
{
  Node* rightPtr = curr->getRight(); 
  curr->setRight(rightPtr->getLeft()); 
  if (curr->getRight() != NULL) 
  {
    (curr->getRight())->setParent(curr); 
  }
  rightPtr->setParent(curr->getParent());
  if (curr->getParent() == NULL) 
  {
    head = rightPtr; 
  } 
  else if (curr == (curr->getParent())->getLeft()) 
  {
    (curr->getParent())->setLeft(rightPtr); 
  } 
  else
  {
    (curr->getParent())->setRight(rightPtr); 
  }  
  rightPtr->setLeft(curr); 
  curr->setParent(rightPtr);
}

void rotateRight(Node* &head, Node* &curr) 
{
  Node *leftPtr = curr->getLeft(); 
  curr->setLeft(leftPtr->getRight()); 
  if (curr->getLeft() != NULL) 
  {
    (curr->getLeft())->setParent(curr); 
  }  
  leftPtr->setParent(curr->getParent());
  if (curr->getParent() == NULL) 
  {
    head = leftPtr; 
  }
  else if (curr == (curr->getParent())->getLeft()) 
  {
    (curr->getParent())->setLeft(leftPtr); 
  } 
  else 
  {
    (curr->getParent())->setRight(leftPtr);
  }
  leftPtr->setRight(curr); 
  curr->setParent(leftPtr);
}

void addNumber(Node* &head, Node* &curr, Node* &prev, int val) {
  if (head == NULL) 
  {
    head = new Node();
    curr = head;
    head->setData(val);
    head->setColor(0);  //head is black
    return;
  } 
  else 
  {
    if (val < curr->getData()) 
    {  
      //lower value goes to left
      prev = curr;
      curr = curr->getLeft();
      if (curr == NULL) 
      {  
        //if empty, insert value
        curr = new Node();
        curr->setData(val);
        prev->setLeft(curr);
        curr->setParent(prev);
        balance(head, curr);  //balance
        return;
      } 
      else 
      {  //if not empty, keep adding
	    addNumber(head, curr, prev, val);
      }
    }
    else 
    {
      prev = curr;
      curr = curr->getRight();
        if (curr == NULL) 
        {
        //if empty, insert value
        curr = new Node();
        curr->setData(val);
        prev->setRight(curr);
        curr->setParent(prev);
        balance(head, curr);  //balance
        return;
      } 
      else 
      {  //if not empty, keep adding
	    addNumber(head, curr, prev, val);
      }
    }
  }
}

void readFile(Node* &head) 
{
  char in[10000];
  char fileName[100];
  int parsedInput[100];
  int siz = 0;
  int count = 0;
  memset(in, 0, 10000);
  memset(fileName, 0, 100);
  memset(parsedInput, 0, 100);

  //get file name
  cout << endl << "What is the name of the file?" << endl;
  cin.get(fileName, 30);
  cin.clear();
  cin.ignore(100000, '\n');
  streampos size;
  ifstream file(fileName, ios::in | ios::binary | ios::ate);
  if (file.is_open()) 
  {
    size = file.tellg();
    file.seekg(0, ios::beg);
    file.read(in, size);
    file.close();
    parse(in, parsedInput, count);  //parse input to modify array
    cout << "IN: ";
    for (int i = 0; i < 100; i++) 
    {
      if (parsedInput[i] != 0) 
      {
        cout << parsedInput[i] << " ";
        siz++;
      }
    }
    cout << endl << "Size: " << siz << endl;;
    //add to tree
    Node* curr = NULL;
    Node* prev = NULL;
    for (int i = 0; i < siz; i++) 
    {
      if(parsedInput[i] == 0)
      {
        break;
      } 
      curr = head;
      addNumber(head, curr, prev, parsedInput[i]);
    }
  } 
}

//print tree
void printTree(Node* root, Trunk *prev, bool isLeft) 
{
  if (root == NULL) 
  {
    return;
  }
  char* prev_str = (char*)("    ");
  Trunk *trunk = new Trunk(prev, prev_str);
  printTree(root->getLeft(), trunk, true);
  if (!prev) 
  {
    trunk -> str = (char*)("---");
  }
  else if (isLeft)
   {
    trunk -> str = (char*)(".---");
    prev_str = (char*)("   |");
  }
  else 
  {
    trunk -> str = (char*)("`---");
    prev -> str = prev_str;
  }
  showTrunks(trunk);
  if (root->getColor() == 0)
  {  
    //black nodes
    cout << GREY << root->getData() << RESET << endl;
  } 
  else 
  {  
    //red nodes
    cout << RED << root->getData() << RESET << endl;
  }
  if (prev) 
  {
    prev -> str = prev_str;
  }
  trunk -> str = (char*)("   |");
  printTree(root->getRight(), trunk, false);
}

void parse(char* in, int* parsedInput, int &count) 
{
  int l = 0; //tracks how many digits each value is
  for (int i = 0; i < strlen(in); i++) 
  {
    if (in[i] == ' ') 
    {
      if (l == 1) 
      {
        int temp = 0;
        temp = in[i-1] - '0';
        parsedInput[count] = temp;
        count++;
        l = 0;
      } 
      else 
      {
        int temp = 0;
        for (int a = 0; a < l; a++) 
        {
          temp = 10 * temp + (in[i-l+a] - '0');
        }
        parsedInput[count] = temp;
        count++;
        l = 0;
      }
    } 
    else 
    {
      int temp = 0;
      l++;
      if (i == strlen(in) - 1) 
      {  
        //last pair of chars
        for (int a = 0; a < l; a++) 
        {
            temp = 10 * temp + (in[i+a+1-l] - '0');
        }
        parsedInput[count] = temp;
        count++;
      }
    }
  } 
}