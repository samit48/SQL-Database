#include "shunting_yard.h"
#include <string>
#include <stdexcept>


ShuntingYard::ShuntingYard()
{
}

ShuntingYard::ShuntingYard(Queue<Token *> input)
{
    _infix = input;
}

void ShuntingYard::infix(Queue<Token *> infix)
{
    _infix = infix;
}

bool ShuntingYard::has_more_logical_tokens() const
{
    Queue<Token *> temp = _infix;
    while (!temp.empty())
    {
        Token *token = temp.pop();
        if (token->token_str() == "and")
        {
            return true;
        }
    }
    return false;
}
//converts an infix expression (stored in _infix) into a postfix expression
//basically if Operator push to stack and if string push queue
//pop stach and push to queue when there is a higher precednece operator
Queue<Token *> ShuntingYard::postfix()
{
    Stack<Token *> shunt;
    Queue<Token *> postfix;

    while (!_infix.empty())
    {
        Token *token = _infix.pop();
        if (token->type() == TOKEN_STR)
        {
            postfix.push(token);
        }
        else if (token->type() == LOGICAL || token->type() == RELATIONAL)
        {
            if (!shunt.empty())
            {
                while ((!shunt.empty()) && (shunt.top()->type() != LPAREN) && (token->get_prec() >= shunt.top()->get_prec()))
                {
                    if (token->type() == LOGICAL || token->type() == RELATIONAL)
                    {
                        postfix.push(shunt.pop());
                    }
                }
                shunt.push(token);
            }
            else
            {
                shunt.push(token);
            }
        }
        else if (token->type() == RPAREN)
        {
            bool leftParenFound = false;
            while (!shunt.empty())
            {
                if (shunt.top()->type() == LPAREN)
                {
                    leftParenFound = true;
                    shunt.pop();
                    break;
                }
                postfix.push(shunt.pop());
            }
            if (!leftParenFound)
            {
                throw 7;
            }
        }
        else if (token->type() == LPAREN)
        {
            shunt.push(token);
        }
    }

    while (!shunt.empty())
    {
        if (shunt.top()->type() == LPAREN)
        {
            throw 8;
        }
        else
        {
            postfix.push(shunt.pop());
        }
    }

    return postfix;
}