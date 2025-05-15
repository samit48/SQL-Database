#include "rpn.h"
#include <string>
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(Queue<Token *> postfix, vector<mmap_sl> indices, map_sl field_map)
{
    _postfix = postfix;
    _indices = indices;
    _field_map = field_map;
}

// this function evaluates the postfix
// when it finds and operation it pops them then evaluates them
vectorlong RPN::operator()()
{
    Stack<Token *> temp;
    Token *left, *right, *result_set;
    checkTokenStrCount(_postfix);

    while (!_postfix.empty())
    {
        Token *_token = _postfix.pop();
        if (_token->type() == TOKEN_STR)
        {
            temp.push(_token);
        }
        else if (_token->type() == RELATIONAL || _token->type() == LOGICAL)
        {
            right = temp.pop();
            left = temp.pop();
            if (!left || !right)
            {
                throw 11; // Throw error code 11 for null pointer operands
            }
            mmap_sl _map = _indices.at(_field_map[left->token_str()]);
            if (_token->type() == RELATIONAL)
            {
                result_set = static_cast<Relational *>(_token)->eval(_map, left, right);
            }
            else if (_token->type() == LOGICAL)
            {
                result_set = static_cast<Logical *>(_token)->eval(left, right);
            }
            else
            {
                cout << "I DON'T KNOW WHATS WRONG" << endl;
            }
            temp.push(result_set);
        }
        else
        {
            cout << "I DON'T KNOW WHATS WRONG" << endl;
        }
    }
    return static_cast<ResultSet *>(temp.pop())->get_recons();
}

void RPN::checkTokenStrCount(Queue<Token *> &_postfix)
{
    int tokenStrCount = 0;

    Queue<Token *> tempQueue;

    // Iterate through the queue
    while (!_postfix.empty())
    {
        Token *token = _postfix.pop();
        tempQueue.push(token); // Push token to the temporary queue

        if (token->type() == TOKEN_STR)
        {
            tokenStrCount++;
        }
        else
        {
            tokenStrCount = 0;
        }

        if (tokenStrCount > 2)
        {
            throw 12;
        }
    }

    // Restore the original queue
    while (!tempQueue.empty())
    {
        _postfix.push(tempQueue.pop());
    }
}
