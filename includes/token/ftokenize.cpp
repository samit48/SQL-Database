#include <iostream>
#include <cstring>
#include <string>
#include <fstream>

#include "../../includes/token/constants_stk.h"
#include "../../includes/token/stokenize.h"
#include "../../includes/token/ftokenize.h"

using namespace std;

FTokenizer::FTokenizer(char *fname) : _stk(), _pos(0), _blockPos(0)
{
    _f.open(fname);
    if (_f.fail())
    {
        cout << "FAILED TO OPEN" << endl;
    }
    else
    {
        cout << "File opened successfully" << endl;
    }
    get_new_block();
    _more = true;
}

TOKENIZER FTokenizer::next_token()
{
    TOKENIZER t;
    if (_stk.done())
    {
        if (get_new_block())
        {
            _pos = _f.gcount();
            _more = true;
            cout << "New block retrieved" << endl;
        }
        else
        {
            _more = false;
            cout << "No more blocks to retrieve" << endl;
        }
    }
    _stk >> t;
    return t;
}

bool FTokenizer::get_new_block()
{
    char temp[MAX_BUFFER];
    _f.read(temp, MAX_BUFFER - 1);
    temp[_f.gcount()] = '\0';
    _stk.set_string(temp);

    if (_f.gcount() <= 0)
    {
        _f.close();
        cout << "End of file reached" << endl;
        return false;
    }

    cout << "Block retrieved from file" << endl;
    return true;
}


bool FTokenizer::more()
{
    return _more;
}

int FTokenizer::pos()
{
    return _pos;
}

int FTokenizer::block_pos()
{
    return _blockPos;
}

FTokenizer &operator>>(FTokenizer &f, TOKENIZER &t)
{
    t = f.next_token();
    return f;
}
