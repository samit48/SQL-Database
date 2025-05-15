#include "../../includes/token/tokenizer.h"
#include "../../includes/token/constants_stk.h"

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// default ctor
TOKENIZER::TOKENIZER():_token(),_type(){}

// takes and assigns token and type
TOKENIZER::TOKENIZER(string s,int type):_token(s), _type(type){}

int TOKENIZER::type() const{
    //returns type of token
    return _type;
}

string TOKENIZER::type_string() const{
    //figures out what type of token it is
    switch (_type)
    {
    case TOKEN_NUMBER:
        return "NUMBER";
        break;
    case TOKEN_DIGITS:
        return "DIGITS";
        break;
    case 1:
        return "DIGITS";
        break;
    case TOKEN_ALPHA:
        return "ALPHA";
        break;
    case TOKEN_SPACE:
        return "SPACE";
        break;
    case TOKEN_OPERATOR:
        return "OPERATOR";
        break;
    case TOKEN_PUNC:
        return "PUNC";
        break;
    case TOKEN_UNKNOWN:
        return "UNKNOWN";
        break;
    default:
        return ("UNKNOWN");
        break;
    }
    
}

string TOKENIZER::token_str() const{
    //returns string token
    return _token;
}

ostream& operator <<(ostream& outs, const TOKENIZER& t){
    //prints the token string and the token type
    outs << "|" << t._token << "|" << endl;
    return outs;
}
