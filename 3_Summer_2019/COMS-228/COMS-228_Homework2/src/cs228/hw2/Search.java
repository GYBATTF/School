package cs228.hw2;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
/**
 * Searches a file for a list of given strings and
 * returns the first index they are found in the file
 * @author Alexander Harms
 *
 */
public class Search {
	/**
	 * Checks if we have the correct parameters required to start the searches,
	 * does the search, and then prints the output
	 * @param args Arguments for a file of strings to search and a file to search in
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		if (args.length == 2) {
			String searchStrings = new String(Files.readAllBytes(Paths.get(args[0])));
			
			HashMap<String, Integer> locationsBruteForce = new HashMap<String, Integer>() {{ for (String line : searchStrings.split("\\R")) {
																								put(line.trim(), -1);
																						  }}};
			HashMap<String, Integer> locationsHashMap = (HashMap<String, Integer>) locationsBruteForce.clone();
			
			String search = new String(Files.readAllBytes(Paths.get(args[1]))).toLowerCase().replaceAll("\\s", " ");

			long bfTime = bruteForceSearch(search, locationsBruteForce, new Date());
			printOutput("Brute Force", locationsBruteForce, bfTime);
			
			long hmTime = hashMapSearch(search, locationsHashMap, new Date());
			printOutput("HashMap Index", locationsHashMap, hmTime);
		} else {
			throw new IllegalArgumentException();
		}
	}

	/**
	 * Searches the text character by character, adding the index (if found) to the table
	 * @param search string to search
	 * @param results HashMap of words to find
	 */
	private static long bruteForceSearch(String search, HashMap<String, Integer> results, Date d) {
		search += ' ';
		// Loop through the string
		for (int i = 0; i < search.length(); i++) {
			// For each character loop through the terms being searched
			for (String key : results.keySet()) {
				String s = key.toLowerCase();
				// If the first character matches the character we are currently on
				if (i + s.length() < search.length() && search.charAt(i) == s.charAt(0) && search.charAt(i + s.length()) == ' ') {
					// Make sure all the other characters in the term match
					int location = i;
					boolean match = true;
					for (int j = 0; j < s.length() && location < search.length(); j++, location++) {
						if (!(match = match && search.charAt(location) == s.charAt(j))) {
							break;
						}
					}
					// If everything is good add the index
					if (match && results.get(key) == -1) {
						results.put(key, i);
					}
				}
			}
		}
		return new Date().getTime() - d.getTime();
	}
	
	/**
	 * Searches a string by putting all the locations of a word in a HashMap
	 * and then looking it up in the HashMap
	 * @param search string to search
	 * @param results terms to search for
	 */
	private static long hashMapSearch(String search, HashMap<String, Integer> results, Date d) {
		HashMap<String, ArrayList<Integer>> searchMap = new HashMap<>();
		int index = 0;
		// Create HashMap of terms to search
		for (String next : search.split(" ")) {
			if (searchMap.get(next) == null) {
				ArrayList<Integer> indexes = new ArrayList<>();
				indexes.add(index);
				searchMap.put(next, indexes);
			} else {
				searchMap.get(next).add(index);
			}
			index += next.length() + 1;
		}
		// Search the HashMap for indexes of terms
		for (String s : results.keySet()) {
			String key = s.toLowerCase();
			// Check if it's a sentence or not
			if (!key.contains(" ")) {
				ArrayList<Integer> indexes = searchMap.get(key);
				if (indexes != null) {
					results.put(s, indexes.get(0));
				}
			} else {
				index = indexOfSentence(key, searchMap);
				if (index != -1) {
					results.put(s, index);
				}
			}
		}
		return new Date().getTime() - d.getTime();
	}
	
	/**
	 * Find the index of a sentence from a HashMap made using a search String
	 * @param term sentence we are searching for
	 * @param searchMap HashMap of text
	 * @return index of the sentence
	 */
	private static int indexOfSentence(String term, HashMap<String, ArrayList<Integer>> searchMap) {
		// Split the term into individual words
		String[] terms = term.split(" ");
		boolean first = true;
		for (int i = 0, index = -1; i < terms.length; i++) {
			// Get the size of the word plus a space
			int size = terms[i].length() + 1;
			// Make sure that the word exists at all in the text
			if (searchMap.get(terms[i]) != null) {
				// Iterate through the indexes of the word
				for (Integer ai : searchMap.get(terms[i])) {
					// If we made it to the last word return the location of the sentence
					if (i == terms.length - 1) {
						return index;
					// If the next term doesn't exist it none of the sentence will match
					} else if (searchMap.get(terms[i + 1]) == null) {
						return -1;
					// Check if the index is found	
					} if (searchMap.get(terms[i + 1]).contains(ai + size)) {
						if (first) {
							index = ai;
							first = false;
						}
						// If we found an index that matches there's no point in checking the rest
						break;
					}
				} 
			// If a single word in the sentence doesn't exist in the text
			// The entire sentence doesn't exist in the text, so return -1
			} else {
				return -1;
			}
		}
		// If nothing has been returned by this point return -1
		return -1;
	}
	
	/**
	 * Formats and prints the results and timings from the specified search
	 * @param method method used to search
	 * @param hm HashMap of indexes from the search
	 * @param timeTaken how long that search took
	 */
	private static void printOutput(String method, HashMap<String, Integer> hm, long timeTaken) {
		// Heading
		System.out.println("Search method: " + method + "\r\nIndex       Search String\r\n--------------------------");
		for (String s : hm.keySet()) {
			// Check if the term was found and print the proper output
			System.out.println(hm.get(s) == -1 ? String.format("--          %s", s) : String.format("%-12d%s", hm.get(s), s));
		}
		// Footing
		System.out.printf("Time taken: %d ticks\r\n\r\n", timeTaken);
	}
}
