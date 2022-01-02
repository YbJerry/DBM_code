module;

#include <vector>
#include <string>
#include <optional>
#include <algorithm>

export module util;

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

export bool operator<(const std::optional<int>& c1, const std::optional<int>& c2) {
    if (c1 && !c2) {
        return true;
    }
    else if (!c1 && c2)
    {
        return false;
    }
    else if(c1 && c2) {
        return c1.value() < c2.value();
    }
    else {
        return false;
    }
}

export bool operator==(const std::optional<int>& c1, const std::optional<int>& c2) {
    if (c1 && !c2 || !c1 && c2) {
        return false;
    }
    else if (!c1 && !c2)
    {
        return false;
    }
    else {
        return c1.value() == c2.value();
    }
}

export std::optional<int> min(const std::optional<int>& c1, const std::optional<int>& c2) {
    if (c1 && c2) {
        return std::optional<int>(std::min(*c1, *c2));
    }
    else if (c1 && !c2)
    {
        return c1;
    }
    else if (!c1 && c2) {
        return c2;
    }
    else {
        return {};
    }
}