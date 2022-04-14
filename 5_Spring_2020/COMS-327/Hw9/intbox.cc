/**
 * Classes to hold numbers and demonstrate polymorphism
 * @author Alexander Harms
 * ajharms
 */
#include "intbox.hh"

// Determines if we need to print NULLs depending on piazza response
#define PRINT_NULLS true

// Singleton contstructor
singleton::singleton(int v) : value(v) { }

bool singleton::contains(int a) {
    return value == a;
}

void singleton::show(std::ostream &s) {
    s << value;
}

interval::interval(int l, int h) : low(l), high(h) {
    if (low > high) throw "Error, low is greater than high";
}

bool interval::contains(int a) {
    return a >= low && a <= high;
}

void interval::show(std::ostream &s) {
    s << "[" << low << ", " << high << "]";
}

collection::collection(unsigned s) : size(s) {
    array = new intbox*[size];
    for (int i = 0; i < size; i++) {
        array[i] = NULL;
    }
}

collection::~collection() {
    delete(array);
}

void collection::set_item(unsigned i, intbox* item) {
    array[i] = item;
}

bool collection::contains(int a) {
    for (int i = 0; i < size; i++) if (array[i] != NULL && (*array[i]).contains(a)) return true;
    return false;
}

void collection::show(std::ostream &s) {
    s << "{";
    for (int i = 0; i < size; i++) {
        if (PRINT_NULLS && array[i] == NULL) {
            s << "NULL";
        } else {
            (*array[i]).show(s);
        }

        if (i < size - 1) {
            s << ", ";
        } 
    }
    s << "}";
}