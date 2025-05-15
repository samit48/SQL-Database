#ifndef RELATIONAL_H
#define RELATIONAL_H
#include <cassert>
#include <iostream>
#include "../tokenizer/token.h"
using namespace std;

class Relational : public Token
{
public:
    Relational(){}
    Relational(string str):_str(str){}
    string token_string(){return _str;}
    virtual TokenType type() {return RELATIONAL;}

private:
    string _str;
};

#endif
