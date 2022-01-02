module;

#include <string>

export module ClockConstraints:CVClockConstraints;

import :BaseClockConstraints;

using namespace std;

export class CVClockConstraints : public BaseClockConstraints 
{
public:
	CVClockConstraints(int num, PRECEDES prec, string val);
	CCType getType() override;
	string getValue();
	int getNumber();
private:

	int num;
	string val;
};

CVClockConstraints::CVClockConstraints(int num, PRECEDES prec, string val) : BaseClockConstraints(prec), num(num), val(val)
{
	
}

CCType CVClockConstraints::getType()
{
	return CCType::CV;
}

string CVClockConstraints::getValue()
{
	return val;
}

int CVClockConstraints::getNumber()
{
	return num;
}
