/**
 * @author Alexander Harms
 * ajharms
 * Assigns nodes to variables and outputs how many are connected to one another
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * Node that a variable points to
 */
struct node {
    int refs;
    struct node *to;
};

/**
 * Variables that can have nodes assigned to
 */
struct vars {
    int numNodes;
    struct node **vars;
    struct node **nodes;
};

/**
 * Links variables to nodes or nodes to nodes
 * @param v
 * struct of variables
 * @param type
 * type of connection to make 'L' for linking nodes, 'V' for linking nodes to variables
 * @param from
 * variable or node to connect from
 * @param to
 * node to connect to
 */
void lnvars(struct vars *v, char type, int from, int to) {
    if (type == '\n' || type == '\r') return;
    if (type == 'L') {
        if (to) {
            if (v->nodes[from]->to == NULL) {
                v->nodes[from]->to = v->nodes[to];
                v->nodes[from]->to->refs++;
            } else {
                v->nodes[from]->to->refs--;
                v->nodes[from]->to = v->nodes[to];
                v->nodes[from]->to->refs++;
            }
        } else if (v->nodes[from]->to != NULL) {
            v->nodes[from]->to->refs--;
            v->nodes[from]->to = NULL;
        }
    } else if (type == 'V') {
        if (to) {
            if (v->vars[from] == NULL) {
                v->vars[from] = v->nodes[to];
                v->vars[from]->refs++;
            } else {
                v->vars[from]->refs--;
                v->vars[from] = v->nodes[to];
                v->vars[from]->refs++;
            }
        } else if (v->vars[from] != NULL) {
            v->vars[from]->refs--;
            v->vars[from] = NULL;
        }
    } else {
        fprintf(stderr, "ERROR, INVALID TYPE '%c'\n", type);
        free(v);
        exit(-1);
    }
}

/**
 * Properly allocated a struct of variables
 * @param size
 * number of variables to allocate
 * @return
 * allocated vars struct
 */
struct vars* mkvars(int size) {
    struct vars *v = malloc(sizeof(struct vars));
    if (v == NULL) {
        fprintf(stderr, "ERROR, FAILED TO ALLOCATE VARS STRUCT\n");
        exit(-1);
    } else {
        if (!(v->vars = calloc(sizeof(struct node*), 10))) {
            fprintf(stderr, "ERROR, FAILED TO ALLOCATE VARS\n");
            free(v->nodes);
            free(v);
            exit(-1);
        }
        if (!(v->nodes = malloc(sizeof(struct node*) * (size + 1)))) {
            fprintf(stderr, "ERROR, FAILED TO ALLOCATE NODES\n");
            free(v);
            exit(-1);
        } else {
            for (int i = 0; i < (size + 1); i++) {
                if (!(v->nodes[i] = calloc(sizeof(struct node), 1))) {
                    fprintf(stderr, "ERROR, FAILED TO ALLOCATE NODES\n");
                    for (int j = 0; j < i; j++) free(v->nodes[j]);
                    free(v->vars);
                    free(v->nodes);
                    free(v);
                    exit(-1);
                }
            }
        }
        v->numNodes = size;
        return v;
    }
}

/**
 * Properly frees a vars struct
 * @param v
 * vars struct to free
 */
void rmvars(struct vars *v) {
    for (int i = 0; i <= v->numNodes; i++) free(v->nodes[i]);
    free(v->nodes);
    free(v->vars);
    free(v);
}

/**
 * Prints a vars struct in the form
 * "Node # has # incoming pointers"
 * If a variable does not have any incoming pointers it will not be printed
 * @param v
 * vars struct to print
 */
void printvars(struct vars *v) {
    for (int i = 1; i <= v->numNodes; i++) {
        if (v->nodes[i]->refs != 0) {
            printf("Node %d has %d incoming pointers\n", i, v->nodes[i]->refs);
        }
    }
}

/**
 * Main method that reads the number of variables followed by the connections to be made, one per line
 * Input is terminated by an EOF marker which can be placed from stdin with Ctrl-d
 * @return
 * 0 for success
 */
int main() {
    int numNodes;
    scanf("%d", &numNodes);

    struct vars *v = mkvars(numNodes);
    while(!feof(stdin)) {
        char type;
        int from;
        int to;
        scanf("%c %d->%d", &type, &from, &to);
        lnvars(v, type, from, to);
    }

    printvars(v);
    rmvars(v);
    return 0;
}