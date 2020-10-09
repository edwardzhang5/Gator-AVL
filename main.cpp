/*
Author: Edward Zhang
Title: AVL Project
Date: 10/9/2020
Purpose: This project creates an AVL data structure to balance GatorIds
*/

#include <iostream>
#include <string>
#include <exception>
#include <vector>
using namespace std;

class Node {
public:
	// Constructors for Nodes
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
	// Default Constructor
	AVL() 
	{
		this->root = nullptr;
		nodeCount = 0;
	}
	void printInorder() 
	{
		if (nodeCount != 0) {
			string s = printInorderR(root);
			cout << s.substr(2) << endl;
		}
		else
			cout << "unsuccessful" << endl;
	}
	string printInorderR(Node* node) 
	{
		// Stepik Code
		// https://stepik.org/submissions/1480359?unit=379726
		string temp;
		if (node == nullptr)
			return "";
		temp = printInorderR(node->left) + ", " + node->name + printInorderR(node->right);
		return temp;
	}
	void printPreorder() 
	{
		string s = printPreorderR(root);
		cout << s.substr(2) << endl;
	}
	string printPreorderR(Node* node) 
	{
		string temp;
		if (node == nullptr)
			return "";
		temp = ", " + node->name+ printPreorderR(node->left) +  printPreorderR(node->right);
		return temp;
	}
	void printPostorder() 
	{
		string s = printPostorderR(root);
		cout << s.substr(2) << endl;
	}
	string printPostorderR(Node* node) 
	{
		string temp;
		if (node == nullptr)
			return "";
		temp =  printPostorderR(node->left) + printPostorderR(node->right) + ", " + node->name;
		return temp;
	}


	Node* balance(Node* node) 
	{
		if (node == nullptr) {
			return node;
		}
		if (node->left == nullptr && node->right == nullptr) {
			// Don't need to balance a child node
			return node;
		}
		if (node->left != nullptr) {
			if (balanceFactor(node) >= 2 && balanceFactor(node->left) >= 1) {
				// Left Left, Rotate Right
				return rotateRight(node);
			}
			else if (balanceFactor(node) >= 2 && balanceFactor(node->left) <= -1) {
				// Left Right Case, Left Right Rotation
				return rotateLeftRight(node);
			}
		}
		if (node->right != nullptr) {
			if (balanceFactor(node) <= -2 && balanceFactor(node->right) <= -1) {
				// Right Right Case, Rotate Left
				return rotateLeft(node);
			}
			else if (balanceFactor(node) <= -2 && balanceFactor(node->right) >= 1) {
				// Right Left Case, Right Left Rotation
				return rotateRightLeft(node);
			}
		}
		return node;

	}
	int balanceFactor(Node* root) 
	{
		if (root == nullptr)
			return 0;
		return height(root->left) - height(root->right);
	}

	void insert(string name, int val) {
		// Adds a student object into tree with given name, ID
		root = insertNode(root, name, val);
		root = balance(root);
		nodeCount++;
	}
	Node* insertNode(Node* root, string name, int val) 
	{
		if (root == nullptr) {
			root = new Node(name, val);
			root->height = 1;
			cout << "successful" << endl;
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

	void remove(int val) 
	{
		bool s = false;
		if (nodeCount == 0) {
			cout << "unsuccessful" << endl;
			return;
		}
		root = removeR(val, root, s);
		if (!s)
			cout << "unsuccessful" << endl;
		else {
			nodeCount--;
			cout << "successful" << endl;
		}

	}
	Node* removeR(int val, Node* node, bool &s)
	{
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
	
	Node* deleteNode(Node* node) 
	{
		if (node->left == nullptr && node->right == nullptr) {
			// Leaf node
			delete node;
			return nullptr;
		}
		else if (node->left == nullptr) {
			// Right node only
			Node* temp = node->right;
			delete node;
			return temp;
		}
		else if (node->right == nullptr) {
			// Left node only
			Node* temp = node->left;
			delete node;
			return temp;
		}
		else { 			// Two children

			// Finds in order successor, leftmost node on right side
			Node* inOrderS = node->right;
			while (inOrderS->left != nullptr) {
				inOrderS = inOrderS->left;
			}
			// Replaces data, deletes old node
			node->val = inOrderS->val;
			node->name = inOrderS->name;
			// t is a placeholder
			bool t = true;
			node->right = removeR(node->val, node->right, t);
		}
		return node;
	}


	void removeInorder(int n) 
	{
		// check of input
		if (n >= nodeCount||n < 0) {
			cout << "unsuccessful" << endl;
			return;
		}
		vector<Node*> nodeList;
		nodeCount--;
		int counter = 0;
		root = removeInorderR(root, n+1, counter);
		cout << "successful" << endl;
	}

	Node* removeInorderR(Node* node, int n, int &currCount) 
	{
		if (node == nullptr)
			return node;
		node->left = removeInorderR(node->left, n, currCount);
		currCount++;
		if (currCount == n) {
			return deleteNode(node);
		}
		else if (currCount > n) {
			return node;
		}
		node->right = removeInorderR(node->right, n, currCount);
		
		return node;

	}

	void search(int val) 
	{
		if (nodeCount == 0) {
			cout << "unsuccessful" << endl;
			return;
		}
		if (!searchNode(val, root))
			cout << "unsuccessful" << endl;
	}

	bool searchNode(int val, Node* node)
	{
		// Search for student with specifed ID, prints name
		// Returns false if ID isn't found
		if (node == nullptr)
			return false;
		if (node->val == val) {
			cout << node->name << endl;
			return true;
		}
		return searchNode(val, node->left) || searchNode(val, node->right);
	}


	void search(string name) 
	{
		if (nodeCount == 0) {
			cout << "unsuccessful" << endl;
			return;
		}
		vector<string> IDMatch;
		searchName(name, root, IDMatch);
		if (IDMatch.empty())
			cout << "unsuccessful" << endl;
		else {
			for (unsigned int i = 0; i < IDMatch.size(); i++) {
				cout << IDMatch[i] << endl;
			}
		}
			
	}
	void searchName(string n, Node* node,vector<string> &s) 
	{
		if (node == nullptr) {
			return;
		}
		if ((node->name).compare(n) == 0) {
			s.push_back(printID(node->val));
		}
		searchName(n, node->left,s);
	    searchName(n, node->right,s);
	}

	string printID(int id) 
	{
		string r = to_string(id);
		while (r.length() < 8) {
			r = "0" + r;
		}
		return r;
	}
	void printLevelCount() 
	{
		cout << height(root) << endl;
	}
	bool checkAVL() 
	{
		return checkAVL(root);
	}
	bool checkAVL(Node* root) {
		// From my Stepik, checks if tree is AVL
		// https://stepik.org/submissions/1590562?unit=400654
		// Helper function for me to check
		if (root == nullptr)
			return true;
		if (abs(height(root->left) - height(root->right)) > 1)
			return false;
		return checkAVL(root->left) && checkAVL(root->right);
	}
	int height(Node* root)
	{
		// From my Stepik Code, recursive height function
		// https://stepik.org/submissions/1590562?unit=400654
		if (root == nullptr)
			return 0;
		return 1 + max(height(root->left), height(root->right));
	}

	// Rotate Methods initially coded in Stepik
	// https://stepik.org/submissions/1480367?unit=379728
	// https://stepik.org/submissions/1480369?unit=379728
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


private:
	Node* root;
	int nodeCount;
};

int main()
{
	//Initializes AVL
	AVL ID;
	
	string line;
	int count;
	getline(cin, line);
	try {
		count = stoi(line);
	}
	catch (exception) {
		cout << "Fatal Error" << endl;
		return 0;
	}
	
	for (int i = 0; i < count; i++) {
		//ID.printInorder();
		getline(cin, line);
		auto pos1 = line.find(" ");
		string command = line.substr(0, pos1);

		if (command.compare("insert") == 0) {
			auto par1 = line.find("\"");
			par1++;
			auto par2 = line.find("\"",par1);
			string name = line.substr(par1, par2 - par1);

			// Checks to make sure name is a valid input (no numbers)
			bool valid = true;
			for (unsigned int i = 0; i < name.size(); i++) {
				if (!isalpha(name[i]) && !(name[i]==' ')) {
					valid = false;
					break;
				}
			}
			par2 += 2;
			string num = line.substr(par2);
			
			// Checks to make sure input is a 8 digit number
			if (num.size() != 8)
				valid = false;
			for (unsigned int i = 0; i < num.length(); i++) {
				if (!isdigit(num[i])) {
					valid = false;
					break;
				}
			}

			int idNum;
			try {
				idNum = stoi(num);
			}
			catch (exception) {
				valid = false;
			}
			
			if (valid) {
				ID.insert(name, idNum);
			}
			else
				cout << "unsuccessful" << endl;

		}
		else if (command.compare("remove") == 0) {
			pos1++;
			string num = line.substr(pos1);
			// Checks to make sure input is a 8 digit number
			bool valid = true;
			for (unsigned int i = 0; i < num.length(); i++) {
				if (!isdigit(num[i])) {
					valid = false;
					break;
				}
			}
			if (num.size() != 8|| !(valid))
				cout << "unsuccessful" << endl;

			else {
				int idNum = stoi(num);
				ID.remove(idNum);
			}
		}
		else if (command.compare("search") == 0) {
			
			if (line.find("\"") != string::npos) {
				auto par1 = line.find("\"");
				par1++;
				auto par2 = line.find("\"", par1);
				string name = line.substr(par1, par2 - par1);
				
				// Checks to make sure name contains no numbers
				bool validName = true;
				for (unsigned int i = 0; i < name.size(); i++) {
					if (!isalpha(name[i]) && !(name[i] == ' ')) {
						validName = false;
					}
				}
				if (validName)
					ID.search(name);
				else
					cout << "unsuccessful" << endl;
			}
			else {
				pos1++;
				string num = line.substr(pos1);
				int idNum;
				
				// Checks to see if number is all digits
				bool valid = true;
				for (unsigned int i = 0; i < num.length(); i++) {
					if (!isdigit(num[i])) {
						valid = false;
						break;
					}
				}
				if (num.size() != 8 || !(valid) ) {
					cout << "unsuccessful" << endl;
				}
				else {
					try {
						idNum = stoi(num);
						ID.search(idNum);
					}
					catch (exception) {
						cout<<"unsuccessful"<<endl;
					}
					
				}
			}
		}
		else if (command.compare("printInorder") == 0) {
			ID.printInorder();
		}
		else if (command.compare("printPreorder") == 0) {
			ID.printPreorder();
		}
		else if (command.compare("printPostorder") == 0) {
			ID.printPostorder();
		}
		else if (command.compare("printLevelCount") == 0) {
			ID.printLevelCount();
		}
		else if (command.compare("removeInorder") == 0) {
			pos1++;
			string num = line.substr(pos1);
			int idNum;

			// Checks if ID is all digits
			bool valid = true;
			for (unsigned int i = 0; i < num.length(); i++) {
				if (!isdigit(num[i])) {
					valid = false;
					break;
				}
			}
			try {
				idNum = stoi(num);
				if (valid)
					ID.removeInorder(idNum);
				else
					cout << "unsuccessful" << endl;
			}
			catch (exception) {
				cout << "unsuccessful" << endl;
			}
		}
		else {
			cout << "unsuccessful" << endl;
		}
	}
	return 0;
}