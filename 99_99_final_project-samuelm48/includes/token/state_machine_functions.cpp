#include "../../includes/token/state_machine_functions.h"
#include <iomanip>
#include <cstring>

void init_table(int _table[][MAX_COLUMNS])
{
    for (int i = 0; i < MAX_ROWS; ++i)
    {
        for (int j = 0; j < MAX_COLUMNS; ++j)
        {
            _table[i][j] = -1;
        }
    }
}

void mark_success(int _table[][MAX_COLUMNS], int state)
{
    //_table[state][?]= 1 success
    _table[state][0] = 1;
}

void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    //_table[state][?]= 0 fail
    _table[state][0] = 0;
}

bool is_success(int _table[][MAX_COLUMNS], int state)
{
    return _table[state][0] == 1;
}

void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state)
{
    for (int i = from; i <= to; i++)
    {
        _table[row][i] = state;
    }
}

void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state)
{
    for (int i = 0; i < strlen(columns); i++)
    {
        char temp = columns[i];
        _table[row][int(temp)] = state;
    }
}

void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state)
{
    table[row][column] = state;
}

void print_table(int _table[][MAX_COLUMNS])
{
    for (int i = 0; i < MAX_ROWS; ++i)
    {
        for (int j = 0; j < MAX_COLUMNS; ++j)
        {
            cout << _table[i][j] << " ";
        }
        cout << endl;
    }
}

void show_string(char s[], int _pos)
{
    for (int i = 0; i < strlen(s); i++)
    {
        cout << s[i];
    }
    cout << endl
         << setw(_pos) << "^" << _pos << endl;
}