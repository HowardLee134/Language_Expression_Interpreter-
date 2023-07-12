#include <iostream>
#include <stack>
#include <string>
#include <stdexcept>
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
    if ((tokens[tokenIndex].get_type() == "NUMBER") || (tokens[tokenIndex].get_type() == "IDENTIFIER"))
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
                throw invalid_argument("no )");
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

Node* parse(vector<Token> tokens)
{
    tokenIndex = 0;
    Node* root = parseExperssion(tokens);
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

void evaluate(stack<Token> &stack) {
    Token sign;
    Token x;
    Token y;
    int result;
    y = stack.top();
    stack.pop();
    x = stack.top();
    stack.pop();
    sign = stack.top();
    stack.pop();
    if (sign.get_type() == "SYMBOL" && x.get_type() == "NUMBER" && y.get_type() == "NUMBER") {
        int num1 = stoi(x.get_value());
        int num2 = stoi(y.get_value());
        if (sign.get_value() == "+") {
            result = num1 + num2;
        }
        else if (sign.get_value() == "-") {
            result = num1 - num2;
        }
        else if (sign.get_value() == "*") {
            result = num1 * num2;
        }
        else if (sign.get_value() == "/") {
            if (num2 == 0) {
                throw invalid_argument ("Div by 0");
            }
            result = num1 / num2;
        }
        if (result < 0) {
            result = 0;
        }
        Token resultToPush(to_string(result), "NUMBER");
        stack.push(resultToPush);
    }
    else {
        stack.push(sign);
        stack.push(x);
        stack.push(y);
    }
}

void evaluateAST(Node* root, stack<Token> &stack) {
    if (root == NULL) {
        if (stack.size() >= 3) {
            evaluate(stack);
        }
        return;
    }
    // Token sign;
    // Token x;
    // Token y;
    // int result;
    Token t = root->returnToken();
    stack.push(t);
    if (stack.size() >= 3) {
        evaluate(stack);
    }
    evaluateAST(root->left, stack);
    evaluateAST(root->mid, stack);
    evaluateAST(root->right, stack);
}