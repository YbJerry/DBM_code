module;

#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <fstream>

export module lexer;

import util;

using namespace std;

export class Lexer {
public:
	Lexer();
	vector<vector<pair<TOKEN, string>>> lex(void);
	vector<vector<pair<TOKEN, string>>> lex(filesystem::path path);
	vector<vector<pair<TOKEN, string>>> lex(string str);
private:
	vector<vector<pair<TOKEN, string>>> lex(istream& is);
};

Lexer::Lexer()
{
}

vector<vector<pair<TOKEN, string>>> Lexer::lex(void)
{
	vector<vector<pair<TOKEN, string>>> res(lex(cin));

	return res;
}

vector<vector<pair<TOKEN, string>>> Lexer::lex(filesystem::path path)
{
	ifstream ifs(path);
	vector<vector<pair<TOKEN, string>>> res(lex(ifs));

	return res;
}

vector<vector<pair<TOKEN, string>>> Lexer::lex(string str)
{
	stringstream ss;
	ss << str;
	return vector<vector<pair<TOKEN, string>>>(lex(ss));
}

vector<vector<pair<TOKEN, string>>> Lexer::lex(istream& is)
{
	vector<vector<pair<TOKEN, string>>> res;
	vector<pair<TOKEN, string>> vec;
	stringstream ss;
	for (char c; !is.eof();) {
		c = is.get();
		if (isdigit(c)) 
		{
			do {
				ss << c;
				c = is.get();
			} while (isdigit(c));
			is.unget();

			string s;
			ss >> s;
			vec.emplace_back(make_pair(TOKEN::NUM, s));
		}
		else if (isalpha(c)) 
		{
			do {
				ss << c;
				c = is.get();
			} while (isalpha(c));
			is.unget();

			string s;
			ss >> s;
			vec.emplace_back(make_pair(TOKEN::VAR, s));
		}
		// handling special variable x0
		else if (c == '@') 
		{
			vec.emplace_back(make_pair(TOKEN::SPVAR, "x0"));
		}
		else if (c == '-')
		{
			vec.emplace_back(make_pair(TOKEN::MINUS, "-"));
		}
		else if (c == '<')
		{
			c = is.get();
			if (c == '=') {
				vec.emplace_back(make_pair(TOKEN::LEQ, "<="));
			}
			else {
				vec.emplace_back(make_pair(TOKEN::LT, "<"));
				is.unget();
			}
		}
		else if (c == '&')
		{
			res.push_back(vec);
			vec.clear();
		}
		else if (c == '\n' || c == EOF) {
			res.push_back(vec);
			break;
		}
		else if (c == ' ') {
			continue;
		}
		else {
			throw out_of_range("Can't identify character");
		}
		ss.clear();
	}

	return res;
}
