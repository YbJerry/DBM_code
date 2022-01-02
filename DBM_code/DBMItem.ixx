module;

#include <optional>

export module DBMItem;

import ClockConstraints;
import util;

using namespace std;

export class DBMItem {
public:
	DBMItem();
	DBMItem(const DBMItem& item);
	DBMItem& operator=(const DBMItem& item);
	DBMItem(int num, PRECEDES prec);
	
	//void set(int num);
	void set(optional<int> num);
	void set(PRECEDES prec);
	void set(int num, PRECEDES prec);
	void set(optional<int> num, PRECEDES prec);
	// reset to (¡Þ,<)
	void reset();
	PRECEDES getPrecedes() const;
	optional<int> getNumber() const;

	bool operator<(const DBMItem& item) const;
private:
	optional<int> num;
	PRECEDES prec;
};

DBMItem::DBMItem() :num(nullopt), prec(PRECEDES::LT)
{

}

DBMItem::DBMItem(const DBMItem& item): num(item.getNumber()), prec(item.getPrecedes())
{
}

DBMItem& DBMItem::operator=(const DBMItem& item)
{
	this->num = item.num;
	this->prec = item.prec;
	return *this;
}

DBMItem::DBMItem(int num, PRECEDES prec):num(num), prec(prec)
{

}

//void DBMItem::set(int num)
//{
//	this->num = num;
//}

void DBMItem::set(optional<int> num)
{
	this->num = num;
}

void DBMItem::set(PRECEDES prec)
{
	this->prec = prec;
}

void DBMItem::set(int num, PRECEDES prec)
{
	set(num);
	set(prec);
}

void DBMItem::set(optional<int> num, PRECEDES prec)
{
	set(num);
	set(prec);
}

void DBMItem::reset()
{
	num = {};
	prec = PRECEDES::LT;
}

PRECEDES DBMItem::getPrecedes() const
{
	return prec;
}

optional<int> DBMItem::getNumber() const
{
	return num;
}

bool DBMItem::operator<(const DBMItem& item) const
{
	if (num) {
		if (item.getPrecedes() == PRECEDES::LT) {
			return num < item.getNumber();
		}
		else {
			return num <= item.getNumber();
		}
	}
	return false;
}
