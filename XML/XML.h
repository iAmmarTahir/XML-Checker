#ifndef XML_H
#define XML_H
#include <iostream>
#include "Stack.h"

using namespace std;

class XML
{
private:
	int lineNumber;
	string tag;
public:
	XML();
	XML(const int& _lineNumber,const string& _tag);
	~XML();
	friend ostream& operator<<(ostream& cout, const XML &rhs);
	bool operator==(const string &rhs);
};


XML::XML()
{
	lineNumber = 0;
	tag = "";
}

XML::~XML()
{}

XML::XML(const int& _lineNumber, const string& _tag)
{
	lineNumber = _lineNumber;
	tag = _tag;
}

ostream& operator<<(ostream& cout, const XML &rhs)
{
	cout << "<" << rhs.tag.c_str() << ">" << " on line number: " << rhs.lineNumber << '.' << endl;
	return cout;
}

bool XML::operator==(const string& rhs)
{
	return tag.compare(rhs) == 0;
}

#endif