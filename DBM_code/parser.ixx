module;

#include <vector>
#include <string>
#include <set>
#include <iostream>

export module parser;

// init := exp CONJ init | exp;
// exp  := VAR prec NUM
//		|  NUM prec VAR
//		|  VAR MINUS VAR prec NUM;
// prec := LT | LEQ;

import util;
import ClockConstraints;

using namespace std;

export class Parser {
public:
	Parser(vector<vector<pair<TOKEN, string>>> stats);
	~Parser();
	void parse();
	vector<BaseClockConstraints*> getClockConstraints();
	set<string> getSymbolTable();
	
private:
	bool isVC(vector<pair<TOKEN, string>> vec) const;
	bool isCV(vector<pair<TOKEN, string>> vec) const;
	bool isVMVC(vector<pair<TOKEN, string>> vec) const;

	vector<vector<pair<TOKEN, string>>> stats;
	// clock-constraints
	vector<BaseClockConstraints*> vecCC;
	// symbol table
	set<string> st;
};

Parser::Parser(vector<vector<pair<TOKEN, string>>> stats) :stats(stats)
{
	
}

Parser::~Parser()
{
	for (auto c : vecCC) {
		delete c;
	}
}

void Parser::parse()
{
	for (const auto &stat:stats)
	{
		if (isVC(stat)) {
			string val = stat[0].second;
			PRECEDES prec = (stat[1].first == TOKEN::LT) ? PRECEDES::LT : PRECEDES::LEQ;
			int num = stoi(stat[2].second);
			if(stat[0].first == TOKEN::VAR)
				st.insert(val);
			vecCC.push_back(new VCClockConstraints(val, prec, num));
		}
		else if (isCV(stat))
		{
			int num = stoi(stat[0].second);
			PRECEDES prec = (stat[1].first == TOKEN::LT) ? PRECEDES::LT : PRECEDES::LEQ;
			string val = stat[2].second;
			if (stat[2].first == TOKEN::VAR)
				st.insert(val);
			vecCC.push_back(new CVClockConstraints(num, prec, val));
		}
		else if (isVMVC(stat))
		{
			string val1 = stat[0].second;
			string val2 = stat[2].second;
			PRECEDES prec = (stat[3].first == TOKEN::LT) ? PRECEDES::LT : PRECEDES::LEQ;
			int num = stoi(stat[4].second);
			if (stat[0].first == TOKEN::VAR)
				st.insert(val1);
			if (stat[2].first == TOKEN::VAR)
				st.insert(val2);
			vecCC.push_back(new VMVCClockConstraints(val1, val2, prec, num));
		}
		else 
		{
			throw out_of_range("invavilid grammar");
		}
	}
}

vector<BaseClockConstraints*> Parser::getClockConstraints()
{
	return vecCC;
}

set<string> Parser::getSymbolTable()
{
	return st;
}

bool Parser::isVC(vector<pair<TOKEN, string>> vec) const
{
	if(vec.size() != 3)
		return false;
	if ((vec[0].first == TOKEN::VAR || vec[0].first == TOKEN::SPVAR) &&
		(vec[1].first == TOKEN::LT || vec[1].first == TOKEN::LEQ) &&
		vec[2].first == TOKEN::NUM)
		return true;
	return false;
}

bool Parser::isCV(vector<pair<TOKEN, string>> vec) const
{
	if (vec.size() != 3)
		return false;
	if (vec[0].first == TOKEN::NUM &&
		(vec[1].first == TOKEN::LT || vec[1].first == TOKEN::LEQ) &&
		(vec[2].first == TOKEN::VAR || vec[2].first == TOKEN::SPVAR))
		return true;
	return false;
}

bool Parser::isVMVC(vector<pair<TOKEN, string>> vec) const
{
	if (vec.size() != 5)
		return false;
	if ((vec[0].first == TOKEN::VAR || vec[0].first == TOKEN::SPVAR) &&
		vec[1].first == TOKEN::MINUS &&
		(vec[2].first == TOKEN::VAR || vec[2].first == TOKEN::SPVAR) &&
		(vec[3].first == TOKEN::LT || vec[3].first == TOKEN::LEQ) &&
		vec[4].first == TOKEN::NUM )
		return true;
	return false;
}
