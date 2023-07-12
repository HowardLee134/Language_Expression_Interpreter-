#include <string>
using namespace std;
class Token
{

public:
    Token()
    {
        value = "";
        type = "";
    }
    Token(string v, string t)
    {
        value = v;
        type = t;
    }
    void set_value(string v)
    {
        value = v;
    }
    string get_value()
    {
        return value;
    }
    void set_type(string t)
    {
        type = t;
    }
    string get_type()
    {
        return type;
    }
private:
    string value;
    string type;
};
