#pragma once
using namespace std;

struct Node
{
	Token token;
	Node* left;
	Node* right;
	Token returnToken()
	{
		return token;
	}
};

Node* createNewNode(Token t, Node* l, Node* r)
{
	Node* newNode = new Node();
	newNode->token = t;
	newNode->left = l;
	newNode->right = r;
	return newNode;
}

string printNode(Node* node)
{
	Token tempToken = node->returnToken();
	return tempToken.getValue() + " : " + tempToken.getType();
}