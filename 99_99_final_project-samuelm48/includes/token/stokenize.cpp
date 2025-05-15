
#include <iostream>
#include <cstring>

#include "../../includes/token/constants_stk.h"
#include "../../includes/token/stokenize.h"
#include "../../includes/token/state_machine_functions.h"

using namespace std;

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer() : _pos(0)
{
    make_table(_table);
}

STokenizer::STokenizer(char str[]) : _pos(0)
{
    make_table(_table);
    set_string(str);
}

bool STokenizer::done()
{
    // return (_pos > length of the token char)
    return (_pos >= strlen(_buffer));
}
bool STokenizer::more()
{
    // return (_pos < length of the token char)
    return (_pos <= strlen(_buffer));
}

bool STokenizer::get_token(int &start_state, string &token)
{
    int length = strlen(_buffer);
    int current_state;

    if (_buffer[_pos] == '\0' || _pos > length)
    {
        _pos = length + 1;
        return false;
    }

    if (_buffer[_pos] == '\'' || _buffer[_pos] == '\"')
    {
        char quote_char = _buffer[_pos];
        _pos++;
        while (_pos < length && _buffer[_pos] != quote_char)
        {
            token += _buffer[_pos];
            _pos++;
        }
        if (_pos < length && _buffer[_pos] == quote_char)
        {
            _pos++;
        }
        start_state = 4;
        return true;
    }

    // Check for consecutive parentheses
    if (_buffer[_pos] == '(' && _buffer[_pos + 1] == '(')
    {
        token += '(';
        _pos++;
        return true;
    }
    if (_buffer[_pos] == ')' && _buffer[_pos + 1] == ')')
    {
        token += ')';
        _pos++;
        return true;
    }
    if (_buffer[_pos] == ',' && _buffer[_pos + 1] == ',')
    {
        token += ',';
        _pos++;
        return true;
    }

    current_state = _table[start_state][(int)_buffer[_pos]];

    while (current_state != -1 && _buffer[_pos] != '\0' &&
           (current_state < 100 && current_state >= 0))
    {
        if (current_state == 2 && _buffer[_pos + 1] == '\0')
        {
            break;
        }
        if (current_state == 0)
        {
            _pos = _pos + 2;
            break;
        }

        start_state = current_state;
        token += _buffer[_pos];
        _pos++;
        current_state = _table[current_state][(int)_buffer[_pos]];
    }

    return true;
}


void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    init_table(_table);

    // FOR DOUBLE
    // doubles:
    mark_fail(_table, 0);    // Mark states 0 and 2 as fail states
    mark_success(_table, 1); // Mark states 1 and 3 as success states
    mark_fail(_table, 2);
    mark_success(_table, 3);

    mark_success(_table, 4);
    mark_success(_table, 5);
    mark_success(_table, 6);
    mark_success(_table, 7);

    // Digits
    mark_cells(0, _table, DIGITS, 1);   // state [0] --- DIGITS ---> [1]
    mark_cells(0, _table, '.', '.', 2); // state [0] --- '.' ------> [2]
    mark_cells(1, _table, DIGITS, 1);   // state [1] --- DIGITS ---> [1]
    mark_cells(1, _table, '.', '.', 2); // state [1] --- '.' ------> [2]
    mark_cells(2, _table, DIGITS, 3);   // state [2] --- DIGITS ---> [3]
    mark_cells(3, _table, DIGITS, 3);   // state [3] --- DIGITS ---> [3]

    // Alpha
    mark_cells(0, _table, ALPHA, 4);
    // mark_cells(1, _table, ALPHA, 4);
    // mark_cells(2, _table, ALPHA, 4);
    mark_cells(4, _table, ALPHA, 4);

    // OPERATORS
    mark_cells(0, _table, OPERATORS, 6);
    // mark_cells(1, _table, OPERATORS, 6);
    // mark_cells(2, _table, OPERATORS, 6);
    // mark_cells(4, _table, OPERATORS, 6);
    // mark_cells(5, _table, OPERATORS, 6);
    mark_cells(6, _table, OPERATORS, 6);

    // SPACES
    mark_cells(0, _table, SPACES, 7);
    // mark_cells(1, _table, SPACES, 7);
    // mark_cells(2, _table, SPACES, 7);
    // mark_cells(4, _table, SPACES, 7);
    // mark_cells(5, _table, SPACES, 7);
    // mark_cells(6, _table, SPACES, 7);
    mark_cells(7, _table, SPACES, 7);
    // PUNC
    mark_cells(0, _table, PUNC, 5);
    // mark_cells(1, _table, PUNC, 5);
    // mark_cells(2, _table, PUNC, 5);
    // mark_cells(4, _table, PUNC, 5);
    mark_cells(5, _table, PUNC, 5);
}

void STokenizer::set_string(char str[])
{
    _pos = 0;
    strcpy(_buffer, str);
}
