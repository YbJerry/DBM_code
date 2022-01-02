#include <iostream>
#include <filesystem>
#include <set>
import lexer;
import parser;
import DBM;
import util;

using namespace std;

int main()
{
    DBM dbm1("0<=x & 0<=y"s);
    dbm1.initZero();
    dbm1.print();

    DBM succDbm1 = succ(dbm1, "0<=x & 0<=y & y<=5"s, "0<=x & 3<=y"s, set<string>{"y"});
    succDbm1.print();

    return 0;
}