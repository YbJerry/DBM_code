#include <iostream>
#include <filesystem>
#include <set>
import lexer;
import parser;
import DBM;

using namespace std;

int main()
{
    DBM dbm1("0<=x & 0<=y"s);
    dbm1.initZero();
    dbm1.print();

    DBM dbm2("0<=x & 0<=y & y<=5"s);
    dbm2.print();

    DBM dbm3 = dbm1 & dbm2;
    dbm3.print();

    DBM dbm4 = dbm3++;
    dbm4.print();

    DBM dbm5 = dbm4 & dbm2;
    dbm5.print();

    DBM dbm6("0<=x & 3<=y"s);
    dbm6.print();

    DBM dbm7 = dbm5 & dbm6;
    dbm7.print();

    DBM dbm8 = dbm7.reset(set<string>{"y"});
    dbm8.print();

    return 0;
}