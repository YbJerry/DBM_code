#include <iostream>
#include <filesystem>
import lexer;
import parser;
import DBM;

int main()
{
    Lexer lexer;
    Parser parser(lexer.lex());
    parser.parse();

    DBM dbm(parser.getClockConstraints(), parser.getSymbolTable());
    dbm.print();

    return 0;
}