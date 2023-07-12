#include <iostream>
//#include "Token.h"
using namespace std;

Node* parseExperssion(vector<Token> tokens);
int tokenIndex;

Node* parseStatement(vector<Token> tokens);
Node* parseBaseStatement(vector<Token> tokens);
Node* parseWhileStatement(vector<Token> tokens);
Node* parseIfStatement(vector<Token> tokens);
Node* parseAssignment(vector<Token> tokens);


void consume_token()
{
    tokenIndex++;
}

Node* parseLeaf(vector<Token> tokens)
{
    if ((tokens[tokenIndex].get_type() == "NUMBER") || (tokens[tokenIndex].get_type() == "IDENTIFIER") || (tokens[tokenIndex].get_value() == "skip"))
    {
        int curIndex = tokenIndex;
        consume_token();
        Node* leaf = createNewNode(tokens[curIndex], NULL, NULL, NULL);
        
        return leaf;
        
    } 

    return NULL;
}

Node* parseElement(vector<Token> tokens)
{
    Node* tree = NULL;
    if (!(tokenIndex >= tokens.size()))
    {
        if (tokens[tokenIndex].get_value() == "(")
        {
            consume_token();
            tree = parseExperssion(tokens);
            if (!(tokenIndex >= tokens.size()) && tokens[tokenIndex].get_value() == ")")
            {
                consume_token();
                return tree;
            }
            else
            {
                throw "Exception";
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
    while (!(tokenIndex >= tokens.size()) && (tokens[tokenIndex].get_value() == "*"))
    {
        int curIndex = tokenIndex;
        consume_token();
        tree = createNewNode(tokens[curIndex], tree, NULL, parseElement(tokens));
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
    while (!(tokenIndex >= tokens.size()) && (tokens[tokenIndex].get_value() == "/"))
    {
        int curIndex = tokenIndex;
        consume_token();
        tree = createNewNode(tokens[curIndex], tree, NULL, parsePiece(tokens));
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
    while(!(tokenIndex >= tokens.size()) && (tokens[tokenIndex].get_value() == "-"))
    {
        int curIndex = tokenIndex;
        consume_token();
        tree = createNewNode(tokens[curIndex], tree, NULL, parseFactor(tokens));
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
    while ((tokenIndex <= tokens.size()) && (tokens[tokenIndex].get_value() == "+"))
    {
        int curIndex = tokenIndex;
        consume_token();
        tree = createNewNode(tokens[curIndex], tree, NULL, parseTerm(tokens));
    }
    return tree;
}

Node* parseAssignment(vector<Token> tokens) {
    if (tokens[tokenIndex].get_type() == "IDENTIFIER") {
        Token x(tokens[tokenIndex].get_value(), tokens[tokenIndex].get_type());
        Node* treeX = createNewNode(x, NULL, NULL, NULL);
        consume_token();
        if (tokens[tokenIndex].get_value() == ":=") {
            Token assignment(tokens[tokenIndex].get_value(), tokens[tokenIndex].get_type());
            consume_token();
            Node* tree1 = parseExperssion(tokens);
            Node* tree = createNewNode(assignment, treeX, NULL, tree1);
            return tree;
        }
    }
    throw "Exception";
}

Node* parseIfStatement(vector<Token> tokens) {
    if (tokens[tokenIndex].get_value() == "if") {
        consume_token();
        Node* tree1 = parseExperssion(tokens);
        if (tokens[tokenIndex].get_value() == "then") {
            consume_token();
            Node* tree2 = parseStatement(tokens);
            if (tokens[tokenIndex].get_value() == "else") {
                consume_token();
                Node* tree3 = parseStatement(tokens);
                if (tokens[tokenIndex].get_value() == "endif") {
                    consume_token();
                    Token tokenIf("IF-STATEMENT", "");
                    Node* tree = createNewNode(tokenIf, tree1, tree2, tree3);
                    return tree;
                }
            }
        }
    }
    throw "Exception";
}

Node* parseWhileStatement(vector<Token> tokens) {
    if (tokens[tokenIndex].get_value() == "while") {
        consume_token();
        Node* tree1 = parseExperssion(tokens);
        if (tokens[tokenIndex].get_value() == "do") {
            consume_token();
            Node* tree2 = parseStatement(tokens);
            if (tokens[tokenIndex].get_value() == "endwhile") {
                consume_token();
                Token tokenWhile("WHILE-LOOP", "");
                Node* tree = createNewNode(tokenWhile, tree1, NULL, tree2);
                return tree;
            }
        }
    }
    throw "Exception";
}

Node* parseBaseStatement(vector<Token> tokens) {
    if (tokens[tokenIndex].get_type() == "IDENTIFIER") {
        return parseAssignment(tokens);
    }
    else if (tokens[tokenIndex].get_value() == "if") {
        return parseIfStatement(tokens);
    }
    else if (tokens[tokenIndex].get_value() == "while") {
        return parseWhileStatement(tokens);
    }
    else if (tokens[tokenIndex].get_value() == "skip") {
        return parseLeaf(tokens);
    }
    else {
        throw "Exception";
    }
}

Node* parseStatement(vector<Token> tokens) {
    Node* tree = parseBaseStatement(tokens);
    if (tree == NULL) {
        return NULL;
    }
    while ((tokenIndex <= tokens.size()) && (tokens[tokenIndex].get_value() == ";")) {
        int curIndex = tokenIndex;
        consume_token();
        tree = createNewNode(tokens[curIndex], tree, NULL, parseBaseStatement(tokens));
    }
    return tree;
}



Node* parse(vector<Token> tokens)
{
    tokenIndex = 0;
    Node* root = parseStatement(tokens);
    return root;
}

void printAST(Node* root, int height, ofstream &outputFile) {
	if (root == NULL) return;
    if (root != NULL)
	{
		for (int i = 0; i < height; i++)
		{
			outputFile << "    ";
		}
		outputFile << printNode(root) << endl;
		printAST(root->left, height + 1, outputFile);
        printAST(root->mid, height + 1, outputFile);
		printAST(root->right, height + 1, outputFile);
	}
}