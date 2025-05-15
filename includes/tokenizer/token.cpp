#include "../../includes/tokenizer/token.h"
#include "../../includes/tokenizer/constants.h"

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// default ctor
Token::Token():_token(),_type(){}

// takes and assigns token and type
Token::Token(string token, TokenType type){
    _token = token;
    _type = type;
}

/* string Token::type_string() const{
    //figures out what type of token it is
    switch (_type)
    {
    case LPAREN:
        return "(";
        break;
    case RPAREN:
        return ")";
        break;
    case TOKEN_STR:
        return "TOKEN_STR";
        break;
    case OPERATOR:
        return "OPERATOR";
        break;
    case RELATIONAL:
        return "RELATIONAL";
        break;
     case LOGICAL:
        return "LOGICAL";
        break;
     case RESULT_SET:
        return "RESULT_SET";
        break;
    case TOKEN_UNKNOWN:
        return "UNKNOWN";
        break;
    default:
        return ("UNKNOWN");
        break;
    }
    
} */



