#ifndef RPN_H
#define RPN_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "../../includes/table/typedefs.h"
#include "../../includes/tokenizer/token.h"

class RPN
{
public:
    RPN();
    RPN(Queue<Token*> postfix, vector<mmap_sl> indices, map_sl field_map);
    vectorlong operator ()();
    void checkTokenStrCount(Queue<Token*>& _postfix);
    friend ostream& operator<<(ostream& outs, const RPN& value) {
        outs << value._postfix << endl;
        return outs;
    }
private:
   Queue<Token*> _postfix;
   vector<mmap_sl> _indices;
   map_sl _field_map;
};

#endif