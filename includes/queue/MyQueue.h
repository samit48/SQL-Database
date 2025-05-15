#ifndef MyQueue_H
#define MyQueue_H
#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
#include "../../includes/node/node.h"
#include <iostream>
#include <cassert>
using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator
    {
    public:
        friend class Queue;         // give access to list to access _ptr
        Iterator() { _ptr = NULL; } // default ctor
        Iterator(node<T> *p) {_ptr = p;}     // Point Iterator to where
                                //   p is pointing to
        T operator*()
        {
            return (_ptr->_item);
        } // dereference operator
        bool is_null()
        {
            return (_ptr == nullptr);
        } // true if _ptr is NULL
        friend bool operator!=(const Iterator &left,
                               const Iterator &right) // true if left != right
        {
            if (left._ptr != right._ptr)
            {
                return true;
            }
            return false;
        }

        friend bool operator==(const Iterator &left,
                               const Iterator &right) // true if left == right
        {
            if (left._ptr == right._ptr)
            {
                return true;
            }
            return false;
        }

        Iterator &operator++()
        { // member operator:++it
          //  or ++it = new_value
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator &it,
                                   int unused)
        { // friend operator: it++
        //assert(it._ptr!=NULL);
            Iterator hold = it;
            it._ptr = it._ptr->_next;
            return hold;
        }

    private:
        node<T> *_ptr; // pointer being encapsulated
    };

    Queue(){
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }

    Queue(const Queue<T> &copyMe){
        bool debug = false;
        if (debug)
        {
            cout << "Queue List fired!!" << endl;
        }

        _copy_list(_front = nullptr, copyMe._front);
        _last_node(_front);
        _size = copyMe._size;
    }
    ~Queue(){
         bool debug = false;
        if (debug)
        {
            cout << "~Queue() destructor!" << endl;
        }

        _clear_list(_front);
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }

    Queue &operator=(const Queue<T> &RHS){
        bool debug = false;
        if (debug)
        {
            cout << "Assignment Operator fired!" << endl;
        }
        if (this == &RHS)
        {
            if (debug)
            {
                cout << "assignment operator: self reference." << endl;
            }
            return *this;
        }
    
        _size = 0;
        _clear_list(_front);
        _rear =nullptr;
        _copy_list(_front = nullptr, RHS._front);
        _rear = _last_node(_front);
        _size = RHS._size;
        return *this;
    }

    bool empty(){return _front == nullptr;}
    T front();
    T back();

    void push(T item);
    T pop();

    Iterator begin() const; // Iterator to the head node
    Iterator end() const;   // Iterator to NULL
    void print_pointers();
    int size() const { return _size; }
    template <typename TT>
    friend ostream &operator<<(ostream &outs, const Queue<TT> &printMe);

private:
    node<T> *_front;
    node<T> *_rear;
    int _size;
};


template <typename T>
T Queue<T>::front(){
    return (_front->_item);
}

template <typename T>
T Queue<T>::back(){
    return (_rear->_item);
}

template <typename T>
void Queue<T>::push(T item){
    node<T>* new_node = new node<T>(item);
    
    if(empty()){
        _front = new_node;
        _rear = new_node;
    }else{
        _rear->_next = new_node;
        _rear = new_node;
    }
    
    _size++;
}

template <typename T>
T Queue<T>::pop(){
    T marker = _delete_node(_front, _front);
    return marker;
}

template <class T>
typename Queue<T>::Iterator Queue<T>::begin() const{
    return Iterator(_front);
}

template <class T>
typename Queue<T>::Iterator Queue<T>::end() const{
    return Iterator(nullptr);
}

template <typename T>
void Queue<T>::print_pointers(){
    _print_list(_front);
}

template <typename TT>
ostream &operator<<(ostream &outs, const Queue<TT> &printMe){
    _print_list(printMe._front);
    return outs;
}

#endif