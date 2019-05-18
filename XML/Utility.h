#ifndef UTILITY_H
#define UTILITY_H

#include <fstream>
#include <string>
#include "XML.h"

using namespace std;

class Utility
{
public:
	static void CheckFile();	// Tests the whole file for errors
	static bool PrologChecker(const string &buffer);	
	static bool AttributesChecker(const string &buffer, int &i);	// Checks the attributes of each tag
	static bool PrologAttributesChecker(const string &buffer, int &i);   // Checks the specific attributes of prolog
	static bool QuotesChecker(const string &buffer, int &i);		// Checks the quotes in each attributes
	static void TagsChecker(ifstream &input);		// Checks each tag of file
	static void PrintError(const XML& tag);			
	static bool CommentsChecker(const string &buffer, int &i);
};

void Utility::CheckFile()
{
	
	ifstream input("XML.txt");
	
	string buffer;
	getline(input, buffer);
	
	if (!PrologChecker(buffer))
	{
		XML tag(1, "xml");
		PrintError(tag);
	}
	else
	{
		TagsChecker(input);			// Rest of file is checked in this function
	}
	input.close();
}

bool Utility::PrologAttributesChecker(const string &buffer, int &i)
{
	string attribute;
	for (; buffer[i] != ' ' && buffer[i] != '='; i++)	// Copying the attribute of prolog into a string for checking
	{
		attribute.push_back(buffer[i]);
	}
	
	if (attribute.compare("version") != 0 && attribute.compare("encoding") != 0)	// Comparison with allowed attributes in prolog
	{
		return false;
	}
	
	if (buffer[i] == '=')	// Checking quotes
	{
		if (!QuotesChecker(buffer, i))	
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool Utility::PrologChecker(const string &buffer)
{
	if (buffer[0] == '<' && buffer[1] == '?')	// Checking the starting of prolog is <? or not
	{
		if (buffer[2] == 'x' && buffer[3] == 'm' && buffer[4] == 'l')	// Checking after there is xml written or not 
		{
			int sz = buffer.length();
			int i;
			
			for (i = 5; i < sz && buffer[i] != '?'; i++)
			{
				if (isalpha(buffer[i]))
				{
					if (!PrologAttributesChecker(buffer, i))	
					{
						return false;
					}
				}
			}
			if (buffer[i] == '?' && buffer[i + 1] == '>')	// Checking if the prolog is ended with ?> or not
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}	
	}
	else
	{
		return false;
	}
}

bool Utility::AttributesChecker(const string &buffer, int &i)
{
	int sz = buffer.length();
	for (; i < sz - 1 && buffer[i] != '='; i++);		// Traversing an attribute to get to '=' or if there is no '=' then error

	if (buffer[i] == '=')
	{
		if (!QuotesChecker(buffer, i))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool Utility::QuotesChecker(const string &buffer, int &i)
{
	Stack<char> quotes;		// For matching quotes
	int sz = buffer.length();
	
	i++;	// The index is at '=' but we need to compare ' or ", so we add 1

	if (buffer[i] == '\'' || buffer[i] == '"')
	{
		quotes.push(buffer[i]);
	}
	else
	{
		return false;
	}

	i++;	// Quotes have been matched, now we need its index to be incremented to check its contents
	
	while (i < sz-1 && buffer[i] != '\'' && buffer[i] != '"')	// Traversing to find either ' or "
	{
		i++;
	}

	if (buffer[i] == '"' || buffer[i] == '\'')
	{
		char temp = quotes.top();
		
		if (temp == buffer[i])			// Checking if the opening quotes are equal to closing quotes and popping it
		{
			quotes.pop(temp);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Utility::PrintError(const XML& tag)
{
	cout << "ERROR: " << tag;
}

void Utility::TagsChecker(ifstream &input)
{
	Stack<XML> tags;	// Stack to check opening and ending tags
	
	string buffer;
	int lineNumber = 1;		// For the records of lines in file
	
	bool isError = false;
	while (getline(input, buffer) && !isError)
	{
		int i = 0;
		lineNumber++;
		int sz = buffer.length();
		
		string tempTag;
		
		for (; i < sz && !isError; i++)
		{
			if (buffer[i] == '<' && isalpha(buffer[i+1]))		// Checking if there is opening tag
			{
				i++;
				for (; i < sz && buffer[i] != '>' && buffer[i] != ' ' && !isError;i++)		// Copying the tag name
				{
					tempTag.push_back(buffer[i]);
				}

				XML tag(lineNumber, tempTag);
				tempTag.clear();	// To handle multiple tags on a single line

				if (buffer[i] == ' ')	// Checking attributes of tags
				{
					for (; i < sz && buffer[i] != '>' && !isError; i++)
					{
						if (isalpha(buffer[i]))
						{
							if (!AttributesChecker(buffer, i))
							{
								PrintError(tag);
								isError = true;
							}
						}
					}
					if (buffer[i] == '>')
					{
						tags.push(tag);
					}
				}
				else if (buffer[i] == '>')		// If closing > is found then push the tag onto stack
				{
					tags.push(tag);
				}
				else
				{
					PrintError(tag);
					isError = true;
				}
			}
			else if (buffer[i] == '<' && buffer[i + 1] == '/')		// Checking for closing tags
			{
				string closingTag;
				i += 2;		// Incrementing to skip < and / checking	

				for (; i < sz && buffer[i] != '>'; i++)		// Copying closing tag name for comparison
				{
					closingTag.push_back(buffer[i]);
				}

				if (buffer[i] == '>')
				{
					XML temp;
					if (!tags.isEmpty())	// For handling the case where closing tag appears but there is no opening tag of it
					{
						if (tags.top() == closingTag)
						{
							tags.pop(temp);
						}
						else
						{
							PrintError(tags.top());
							isError = true;
						}
					}
					else
					{
						isError = true;
						cout << "ERROR: <\\" << closingTag << "> on line number " << lineNumber << ".\n";
					}
				}
				else
				{
					PrintError(tags.top());
					isError = true;
				}
			}
			else if (buffer[i] == '<' && buffer[i + 1] == '!' && buffer[i + 2] == '-' && buffer[i + 3] == '-')  // Checking for comments
			{
				if (!CommentsChecker(buffer, i))
				{
					cout << "ERROR: Comments have invalid syntax on line number " << lineNumber << ".\n";
					isError = true;
				}
			}
		}
	}
	if (tags.isEmpty() && !isError)
	{
		cout << "The file has no ERRORS.\n";
	}
}

bool Utility::CommentsChecker(const string &buffer, int &i)
{
	int sz = buffer.length();
	for (; i < sz ; i++)
	{
		if (buffer[i] == '-' && buffer[i + 1] == '-' && buffer[i + 2] == '>')
		{
			i += 3;		// Skipping --> as it is already being checked
			return true;
		}
	}
	return false;
}

#endif // !UTILITY_H