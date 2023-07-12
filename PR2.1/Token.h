#pragma once
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
	void setValue(string v)
	{
		value = v;
	}
	string getValue()
	{
		return value;
	}
	void setType(string t)
	{
		type = t;
	}
	string getType()
	{
		return type;
	}
private:
	string value;
	string type;
};

