package mini1;

import java.util.Random;
import java.util.Scanner;

public class FromLoopToNuts {

	private FromLoopToNuts() {}
	
	// Eliminate repeated characters in a row
	public static String eliminateRuns(String s) {
		// If its an empty string return an empty string
		if (s.equals("")) {
			return "";
		}
		
		// Initialize variables and set them up with the starting characters
		String runLess = "";
		runLess += s.charAt(0);
		char previous = s.charAt(0);
		char current = s.charAt(1);
		
		// Run through the string checking if its a repeat and if not adding it to the finished string
		for (int iter = 2; iter < s.length(); iter++) {
			if (previous != current) {
				runLess += current;
			}
			previous = current;
			current = s.charAt(iter);
		}
		
		// Make sure theres nothing left
		if (previous != current) {
			runLess += current;
		}
		
		// Return the finished string
		return runLess;
	}
	
	// Checks to see if two string differ by exactly one swap
	public static boolean differByOneSwap(String s, String t) {
		// Initialize values
		boolean oneSwap = false;
		int numSwaps = 0;
		String sub = "";
		String rsub = "";
		
		// Check to see if the strings are the same length
		if (s.length() == t.length()) {
			// Iterate through the string
			for (int offset = 0; offset + 1 < s.length(); offset++) {
				// Grab two letters at a time
				sub = s.substring(offset, offset + 2);
				// Reverse them
				for (int i = 1; i >= 0; i--) {
					rsub += sub.charAt(i);
				}
				
				// Check if the reversed pair matches the same pair from the target
				if (rsub.equals(t.substring(offset, offset + 2))) {
					numSwaps++;
				}
				
				// If they're the same both ways don't count it
				if (sub.equals(rsub)) {
					numSwaps--;
				}
				
				rsub = "";
				
			}
		}
		
		// Checks to see if there's exactly one swap
		if (numSwaps == 1) {
			oneSwap = true;
		}
		
		// Returns if there's a swap or not
		return oneSwap;
	}
	
	// Generates random numbers and sees how long until you get three in a row
	public static int threeInARow(Random rand, int bound) {
		// Initialize values
		int runs = 3;
		int first = rand.nextInt(bound);
		int second = rand.nextInt(bound);
		int third = rand.nextInt(bound);
		
		// Starts generating random numbers and counting iterations
		while (!((first == second) && (second == third))) {
			first = second;
			second = third;
			third = rand.nextInt(bound);
			runs++;
		}
		
		// Returns number of iterations to three in a row
		return runs;
	}
	
	public static int findEscapeCount(double x, double y, int maxIterations) {
		int iters = 0;
		double a = 0;
		double b = 0;
		double tempA;
		double tempB;
		
		while ((iters <= maxIterations) && ((a * a + b * b) < 4)) {
			tempA = a * a - b * b + x;
			tempB = 2.0 * a * b + y;
			a = tempA;
			b = tempB;
			iters++;
			if (iters > maxIterations) {
				return maxIterations;
			}
		}
		
		return iters;
	}
	
	public static int countMatches(String s, String t) {
		if (s.equals("") || t.equals("")) {
			return 0;
		}
		int matches = 0;
		int bigger = 0;
		
		if (s.length() == t.length()) {
			bigger = s.length() - 1;
		} else if (s.length() > t.length()){
			bigger = t.length() - 1;
		} else if (s.length() < t.length()) {
			bigger = s.length() - 1;
		}
		
		for (int offset = 0; offset <= bigger; offset++) {
			if (Character.toString(s.charAt(offset)).equals(Character.toString(t.charAt(offset)))) {
				matches++;
			}
		}
		
		return matches;
	}
	
	public static boolean isArithmetic(String text) {
		if ((text.equals(""))) {
			return true;
		}
		
		Scanner num = new Scanner(text);
		num.useDelimiter(",");
		
		int items = 1;
		int arith = 1;
		int previous = 0;
		int current = 0;
		int diff = 0;
	
		if (num.hasNextInt()) {
			previous = num.nextInt();
			if (!(num.hasNext())) {
				return true;
			}
			current = num.nextInt();
			diff = current - previous;
		}
		
		while (num.hasNextInt()) {
			items++;
			previous = current;
			current = num.nextInt();
			if (current - previous == diff) {
				arith++;
			}
		}
		
		if (num.hasNext()) {
			return false;
		}
		
		if (items <= 2 || items == arith) {
			return true;
		} else {
			return false;
		}
	}
	
	public static int countSubstrings(String t, String s) {
		if (s.equals("") || t.equals("")) {
			return 0;
		}
		int substrings = 0;
		int offset = t.length();
		int start = 0;
		while (offset <= s.length()) {
			if (t.equals(s.substring(start, offset))) {
				substrings++;
				start = offset - 1;
				offset += t.length() - 1;
			}
			offset++;
			start++;
		}
		return substrings;
	}
	
	public static int countSubstringsWithOverlap(String t, String s) {
		if (s.equals("") || t.equals("")) {
			return 0;
		}
		int substrings = 0;
		int offset = t.length();
		int start = 0;
		while (offset <= s.length()) {
			if (t.equals(s.substring(start, offset))) {
				substrings++;
			}
			offset++;
			start++;
		}
		return substrings;
	}
}
