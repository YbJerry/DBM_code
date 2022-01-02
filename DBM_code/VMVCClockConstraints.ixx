module;

#include <string>

export module ClockConstraints:VMVCClockConstraints;

import :BaseClockConstraints;

using namespace std;

export class VMVCClockConstraints : public BaseClockConstraints
{
public:
	VMVCClockConstraints(string val1, string val2, PRECEDES prec, int num);
	CCType getType() override;
	string getValue1();
	string getValue2();
	int getNumber();
private:
	string val1;
	string val2;
	int num;
};

VMVCClockConstraints::VMVCClockConstraints(string val1, string val2, PRECEDES prec, int num) : BaseClockConstraints(prec), num(num), val1(val1), val2(val2)
{

}

CCType VMVCClockConstraints::getType()
{
	return CCType::VMVC;
}

string VMVCClockConstraints::getValue1()
{
	return val1;
}

string VMVCClockConstraints::getValue2()
{
	return val2;
}

int VMVCClockConstraints::getNumber()
{
	return num;
}
