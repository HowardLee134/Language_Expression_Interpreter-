using namespace std;

struct Node
{
    Token token;
    Node* left;
    Node* mid;
    Node* right;
    Token returnToken()
    {
        return token;
    }
};

Node* createNewNode(Token t, Node* l, Node* m, Node* r)
{
    Node* newNode = new Node();
    newNode->token = t;
    newNode->left = l;
    newNode->mid = m;
    newNode->right = r;
    return newNode;
}

string printNode(Node* node)
{
    Token tempToken = node->returnToken();
    string result;
    if (tempToken.get_type() != "") {
        result = tempToken.get_value() + " : " + tempToken.get_type();
    }
    else {
        result = tempToken.get_value();
    }
    return result;
}