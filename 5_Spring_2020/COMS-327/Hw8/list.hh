/**
 * Class to build a list and then create a fixed list
 * @author Alexander Harms
 * ajharms
 */
#ifndef LIST_HH
#define LIST_HH

/**
 * Struct to hold a value and reference to the next node in the list
 */
struct node {
    int value;
    struct node *next;
};

/**
 * Class to build a list
 */
class list_builder {
    private:
    // First and last nodes
        struct node *first;
        struct node *last;
        // Size of list
        unsigned size;
        // Prevent copy/assignments
        inline void operator=(const list_builder& A);

    public:
        // Constructor
        list_builder();
        // Destructor
        ~list_builder();
        // Add a value to the front of the list
        void add_to_front(int value);
        // Add a value to the back of the list
        void add_to_back(int value);
        // Remove all elements from the list
        void reset();
        // Allow fixed list to access elements of the list
        friend class fixed_list;
};

// Class to create a fixed list from a list_builder
class fixed_list {
    private:
        // Array pointer
        int *list;
        // Size of the array
        unsigned size;
        // Prevent assignment/copy
        inline void operator=(const list_builder& A);

    public:
        // Constructor
        fixed_list(const list_builder &lb);
        // Destructor
        ~fixed_list();
        // Returns the size of the list
        int Size();
        // Allows indexing of the list
        int& operator[](int);
        // Allows the list to be printed as an output stream
        friend std::ostream &operator<< (std::ostream &output, fixed_list &r);
};

#endif