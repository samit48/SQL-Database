#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
using namespace std;
#include "constants.h"
#include <vector>
#include <algorithm>
#include <string>
#include "../../includes/table/typedefs.h"
// #include "../../includes/table/table.h"

// print
// typeof
class Token
{
public:
    Token();
    Token(string token, TokenType type);
    friend ostream &operator<<(ostream &outs, const Token &t)
    {
        outs << "|" << t._token << "|" << endl;
        return outs;
    }
    virtual void Print(ostream &outs = cout)
    {
        outs << "|" << _token << "|" << endl;
    }
    TokenType type() { return _type; }
    string type_string() const { return _token; }
    string token_str() const { return _token; }
    int get_prec()
    {
        int _prec;
        if (_type == LOGICAL)
        {
            if (_token == "or")
            {
                return _prec = 4;
            }
            else{
                return _prec = 3;
            }
        }
        else if (_type == RELATIONAL)
        {
            _prec = 2;
        }
        else
        {
            cout << "UNKOWN ERROR in get_prec()" << endl;
            _prec = -1;
        }
        return _prec;
    }

private:
    string _token;
    TokenType _type;
};

//--------------------------------------------------------------------------------
class TokenStr : public Token
{
public:
    TokenStr() : _str("") {}
    TokenStr(string str) : _str(str), Token(str, TOKEN_STR) {}
    string token_string() { return _str; }
    virtual TokenType type() { return TOKEN_STR; }
    virtual void Print(ostream &outs = cout)
    {
        outs << "|" << _str << "|" << endl;
    }

private:
    string _str;
};

//--------------------------------------------------------------------------------

class LParen : public Token
{
public:
    LParen() : Token("(", LPAREN) {}
    virtual TokenType type() { return LPAREN; }
    virtual void Print(ostream &outs = cout)
    {
        outs << "|" << lparen << "|" << endl;
    }

private:
    string lparen;
};

//--------------------------------------------------------------------------------

class Rparen : public Token
{
public:
    Rparen() : Token(")", RPAREN) {}
    virtual TokenType type() { return RPAREN; }
    virtual void Print(ostream &outs = cout)
    {
        outs << "|" << rparen << "|" << endl;
    }

private:
    string rparen;
};

//--------------------------------------------------------------------------------

class ResultSet : public Token
{
public:
    ResultSet(const vectorlong recons) : Token("", RESULT_SET), _recons(recons) {}
    // "and" it returs the same nunmbers 
    Token *and_with(ResultSet *other)
    {
        vectorlong temp = other->get_recons();
        vectorlong r_set;
        sort(_recons.begin(), _recons.end());
        sort(temp.begin(), temp.end());
        set_union(_recons.begin(), _recons.end(), temp.begin(), temp.end(), back_inserter(r_set));
        return new ResultSet(r_set);
    }
    // "or" it returns all nunmbers 
    Token *or_with(ResultSet *other)
    {
        vectorlong temp = other->get_recons();
        vectorlong r_set;
        sort(_recons.begin(), _recons.end());
        sort(temp.begin(), temp.end());
        set_intersection(_recons.begin(), _recons.end(), temp.begin(), temp.end(), back_inserter(r_set));
        return new ResultSet(r_set);
    }

    vectorlong get_recons()
    {
        return _recons;
    }

    virtual void Print(ostream &outs = cout)
    {
        for (int i = 0; i < _recons.size(); i++)
        {
            outs << _recons[i];
        }
    }

private:
    vectorlong _recons;
};

//--------------------------------------------------------------------------------

class Operator : public Token
{
public:
    Operator() {}
    Operator(const string &op) : Token(op, _TOKEN_OPERATOR), _op(op)
    {
        _op = op;
        _prec = get_prec();
        if (op == "and")
        {
            _prec = 3;
        }
        else if (op == "or")
        {
            _prec = 3;
        }
        else if (op == "=" || op == ">" || op == "<" || op == ">=" || op == "<=")
        {
            _prec = 3;
        }
        else
        {
            _prec = 1;
        }
    }

    virtual int get_prec() { return _prec; }
    string get_operator() { return _op; }

    virtual TokenType type() { return _TOKEN_OPERATOR; }

    virtual void Print(ostream &outs = cout)
    {
        outs << "|" << _op << "|" << endl;
    }

private:
    int _prec;
    string _op;
};

//--------------------------------------------------------------------------------

class Relational : public Token
{
public:
    Relational() { _str = ""; }
    Relational(string str) : Token(str, RELATIONAL), _str(str) {}

    virtual void Print(ostream &outs = cout)
    {
        outs << "|" << _str << "|" << endl;
    }
    //This function evaluates a condition on a map and returns a set of record numbers 
    ResultSet *eval(mmap_sl map, Token *left, Token *right)
    {
        mmap_sl _map = map;
        string value = right->token_str();
        /*
        vectorlong _recons;
        _recons = map.at(value);

        cout << "right->token_str()" << value << endl;
        cout << "left->token_str()" << left->token_str() << endl;
        cout << "_str" << _str << endl; */

        vectorlong rec_num;
        if (_str == "=")
        {
            rec_num = map.at(value);
        }
        else if (_str == "<")
        {
            MMap<string, long>::Iterator it = map.begin();
            for (; it != map.lower_bound(value); it++)
            {
                rec_num += (*it).value_list;
            }
        }
        else if (_str == ">")
        {
            MMap<string, long>::Iterator it = map.upper_bound(value);

            for (; it != map.end(); it++)
            {
                rec_num += (*it).value_list;
            }
        }
        else if (_str == ">=")
        {
            MMap<string, long>::Iterator it = map.lower_bound(value);
            for (; it != map.end(); it++)
            {
                if ((*it).key >= value)
                {
                    rec_num += (*it).value_list;
                }
            }
        }

        else if (_str == "<=")
        {
            MMap<string, long>::Iterator it = map.begin();
            for (; it != map.upper_bound(value); it++)
            {
                if ((*it).key <= value)
                {
                    rec_num += (*it).value_list;
                }
            }
        }

        return new ResultSet(rec_num);
        // ResultSet(recons)
    }

    string token_string() { return _str; }
    virtual TokenType type() { return RELATIONAL; }

private:
    string _str;
    int _op;
};

//--------------------------------------------------------------------------------

class Logical : public Token
{
public:
    Logical() { _str = ""; }
    Logical(string str) : Token(str, LOGICAL), _str(str) {}

    virtual void Print(ostream &outs = cout)
    {
        outs << "|" << _str << "|" << endl;
    }
    //evaluates logical operations ("and" or "or") 
    Token *eval(Token *left, Token *right)
    {
        Token *_token;
        if (_str == "and")
        {
            _token = static_cast<ResultSet *>(left)->or_with(static_cast<ResultSet *>(right));
        }
        else if (_str == "or")
        {
            _token = static_cast<ResultSet *>(left)->and_with(static_cast<ResultSet *>(right));
        }
        else
        {
            cout << "-----ERROR-----" << endl;
            return nullptr;
        }
        return _token;
    }
    string token_string() { return _str; }
    virtual TokenType type() { return LOGICAL; }

private:
    string _str;
    int _op;
};

//--------------------------------------------------------------------------------
#endif