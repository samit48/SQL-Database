#ifndef MyStack_H
#define MyStack_H
#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
#include "../../includes/node/node.h"
#include <iostream>
#include <cassert>
using namespace std;

template <typename T>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                     //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                //default ctor
        Iterator(node<T>* p){_ptr = p;}          //Point Iterator to where
                                                //  p is pointing to
        T &operator*()
        {
            return (_ptr->_item);
        } // dereference operator

        T *operator->()
        {
            return (_ptr->_next);
        } // member access operator
        bool is_null()
        {
            return (_ptr == nullptr);
        } // true if _ptr is NULL
        friend bool operator!=(const Iterator &left,
                               const Iterator &right)
        {
            if (left._ptr != right._ptr)
            {
                return true;
            }
            return false;
        } // true if left != right

        friend bool operator==(const Iterator &left,
                               const Iterator &right)
        {
            if (left._ptr == right._ptr)
            {
                return true;
            }
            return false;
        } // true if left == right

        Iterator &operator++()
        {
            _ptr = _ptr->_next;
            return *this;
        } // member operator:
          //   ++it; or
          //   ++it = new_value

        friend Iterator operator++(Iterator &it,
                                   int unused)
        {
            Iterator hold = it;
            it._ptr = it._ptr->_next;
            return hold;
        } // friend operator: it++

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Stack(){
        _top = nullptr;
        _size = 0;
    }

    Stack(const Stack<T>& copyMe){
        bool debug = true;
        if (debug)
        {
            //cout << "Stack List fired!!" << endl;
        }

        _copy_list(_top = nullptr, copyMe._top);
        _size = copyMe._size;
    }

    ~Stack(){
        bool debug = true;
        if (debug)
        {
            //cout << "~Stack() destructor!" << endl;
        }

        _clear_list(_top);
        _top = nullptr;
        _size = 0;
    }

    Stack<T>& operator=(const Stack<T>& RHS){
        bool debug = true;
        if (debug)
        {
            //cout << "Assignment Operator fired!" << endl;
        }
        if (this == &RHS)
        {
            if (debug)
            {
                //cout << "assignment operator: self reference." << endl;
            }
            return *this;
        }

        _clear_list(_top);
        _copy_list(_top = nullptr, RHS._top);
        _size = RHS._size;
        return *this;
    }

    T top();
    bool empty(){return _top == nullptr;}
    void push(T item);
    T pop();
    template<typename TT>
    friend ostream& operator<<(ostream& outs, 
                                        const Stack<TT>& printMe);

    Iterator begin() const;              //Iterator to the head node
    Iterator end() const;                //Iterator to NULL
    int size() const { return _size; }

private:
    node<T>* _top;
    int _size;
};


template <typename T>
T Stack<T>::top(){
    assert(_top != nullptr);
    return _top->_item;
}

template <typename T>
void Stack<T>::push(T item){
    _size++;
    _insert_head(_top, item);
}

template <typename T>
T Stack<T>::pop(){
    _size--;
    T hold = _delete_node(_top, _top);
    return hold;
}

template<typename TT>
ostream& operator<<(ostream& outs, const Stack<TT>& printMe){
    _print_list(printMe._top);
    return outs;
}

template <class T>
typename Stack<T>::Iterator Stack<T>::begin() const{
    return (Iterator(_top));
}

template <class T>
typename Stack<T>::Iterator Stack<T>::end() const{
    return(Iterator(nullptr));
} 







#endif

