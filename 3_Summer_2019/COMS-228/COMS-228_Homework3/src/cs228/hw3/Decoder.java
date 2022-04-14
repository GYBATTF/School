package cs228.hw3;

import java.io.File;
import java.util.Scanner;

/**
 * Class containing main() method for decoding assignment
 * @author Alexander Harms
 * 
 */
public class Decoder {

	public static void main(String[] args) {
		
		// Strings for processing the input
		String treeString = null;
		String candidateString = null; //will store the 2nd line of the file, which could be either an encoding tree(in 3-line file) or an encoded message (in 2-line file)
		String encodedMessage = null; //should be 0s and 1s only

		//load a string with the encoding tree and another string with the encoded message
		try {
			Scanner scan = new Scanner(new File("constitution.msg"));
			treeString = scan.nextLine();
			candidateString = scan.nextLine(); //2nd line is either last line of encoding tree or the message line
			if (scan.hasNextLine()){  //if there is another line, the 2nd line is part of the tree should be added to the tree
				treeString = treeString+"\n"+candidateString;
				encodedMessage = scan.nextLine(); //read 3rd line into message
			}
			else {  //if only 2 lines in the file, the 2nd line is the encoded message
				encodedMessage = candidateString;
			}
			//System.out.println(treeString);
			scan.close();
			} catch (Exception ex) {
				System.out.println(ex.getMessage());
			}	
		
		//recursively build the parsing tree from the string
		EncodingTree codeParser = new EncodingTree(treeString);
		//non-recursively build the parsing tree from the string
		//EncodingTree codeParser = new EncodingTree(treeString); 
		System.out.println("character \tcode");
		System.out.println("-----------------------------");
		EncodingTree.printCodes(codeParser, "");
		System.out.println("MESSAGE");
		decode(codeParser, encodedMessage);
		
		
	}
	
	/**
	 * Decodes the string into human-readable form.
	 * @param codeRoot
	 * code tree to use to decode
	 * @param msg
	 * message to print
	 */
	public static void decode(EncodingTree codeRoot, String msg) {
		EncodingTree current = codeRoot;
		
		for (int i = 0; i < msg.length(); i++) {
			char c = msg.charAt(i);
			if (current.codedChar != '\u0000') {
				System.out.print(current.codedChar);
				current = codeRoot;
				i--;
			} else {
				if (c == '0') {
					current = current.left;
				} else {
					current = current.right;
				}
			}
		}
		
		System.out.print(current.codedChar);
	}
}