/**
 * Class to build a list and then create a fixed list
 * @author Alexander Harms
 * ajharms
 */
#include "list.hh"
#include <iostream>

list_builder::list_builder() {
    first = NULL;
    last = NULL;
    size = 0;
}

list_builder::~list_builder() {
    reset();
}

void list_builder::add_to_front(int value) {
    struct node *n = new struct node;
    n->value = value;
    n->next = first;

    if (first == NULL) {
        last = n;
    }
    first = n;

    size++;
}

void list_builder::add_to_back(int value) {
    struct node *n = new struct node;
    n->value = value;
    n->next = NULL;

    if (last == NULL) {
        first = last = n;
    } else {
        last->next = n;
        last = n;
    }

    size++;
}

void list_builder::reset() {
    while (first != NULL) {
        last = first->next;
        delete(first);
        first = last;
        size--;
    }
}

fixed_list::fixed_list(const list_builder &lb) {
    list = new int[lb.size];
    size = lb.size;

    struct node *n = lb.first;
    for (int i = 0; i < size; i++) {
        list[i] = n->value;
        n = n->next;
    }
}

fixed_list::~fixed_list() {
    delete(list);
}

int fixed_list::Size() {
    return size;
}

int& fixed_list::operator[](int index) {
    if (index >= size) {
        std::cerr << "Error, index out of range\n";
    }
    return list[index];
}

std::ostream &operator<< (std::ostream &output, fixed_list &fl) {
    output << "[";
    for (int i = 0, s = fl.Size(); i < s; i++) {
        output << fl[i];
        if (i < s - 1) {
            output << ", ";
        }
    }
    output << "]";
    return output;
}