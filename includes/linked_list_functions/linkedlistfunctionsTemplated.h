#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H
#include "../node/node.h"
#include <cassert>
#include <iostream>
using namespace std;


template <typename ITEM_TYPE>
void _print_list(node<ITEM_TYPE> *head);

template <typename ITEM_TYPE>
void _print_list_backwards(node<ITEM_TYPE> *head);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_search_list(node<ITEM_TYPE> *head,
                              ITEM_TYPE key);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_head(node<ITEM_TYPE> *&head,
                              ITEM_TYPE insert_this);

// insert after ptr
template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_after(node<ITEM_TYPE> *&head,
                               node<ITEM_TYPE> *after_this,
                               ITEM_TYPE insert_this);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_before(node<ITEM_TYPE> *&head,
                                node<ITEM_TYPE> *before_this,
                                ITEM_TYPE insert_this);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_previous_node(node<ITEM_TYPE> *head,
                                node<ITEM_TYPE> *prev_to_this);

template <typename ITEM_TYPE>
ITEM_TYPE _delete_node(node<ITEM_TYPE> *&head,
                       node<ITEM_TYPE> *delete_this);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_copy_list(node<ITEM_TYPE> *head);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_copy_list(node<ITEM_TYPE> *&dest, node<ITEM_TYPE> *src);

template <typename ITEM_TYPE>
void _clear_list(node<ITEM_TYPE> *&head);

template <typename ITEM_TYPE>
ITEM_TYPE &_at(node<ITEM_TYPE> *head, int pos);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_sorted(node<ITEM_TYPE> *&head, // insert
                                ITEM_TYPE item,
                                bool ascending = true);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_sorted_and_add(node<ITEM_TYPE> *&head,
                                        ITEM_TYPE item,
                                        bool ascending = true);
// node after which this item goes order: 0 ascending
template <typename ITEM_TYPE>
node<ITEM_TYPE> *_where_this_goes(node<ITEM_TYPE> *head,
                                  ITEM_TYPE item,
                                  bool ascending = true);

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_last_node(node<ITEM_TYPE> *head);

//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//

template <typename ITEM_TYPE>
void _print_list(node<ITEM_TYPE> *head)
{
    for (node<ITEM_TYPE> *walker = head; walker != nullptr; walker = walker->_next)
    {
        cout << *walker;
    }
    cout << "|||";
    return;
}

template <typename ITEM_TYPE>
void _print_list_backwards(node<ITEM_TYPE> *head){
        
}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_search_list(node<ITEM_TYPE> *head,
                              ITEM_TYPE key)
{

    for (node<ITEM_TYPE> *walker = head; walker != nullptr; walker = walker->_next)
    {

        if (walker->_item == key)
        {
            return walker;
        }
    }

    return nullptr;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_head(node<ITEM_TYPE> *&head,
                              ITEM_TYPE insert_this)
{
    node<ITEM_TYPE> *temp = new node<ITEM_TYPE>(insert_this);
    temp->_next = head;
    head = temp;
    return temp;
}

// insert after ptr
template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_after(node<ITEM_TYPE> *&head,
                               node<ITEM_TYPE> *after_this,
                               ITEM_TYPE insert_this)
{
    if (head == nullptr)
    {
        _insert_head(head, insert_this);
        return head;
    }

    node<ITEM_TYPE> *temp = new node<ITEM_TYPE>(insert_this);
    temp->_next = after_this->_next;
    after_this->_next = temp;

    return temp;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_before(node<ITEM_TYPE> *&head,
                                node<ITEM_TYPE> *before_this,
                                ITEM_TYPE insert_this)
{
    node<ITEM_TYPE> *previous = _previous_node(head, before_this);
    if (previous == before_this)
    {
        _insert_head(head, insert_this);
        return head;
    }
    else
    {
        node<ITEM_TYPE> *temp = new node<ITEM_TYPE>(insert_this);
        temp->_next = before_this;
        previous->_next = temp;
        return temp;
    }
}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_previous_node(node<ITEM_TYPE> *head,
                                node<ITEM_TYPE> *prev_to_this)
{


    if (head == prev_to_this)
    {
        return prev_to_this;
    }
    node<ITEM_TYPE> *walker = head;


    while (walker != nullptr && walker->_next != prev_to_this)
    {
        walker = walker->_next;
    }

    return walker;
}

template <typename ITEM_TYPE>
ITEM_TYPE _delete_node(node<ITEM_TYPE> *&head,
                       node<ITEM_TYPE> *delete_this)
{

    if (head == nullptr || delete_this == nullptr)
    {
        return ITEM_TYPE();   
    }

    if(head == delete_this){
        head = delete_this->_next;
        ITEM_TYPE hold = delete_this->_item;
        delete delete_this;
            
        return hold;
    }
    

    node<ITEM_TYPE> *previous = _previous_node(head, delete_this);

    node<ITEM_TYPE> *walker = head;

    if (previous == delete_this)
    {
        walker = head->_next;
        ITEM_TYPE hold = head->_item;
        delete head;
        head = walker;
        return hold;
    }
    else
    {
        ITEM_TYPE hold = delete_this->_item;
        previous->_next = delete_this->_next;
        delete delete_this;
        return hold;
    }

}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_copy_list(node<ITEM_TYPE> *head)
{
    if (head == nullptr)
    {
        return nullptr;
    }

    node<ITEM_TYPE> *new_head = nullptr;
    _copy_list(new_head, head);
    return new_head;
}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_copy_list(node<ITEM_TYPE> *&dest, node<ITEM_TYPE> *src)
{

    node<ITEM_TYPE> *walker1 = dest;
    node<ITEM_TYPE> *walker2 = src;

    for (walker2 = src; walker2 != nullptr; walker2 = walker2->_next)
    {
        walker1 = _insert_after(dest, walker1, walker2->_item);
    }
    return walker1;
}

template <typename ITEM_TYPE>
void _clear_list(node<ITEM_TYPE> *&head)
{

    node<ITEM_TYPE> *walker = head;
    node<ITEM_TYPE> *next = nullptr;

    while (walker != nullptr)
    {
        next = walker->_next;
        delete walker;
        walker = next;
    }

    head = nullptr;
}

template <typename ITEM_TYPE>
ITEM_TYPE &_at(node<ITEM_TYPE> *head, int pos)
{
    assert(head != nullptr && pos >= 0);

    node<ITEM_TYPE> *walker = head;

    for (int i = 0; walker != nullptr; i++)
    {
        if (i == pos)
        {
            return walker->_item;
        }
        walker = walker->_next;
    }
    assert(walker != nullptr);
}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_last_node(node<ITEM_TYPE> *head)
{
    node<ITEM_TYPE> *walker = head;

    if (head == nullptr)
    {
        return nullptr;
    }

    while(walker->_next != nullptr)
    {
        walker = walker->_next;
    }

    return walker;

}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_sorted(node<ITEM_TYPE> *&head, // insert
                                ITEM_TYPE item,
                                bool ascending)
{
    node<ITEM_TYPE>* walker = _where_this_goes(head, item, ascending);
    if (walker == nullptr)
    {
        return _insert_head(head, item);
    }

    if (ascending)
    {
    if (walker->_item >= item)
    {
        return _insert_before(head, walker, item);
    }
    }
    else{
    if (walker->_item <= item)
    {
        return _insert_before(head, walker, item);
    }
    }
    



    
    return _insert_after(head,walker,item);

}

template <typename ITEM_TYPE>
node<ITEM_TYPE> *_insert_sorted_and_add(node<ITEM_TYPE> *&head,
                                        ITEM_TYPE item,
                                        bool ascending)
{
    node<ITEM_TYPE>* walker = _where_this_goes(head, item, ascending);
    if (walker->_item == item)
    {
        walker->_item += item;
        return walker;
    }
    else{
        return _insert_sorted(head, item, ascending);
    }
    
}

// node after which this item goes order: 0 ascending
template <typename ITEM_TYPE>
node<ITEM_TYPE> *_where_this_goes(node<ITEM_TYPE> *head,
                                  ITEM_TYPE item,
                                  bool ascending)
{
    if (head == nullptr)
    {
        return head;
    }
    node<ITEM_TYPE>* walker = head;
    node<ITEM_TYPE>* walker2 = head ->_next;

    while (walker2 != nullptr)
    {
        if (ascending)
        {
            if (item < walker2 ->_item)
            {
                return walker;
            }
            walker = walker ->_next;
            walker2 = walker2 ->_next;
        }
        else{
            if (item > walker2 ->_item)
            {
                return walker;
            }
            walker = walker->_next;
            walker2 = walker2 ->_next;
        }
    }
    return walker;    
}



#endif