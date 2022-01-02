module;

#include <vector>
#include <string>

export module util;

//export typedef std::vector<std::pair<TOKEN, std::string>> ;

export enum class TOKEN
{
    VAR,    // variable
    SPVAR,  // special variable x0
    NUM,    // digital number
    MINUS,
    LT,     // less than
    LEQ,    // less than or equal to
    CONJ    // conjunction
};
export enum class PRECEDES
{
    LT,
    LEQ
};