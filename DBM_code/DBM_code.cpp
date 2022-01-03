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
    string s0 = "0<=x & 0<=y & y<=5"s;
    string s1 = "0<=x & x<=8 & 0<=y & y <= 10";
    string s0tos1 = "0<=x & 3<=y"s;
    string s1tos0 = "6<=x & 4<=y"s;
    DBM dbm1("0<=x & 0<=y"s);
    dbm1.initZero();
    dbm1.print();

    DBM succ1 = succ(dbm1, s0, s0tos1, set<string>{"y"});
    succ1.print();

    DBM succ2 = succ(succ1, s1, s1tos0, set<string>{"x"});
    succ2.print();

    DBM succ3 = succ(succ2, s0, s0tos1, set<string>{"y"});
    succ3.print();

    DBM succ4 = succ(succ3, s1, s1tos0, set<string>{"x"});
    succ4.print();

    DBM succ5 = succ(succ4, s0, s0tos1, set<string>{"y"});
    succ5.print();

    return 0;
}