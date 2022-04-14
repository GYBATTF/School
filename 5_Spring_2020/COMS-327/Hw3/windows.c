/**
 * @author Alexander Harms
 * Program to read in coordinates for an area and windows to put in it
 */
#include<stdio.h>

/*
 * window struct to hold information about a window  
 */ 
struct window {
    // Box coordinates
    unsigned xlow, ylow, xhigh, yhigh;
    // Characters to fill with
    char wch;
};

/**
 * "You may safely assume that the input file will not contain more than 100 windows,
 * and the display area will not be wider than 100 characters"
 */
const int SAFE_ASSUMPTION = 100;

/**
 * Gets windows to fill into the area
 * @param W
 * array to fill with windows
 * @param wmax
 * size of W
 * @return
 * number of windows entered
 */
unsigned fill_windows(struct window W[], unsigned wmax) {
    // Get the number of windows to be entered
    unsigned numw;
    scanf("%d", &numw);

    // Get the specified number of windows
    for (int i = 0; i < numw; i++) {
        scanf("%d,%d %d,%d %c", &W[i].xlow, &W[i].ylow, &W[i].xhigh, &W[i].yhigh, &W[i].wch);
    }

    // Return the number of windows entered
    return numw;
}

/**
 * Fills a line with windows
 * @param W
 * array of windows to get fill data from
 * @param Wsize
 * how many windows we have stored in the array
 * @param x1
 * x position of the start of the line we are filling
 * @param x2
 * x position of the end of the line we are filling
 * @param y
 * y position of the line we are filling
 * @param line
 * line array the fill with window characters
 */
void fill_line(struct window W[], unsigned Wsize, unsigned x1, unsigned x2, unsigned y, char line[]) {
    // Initialize the area we are printing with empty spaces
    for (int i = x1; i <= x2; i++) line[i] = ' ';

    // Go through each window
    for (int i = 0; i < Wsize; i++) {

        // Make sure the window is in the line we are printing
        if (W[i].yhigh >= y && W[i].ylow <= y) {

            // Not sure if needed, but rather wouldn't risk going outside the array
            int j = W[i].xlow - x1 < 0 ? 0 : W[i].xlow - x1;
            
            // Fill with charaters to either the end of the window or the end of the line
            for (;j <= x2 && j <= W[i].xhigh; j++) line[j] = W[i].wch;
        }
    }
}

/**
 * Main function
 * Gets the size of the output
 * Then gets the windows to be printed
 * Sets up a line to be printed
 * Prints the line
 * Repeats until all lines are printed
 */
void main() {
    // Get the window size
    unsigned dxlow, dylow, dxhigh, dyhigh;
    scanf("%d,%d %d,%d", &dxlow, &dylow, &dxhigh, &dyhigh);

    // Don't go past the end of the line array
    dxhigh = dxhigh > SAFE_ASSUMPTION ? SAFE_ASSUMPTION : dxhigh;

    // Prepare needed arrays
    struct window W[SAFE_ASSUMPTION];
    char line[SAFE_ASSUMPTION];

    // Get the windows and how many
    unsigned size = fill_windows(W, SAFE_ASSUMPTION);
    
    // Loop though each y value
    for (int y = dylow; y <= dyhigh; y++) {

        // Fill the line with window content
        fill_line(W, size, dxlow, dxhigh, y, line);

        // Print the line
        for (int i = 0; i <= dxhigh - dxlow; printf("%c", line[i]), i++);

        // It's the end of the line!
        printf("\n");
    }
}
