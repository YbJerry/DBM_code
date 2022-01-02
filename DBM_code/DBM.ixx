module;

#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <iomanip>
#include <format>

export module DBM;

import lexer;
import parser;
import ClockConstraints;
import DBMItem;
import util;

using namespace std;

/// <summary>
/// <c>DBM</c> is data structure of Difference-Bound Matrices.
/// </summary>
export class DBM {
public:
	DBM();
	DBM(const DBM& dbm);
	DBM(DBM&& dbm);
	DBM(string str);
	DBM(vector<BaseClockConstraints*> vecCC, set<string> st);
	void print();
	void init();
	void initZero();

	DBM& operator=(const DBM& dbm);
	// intersection
	DBM operator&(const DBM& dbm);
	// clock reset
	DBM reset(set<string> rs);
	// elasping of time
	DBM operator++(int);
private:
	/// <summary>
	/// find DBM's canonical form
	/// </summary>
	void calCanonicalForm();

	vector<vector<DBMItem>> matrices;
	// a haspmap from symbol to matrices's index
	unordered_map<string, int> valIdx;
	// a haspmap from matrices's index to symbol
	unordered_map<int, string> idxVal;
};

DBM::DBM()
{
}

DBM::DBM(const DBM& dbm)
{
	*this = dbm;
}

DBM::DBM(DBM&& dbm)
{
	matrices = move(dbm.matrices);
	valIdx = move(dbm.valIdx);
	idxVal = move(dbm.idxVal);
}

DBM::DBM(string str)
{
	Lexer lexer;
	Parser parser(lexer.lex(str));
	parser.parse();
	*this = move(DBM(parser.getClockConstraints(), parser.getSymbolTable()));
}

DBM::DBM(vector<BaseClockConstraints*> vecCC, set<string> st): matrices(vector<vector<DBMItem>>(st.size()+1, vector<DBMItem>(st.size()+1)))
{
	valIdx["x0"] = 0;
	idxVal[0] = "x0";
	for (int i = 1; auto& s : st)
	{
		idxVal[i] = s;
		valIdx[s] = i++;
	}
	for (size_t i = 0; i < valIdx.size(); i++)
	{
		matrices[i][i].set(0, PRECEDES::LEQ);
	}
	for (auto cc : vecCC)
	{
		switch (cc->getType())
		{
		case CCType::CV: {
			auto *pcc = dynamic_cast<CVClockConstraints*>(cc);
			int i = valIdx[pcc->getValue()];
			matrices[0][i].set(-pcc->getNumber(), pcc->getPrecedes());
		}
			break;
		case CCType::VC: {
			auto *pcc = dynamic_cast<VCClockConstraints*>(cc);
			int i = valIdx[pcc->getValue()];
			matrices[i][0].set(pcc->getNumber(), pcc->getPrecedes());
		}
			break;
		case CCType::VMVC: {
			auto *pcc = dynamic_cast<VMVCClockConstraints*>(cc);
			int i = valIdx[pcc->getValue1()];
			int j = valIdx[pcc->getValue2()];
			matrices[i][j].set(pcc->getNumber(), pcc->getPrecedes());
		}
			break;
		default:
			throw out_of_range("not a valid ClockConstraint type");
			break;
		}
	}
	calCanonicalForm();
}

void DBM::print()
{
	const int tabNum = 10;
	cout << setw(tabNum) << " ";
	for (auto& [_, val] : idxVal)
	{
		cout << setw(tabNum) << val;
	}
	cout << endl;

	for (size_t i = 0, n = valIdx.size(); i < n; ++i) {
		cout << setw(tabNum) << idxVal[static_cast<int>(i)];
		for (int j = 0; j < n; ++j) {
			const auto& item = matrices[i][j];
			string prec = (item.getPrecedes() == PRECEDES::LEQ) ? "<=" : "<";
			string num = "¡Þ";
			if (auto t = item.getNumber())
				num = to_string(*t);
			cout << setw(tabNum) << format("({}, {})", num, prec);
		}
		cout << endl;
	}
	
}

void DBM::init()
{
	size_t n = matrices.size();
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++) 
		{
			matrices[i][j].reset();
		}
	}
}

void DBM::initZero()
{
	size_t n = matrices.size();
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			matrices[i][j].set(0, PRECEDES::LEQ);
		}
	}
}

DBM& DBM::operator=(const DBM& dbm)
{
	matrices = dbm.matrices;
	valIdx = dbm.valIdx;
	idxVal = dbm.idxVal;
	return *this;
}

DBM DBM::operator&(const DBM& dbm)
{
	DBM res(*this);
	size_t n = matrices.size();
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++) 
		{
			const auto &c1 = matrices[i][j].getNumber();
			const auto &c2 = dbm.matrices[i][j].getNumber();
			const auto &prec1 = matrices[i][j].getPrecedes();
			const auto &prec2 = dbm.matrices[i][j].getPrecedes();
			auto &d = res.matrices[i][j];
			d.set(min(c1, c2));
			if (c1 < c2) {
				d.set(prec1);
			}
			else if (c2 < c1)
			{
				d.set(prec2);
			}
			else
			{
				if (prec1 == prec2) {
					d.set(prec1);
				}
				else {
					d.set(PRECEDES::LT);
				}
			}
		}
	}
	res.calCanonicalForm();
	return res;
}

DBM DBM::reset(set<string> rs)
{
	DBM res(*this);
	size_t n = res.matrices.size();
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			const auto& xi = idxVal[i];
			const auto& xj = idxVal[j];
			auto& item = res.matrices[i][j];
			if (rs.count(xi) && rs.count(xj)) {
				item.set(0, PRECEDES::LEQ);
			}
			else if (rs.count(xi) && !rs.count(xj))
			{
				const auto& tmp = res.matrices[0][j];
				item.set(*tmp.getNumber(), tmp.getPrecedes());
			}
			else if (!rs.count(xi) && rs.count(xj))
			{
				const auto& tmp = res.matrices[i][0];
				item.set(tmp.getNumber(), tmp.getPrecedes());
			}
		}
	}
	res.calCanonicalForm();
	return res;
}

DBM DBM::operator++(int)
{
	DBM res(*this);
	size_t n = res.matrices.size();
	for (size_t i = 1; i < n; i++)
	{
		res.matrices[i][0].reset();
	}
	res.calCanonicalForm();
	return res;
}

void DBM::calCanonicalForm()
{
	size_t n = matrices.size();
	for (size_t k = 0; k < n; k++) 
	{
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < n; j++) 
			{
				const auto& item1 = matrices[i][k];
				const auto& item2 = matrices[k][j];
				auto& itemRes = matrices[i][j];
				if (item1.getNumber() && item2.getNumber()) {
					unique_ptr<DBMItem> itemTempPtr;
					if (item1.getPrecedes() == PRECEDES::LEQ && item2.getPrecedes() == PRECEDES::LEQ) {
						itemTempPtr = make_unique<DBMItem>(item1.getNumber().value() + item2.getNumber().value(), PRECEDES::LEQ);
					}
					else {
						itemTempPtr = make_unique<DBMItem>(item1.getNumber().value() + item2.getNumber().value(), PRECEDES::LT);
					}
					
					if(!(itemRes < *itemTempPtr))
						itemRes = *itemTempPtr;
				}
			}
		}
	}
}

export DBM succ(DBM& d, string invariantOfLocation, string guardOfTransition, set<string> resetClocks) {
	DBM Is(invariantOfLocation);
	DBM g(guardOfTransition);
	return ((d & Is)++ & Is & g).reset(resetClocks);
}
