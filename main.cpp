/*
Author: Edward Zhang
Title: AVL Project
Date: 10/9/2020
Purpose: This project creates an AVL data structure to balance GatorIds
*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node {
public:
	Node() {
		val = 0;
		name = "";
		left = nullptr;
		right = nullptr;
	}
	Node(string name, int val) {
		this->val = val;
		this->name = name;
		left = nullptr;
		right = nullptr;
	}
	int val;
	string name;
	Node* left;
	Node* right;
	int height;
};

class AVL {
public:
	AVL() {
		this->root = nullptr;
		nodeCount = 0;
	}

	void printInorder() {
		string s = printInorderR(root);
		cout << s.substr(2)<<endl;
	}
	string printInorderR(Node* node) {
		//Stepik Code
		string temp;
		if (node == nullptr)
			return "";
		temp = printInorderR(node->left) + ", " + node->name + printInorderR(node->right);
		return temp;
	}
	void printPreorder() {
		string s = printPreorderR(root);
		cout << s.substr(2) << endl;
	}
	string printPreorderR(Node* node) {
		//Stepik Code
		string temp;
		if (node == nullptr)
			return "";
		temp = ", " + node->name+ printInorderR(node->left) +  printInorderR(node->right);
		return temp;
	}
	void printPostorder() {
		string s = printPostorderR(root);
		cout << s.substr(2) << endl;
	}
	string printPostorderR(Node* node) {
		//Stepik Code
		string temp;
		if (node == nullptr)
			return "";
		temp =  printInorderR(node->left) + printInorderR(node->right) + ", " + node->name;
		return temp;
	}


	Node* balance(Node* node) {
		if (node == nullptr) {
			return node;
		}
		if (node->left == nullptr && node->right == nullptr) {
			//Don't need to balance a child node
			return node;
		}
		if (node->left != nullptr) {
			if (balanceFactor(node) >= 2 && balanceFactor(node->left) >= 1) {
				//Left Left, Rotate Right
				//cout << "Left Left" << endl;
				return rotateRight(node);
			}
			else if (balanceFactor(node) >= 2 && balanceFactor(node->left) <= -1) {
				//Left Right Case, Left Right Rotation
				//cout << "Left Right" << endl;
				return rotateLeftRight(node);
			}
		}
		if (node->right != nullptr) {
			if (balanceFactor(node) <= -2 && balanceFactor(node->right) <= -1) {
				//Right Right Case, Rotate Left
				//cout << "Right right" << endl;
				return rotateLeft(node);
			}
			else if (balanceFactor(node) <= -2 && balanceFactor(node->right) >= 1) {
				//Right Left Case, Right Left Rotation
				//cout << "Right LEft" << endl;
				return rotateRightLeft(node);
			}
		}
		return node;

	}
	int balanceFactor(Node* root) {
		if (root == nullptr)
			return 0;
		return height(root->left) - height(root->right);
	}

	void insert(string name, int val) {
		//Adds a student object into tree with given name, ID
		root = insertNode(root, name, val);
		root = balance(root);
		nodeCount++;
	}
	Node* insertNode(Node* root, string name, int val) {
		if (root == nullptr) {
			root = new Node(name, val);
			root->height = 1;
			return root;
		}
		if (val > root->val)
			root->right = insertNode(root->right, name, val);
		else if (val < root->val)
			root->left = insertNode(root->left, name, val);
		else {
			cout << "unsuccessful" << endl;
			return nullptr;
		}
		root->left = balance(root->left);
		root->right = balance(root->right);
		root->height = height(root);
		return root;
	}

	bool checkAVL(Node* root) {
		//From my Stepik, checks if tree is AVL
		if (root == nullptr)
			return true;
		if (abs(height(root->left) - height(root->right)) > 1)
			return false;
		return checkAVL(root->left) && checkAVL(root->right);
	}

	void remove(int val) {
		bool s = false;
		if (nodeCount == 0) {
			return;
		}
		root = removeR(val, root, s);
		if (!s)
			cout << "unsuccessful" << endl;
		else
			nodeCount--;

	}
	Node* removeR(int val, Node* node, bool &s) {
		//Find and remove account with specified ID
		if (node == nullptr)
			return node;
		else if (node->val < val) {
			
			node->right = removeR(val, node->right, s);
		}
		else if (node->val > val) {
			node->left = removeR(val, node->left, s);
		}
		else {
			s = true;
			return deleteNode(node);
		}
		return node;

	}
	
	Node* deleteNode(Node* node) {
		if (node->left == nullptr && node->right == nullptr) {
			//Child node
			delete node;
			return nullptr;
		}
		else if (node->left == nullptr) {
			Node* temp = node->right;
			delete node;
			return temp;
		}
		else if (node->right == nullptr) {
			Node* temp = node->left;
			delete node;
			return temp;
		}
		else {
			//Two children
	
			//Finds in order successor, leftmost node on right side
			Node* inOrderS = node->right;
			while (inOrderS->left != nullptr) {
				inOrderS = inOrderS->left;
			}
			//Replaces data, deletes old node
			node->val = inOrderS->val;
			node->name = inOrderS->name;
			// t is a placeholder
			bool t = true;
			node->right = removeR(node->val, node->right, t);
		}
	}


	void removeInorder(int n) {
		if (n >= nodeCount) {
			cout << "unsuccessful" << endl;
			return;
		}
		root = removeInorderR(root, n);
	}

	Node* removeInorderR(Node* node, int n) {
		if (node == nullptr)
			return node;

	}

	void search(int val) {
		if (!searchNode(val, root))
			cout << "unsuccessful" << endl;
	}

	bool searchNode(int val, Node* node) {
		//Search for student with specifed ID, return name
		if (node == nullptr)
			return false;
		if (node->val == val) {
			cout << node->name << endl;
			return true;
		}
		return searchNode(val, node->left) || searchNode(val, node->right);
	}


	void search(string name) {
		vector<string> IDMatch;
		searchName(name, root, IDMatch);
		if (IDMatch.empty())
			cout << "unsuccessful";
		else {
			for (unsigned int i = 0; i < IDMatch.size(); i++) {
				cout << IDMatch[i] << endl;
			}
		}
			
	}
	void searchName(string n, Node* node,vector<string> &s) {
		if (node == nullptr) {
			return;
		}
		if ((node->name).compare(n) == 0) {
			s.push_back(printID(node->val));
		}
		searchName(n, node->left,s);
	    searchName(n, node->right,s);
	}

	string printID(int id) {
		string r = to_string(id);
		while (r.length() < 8) {
			r = "0" + r;
		}
		return r;
	}


	int height(Node* root)
	{
		//From Stepik Code
		if (root == nullptr)
			return 0;
		return 1 + max(height(root->left), height(root->right));
	}

	//Rotate Methods from Stepik
	Node* rotateLeft(Node* node)
	{
		Node* grandChild = node->right->left;
		Node* newParent = node->right;
		newParent->left = node;
		node->right = grandChild;
		node->height = height(node);
		return newParent;
	}

	Node* rotateRight(Node* node)
	{
		Node* grandChild = node->left->right;
		Node* newParent = node->left;
		newParent->right = node;
		node->left = grandChild;
		node->height = height(node);
		return newParent;
	}

	Node* rotateLeftRight(Node* node)
	{
		node->left = rotateLeft(node->left);
		node->height = height(node);
		return rotateRight(node);
	}
	Node* rotateRightLeft(Node* node)
	{
		node->right = rotateRight(node->right);
		node->height = height(node);
		return rotateLeft(node);
	}
	void printLevelCount() {
		cout<<height(root)<<endl;
	}

private:
	Node* root;
	int nodeCount;
};

int main()
{
	//Initializes AVL
	AVL ID;
	ID.insert("dad", 1);
	ID.insert("ed", 2);
	
	ID.insert("tay", 3);
	ID.insert("daddy", 4);
	ID.insert("test", 5);
	ID.insert("dad", 6);
	
	ID.insert("Tay", 19);
	ID.insert("Ed", 16);
	ID.insert("die", 69);
	ID.insert("omega", 18);
	ID.insert("omega", 20);
	ID.insert("omega", 40);
	ID.insert("omega", 60);
	
	ID.remove(19);
	//ID.search("lol");

	ID.printLevelCount();
	
	ID.printInorder();

	
	int count;
	cin >> count;
	string line;
	for (unsigned int i = 0; i < count; i++) {
		getline(cin, line);
		stringstream s(line);
		string word;
		while (s >> word) {

		}

	}
	
	return 0;
}