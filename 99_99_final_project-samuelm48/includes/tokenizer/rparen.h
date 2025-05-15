#ifndef RPAREN_H
#define RPAREN_H

#include <iostream>
using namespace std;
#include "constants.h"
#include "token.h"

class Rparen : public Token
{
public:
    Rparen() : Token("(", RPAREN){}
    virtual TokenType type(){return RPAREN;}
    virtual void Print(ostream& outs=cout);
private:
    string rparen;
};

#endif