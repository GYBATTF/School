package mini3;

import java.util.Arrays;

/**
 * Method for returning sandwich combinations in a recursive way
 * @author Alexander Harms
 *
 */
public class Combinations {
	/**
	 * Recursively calculates combinations of sandwich ingredients
	 * @param choices
	 *  one dimensional integer array of choices for ingredients.
	 * @return
	 * two dimensional integer array of sandwich combinations
	 */
	public static int[][] getCombinations(int[] choices) {
		// Calculate the size of the array from amount of options
		int size = 1;
		for (int i : choices) {
			size *= i;
		}
		// Base case, if there's one choice construct an array of that size and return it
		// Populates it with the number of options incrementally
		if (choices.length == 1) {
			int[][] oneChoice = new int[size][1];
			for (int i = 0; i < size; i++) {
				oneChoice[i][0] = i;
			}
			return oneChoice;
			
		// If there's more than one choice break it down and then build it up again
		}
		// Arrays that we are returning and the array that we are merging into it
		int[][] allChoices = new int[size][choices.length];
		int[][] toMerge = getCombinations(Arrays.copyOfRange(choices, 1, choices.length));
		
		// Populate the first item in the 2D array
		for (int i = 0, j = 0, k = 0; i < size; i++, k++) {
			// Check if j needs incrementing and k needs reset
			if ((i % (size / choices[0])) == 0 && i != 0) {
				j++;
				k = 0;
			}
			// Assign the top option to the array
			allChoices[i][0] = j;
			// Populate with the array from the recursion
			for (int h = 0; h < toMerge[0].length; h++) {
				allChoices[i][h + 1] = toMerge[k][h];
			}
		}
		// Sort the array and then return it
		Arrays.sort(allChoices, new ArrayComparator());
		return allChoices;
	}
}
