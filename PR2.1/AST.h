#pragma once
#include "Token.h"
using namespace std;

Node* parseExperssion(vector<Token> tokens);
int tokenIndex;

void consume_token()
{
	tokenIndex++;
}

Node* parseLeaf(vector<Token> tokens)
{
	if ((tokens[tokenIndex].getType() == "NUMBER") || (tokens[tokenIndex].getType() == "IDENTIFIER"))
	{
		int curIndex = tokenIndex;
		consume_token();
		Node* leaf = createNewNode(tokens[curIndex], NULL, NULL);
		return leaf;
	}
	return NULL;
}

Node* parseElement(vector<Token> tokens)
{
	Node* tree = NULL;
	if (!(tokenIndex >= tokens.size()))
	{
		if (tokens[tokenIndex].getValue() == "(")
		{
			consume_token();
			tree = parseExperssion(tokens);
			if (!(tokenIndex >= tokens.size()) && tokens[tokenIndex].getValue() == ")")
			{
				consume_token();
				return tree;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return parseLeaf(tokens);
		}
	}
	return tree;
}

Node* parsePiece(vector<Token> tokens)
{
	Node* tree = parseElement(tokens);
	if (tree == NULL)
	{
		return NULL;
	}
	if (!(tokenIndex >= tokens.size()) && (tokens[tokenIndex].getValue() == "*"))
	{
		int curIndex = tokenIndex;
		consume_token();
		tree = createNewNode(tokens[curIndex], tree, parseElement(tokens));
	}
	return tree;
}

Node* parseFactor(vector<Token> tokens)
{
	Node* tree = parsePiece(tokens);
	if (tree == NULL)
	{
		return NULL;
	}
	if (!(tokenIndex >= tokens.size()) && (tokens[tokenIndex].getValue() == "/"))
	{
		int curIndex = tokenIndex;
		consume_token();
		tree = createNewNode(tokens[curIndex], tree, parsePiece(tokens));
	}
	return tree;
}

Node* parseTerm(vector<Token> tokens)
{
	Node* tree = parseFactor(tokens);
	if (tree == NULL)
	{
		return NULL;
	}
	if (!(tokenIndex >= tokens.size()) && (tokens[tokenIndex].getValue() == "-"))
	{
		int curIndex = tokenIndex;
		consume_token();
		tree = createNewNode(tokens[curIndex], tree, parseFactor(tokens));
	}
	return tree;
}

Node* parseExperssion(vector<Token> tokens)
{
	Node* tree = parseTerm(tokens);
	if (tree == NULL)
	{
		return NULL;
	}
	if (!(tokenIndex >= tokens.size()) && (tokens[tokenIndex].getValue() == "+"))
	{
		int curIndex = tokenIndex;
		consume_token();
		tree = createNewNode(tokens[curIndex], tree, parseTerm(tokens));
	}
	return tree;
}

Node* parse(vector<Token> tokens)
{
	tokenIndex = 0;
	Node* root = parseExperssion(tokens);
	return root;
}

void printAST(Node* root, int height, ofstream &outputFile)
{
	if (root != NULL)
	{
		for (int i = 0; i < height; i++)
		{
			outputFile << "    ";
		}
		outputFile << printNode(root) << endl;
		printAST(root->left, height + 1, outputFile);
		printAST(root->right, height + 1, outputFile);
	}
}