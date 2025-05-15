#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <iostream>
using namespace std;



class TOKENIZER
{
public:
    TOKENIZER();
    TOKENIZER(string str, int type);
    friend ostream& operator <<(ostream& outs, const TOKENIZER& t);
    int type() const;
    string type_string() const;
    string token_str() const;
private:
    string _token;
    int _type;
};

#endif