/**
 * @author Alexander Harms
 * ajharms
 * Counts number of usages of words in a file
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * A node for the linked list
 */
struct node {
    struct node *next;
    char *string;
    unsigned count;
};

/**
 * Hashmap
 */
struct hashtable {
    struct node **nodes;
    unsigned size;
};

/**
 * Initializes a hashtable
 * @param T
 * hashtable to initialize
 * @param size
 * size to initialize to
 */
void init_table(struct hashtable* T, unsigned size) {
    T->nodes = calloc(sizeof(struct node*), size);
    T->size = size;
}

/**
 * Frees the memory used by a hashtable destroying it
 * @param T
 * hashtable to free
 */
void deinit_table(struct hashtable* T) {
    for (int i = 0; i < T->size; i++) for (struct node *n = *(T->nodes + i), *nn; n; n = nn) {
            nn = n->next;
            free(n->string);
            free(n);
        }
    free(T->nodes);
}

/**
 * Hashes a string
 * @param str
 * string to hash
 * @return
 * hash of the string
 */
unsigned long hash(char *str) {
    unsigned long hash = 5381;
    while (*str) hash = ((hash << 5) + hash) + *str++;
    return hash;
}

/**
 * Adds a word to the table if it isnt already there,
 * if it is increments the count
 * @param T
 * hashtable to add to
 * @param str
 * string to add
 */
void add_to_table(struct hashtable* T, char* str) {
    if (!*str) return;
    // Copy the string
    char *s = calloc(sizeof(char), (strlen(str) + 1));
    for (char *c = s; c && *str; *c = (char) tolower(*str), str++, c++);

    // Get the string's hash and the bucket for that hash
    unsigned h = hash(s) % T->size;
    struct node *bucket = *(T->nodes + h);

    // Search linked list for the string
    while (bucket != 0 && strcmp(bucket->string, s) != 0) bucket = bucket->next;

    // If the string wasn't found, attempt to create a new node and add it
    if (!bucket && (bucket = calloc(sizeof(struct node), 1))) {
        // Link to head
        bucket->next = *(T->nodes + h);
        // Connect node to head
        *(T->nodes + h) = bucket;
        // Add string to node
        bucket->string = s;
    } else if (!bucket) {
        fprintf(stderr, "ERROR: FAILED TO ADD NODE!\n");
        deinit_table(T);
        exit(-1);
    } else {
        free(s);
    }

    // Increment the count
    bucket->count++;
}

/**
 * Prints a hashtable in the format:
 * count str
 * @param T
 * hashtable to print
 */
void show_table(struct hashtable* T) {
    // For each bucket in the table, loop through each node if available and print the string and count
    for (int i = 0; i < T->size; i++) for (struct node *n = *(T->nodes + i); n; n = n->next) printf("%d %s\n", n->count, n->string);
}

/**
 * Takes input from stdin in the form of:
 * hashtable_size
 * str
 * str
 * str
 * EOF
 * @return
 * 0 for succesful exit
 */
int main() {
    // Get the size
    unsigned size;
    scanf("%d", &size);

    // Initialize hashtable
    struct hashtable h;
    init_table(&h, size);

    // Add strings until either EOF is encountered or a string is >32 characters
    // Hint: use Ctrl-d for EOF
    for (char s[34] = {0}; !feof(stdin); scanf("%s", s)) if (strlen(s) > 32) {
        fprintf(stderr, "ERROR: STRING LONGER THAN 32 CHARACTERS!\n");
        deinit_table(&h);
        exit(-1);
    } else add_to_table(&h, s);

    // Print the table
    show_table(&h);
    deinit_table(&h);

    return 0;
}

