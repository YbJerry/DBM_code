module;

export module ClockConstraints:BaseClockConstraints;

import util;

export enum class CCType
{
    CV,     // c < x
    VC,     // x < c
    VMVC    // x - y < c
};

export class BaseClockConstraints
{
public:
    BaseClockConstraints(PRECEDES prec);
    virtual ~BaseClockConstraints();

    PRECEDES getPrecedes();
    virtual CCType getType() = 0;
private:
    PRECEDES prec;
};

BaseClockConstraints::BaseClockConstraints(PRECEDES prec) : prec(prec)
{
    
}

BaseClockConstraints::~BaseClockConstraints()
{
}

PRECEDES BaseClockConstraints::getPrecedes()
{
    return prec;
}