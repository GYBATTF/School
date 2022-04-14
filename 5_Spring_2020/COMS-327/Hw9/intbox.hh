/**
 * Classes to hold numbers and demonstrate polymorphism
 * @author Alexander Harms
 * ajharms
 */
#include <iostream>

class intbox {
    public:

        virtual bool contains(int a) = 0;
        virtual void show(std::ostream &s) = 0;
};

class singleton : public intbox {
    int value;

    public:
        singleton(int v);
        bool contains(int a);
        void show(std::ostream &s);
};

class interval : public intbox {
    int low;
    int high;

    public:
        interval(int l, int h);
        bool contains(int a);
        void show(std::ostream &s);
};

class collection : public intbox {
    unsigned size;
    intbox **array;

    public:
        collection(unsigned s);
        ~collection();
        void set_item(unsigned i, intbox* item);
        bool contains(int a);
        void show(std::ostream &s);
};