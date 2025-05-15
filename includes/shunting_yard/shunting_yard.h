#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include <string>
#include <cmath>
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "../../includes/tokenizer/token.h"

class ShuntingYard {
   public:
    ShuntingYard();

    ShuntingYard(Queue<Token*> input);

    Queue<Token*> postfix();
    Queue<Token*> postfix(Queue<Token*> input){
        _infix = input;
        return postfix();
    }
    void infix(Queue<Token*> infix);
    bool has_more_logical_tokens() const;

    friend ostream& operator<<(ostream& outs, const ShuntingYard& s) {
        outs << s._infix << endl;
        return outs;
    }

   private:
    Queue<Token*> _infix;
};



#endif // SHUNTING_YARD_H
