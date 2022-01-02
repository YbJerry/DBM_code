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
	DBM(vector<BaseClockConstraints*> vecCC, set<string> st);
	void print();
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
		cout << setw(tabNum) << idxVal[i];
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
					if (item1.getPrecedes() == PRECEDES::LEQ && item2.getPrecedes() == PRECEDES::LEQ) {
						itemRes.set(PRECEDES::LEQ);
					}
					else {
						itemRes.set(PRECEDES::LT);
					}
					itemRes.set(item1.getNumber().value() + item2.getNumber().value());
				}
			}
		}
	}
}
