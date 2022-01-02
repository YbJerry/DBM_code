module;

#include <string>

export module ClockConstraints:VCClockConstraints;

import :BaseClockConstraints;

using namespace std;

export class VCClockConstraints : public BaseClockConstraints
{
public:
	VCClockConstraints(string val, PRECEDES prec, int num);
	CCType getType() override;
	string getValue();
	int getNumber();
private:
	int num;
	string val;
};

VCClockConstraints::VCClockConstraints(string val, PRECEDES prec, int num) : BaseClockConstraints(prec), num(num), val(val)
{

}

CCType VCClockConstraints::getType()
{
	return CCType::VC;
}

string VCClockConstraints::getValue()
{
	return val;
}

int VCClockConstraints::getNumber()
{
	return num;
}
