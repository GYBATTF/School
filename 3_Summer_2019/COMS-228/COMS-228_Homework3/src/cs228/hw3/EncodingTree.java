package cs228.hw3;

import java.util.Stack;

/**
 * Class to store a tree to decode an encoded method
 * @author Alexander Harms
 *
 */
public class EncodingTree {
	public char codedChar;
	public EncodingTree left;
	public EncodingTree right;
	
	/**
	 * Constructor that iteratively builds a tree
	 * @param treeString
	 * string to use to build the tree from
	 */
	public EncodingTree(String treeString) {
		Stack<EncodingTree> nodes = new Stack<>();
		nodes.push(this);
		
		for (int i = 1; treeString != null && i < treeString.length(); i++) {
			char c = treeString.charAt(i);
			EncodingTree et = new EncodingTree((c =='^') ? '\u0000' : c);
			
			if (c == '^') {
				if (nodes.peek().left == null) {
					nodes.peek().left = et;
					nodes.push(et);
				} else if (nodes.peek().right == null) {
					nodes.peek().right = et;
					nodes.push(et);
				} else {
					nodes.pop();
					i--;
				}
			} else {
				if (nodes.peek().left == null) {
					nodes.peek().left = et;
				} else if (nodes.peek().right == null) {
					nodes.peek().right = et;
				} else {
					nodes.pop();
					i--;
				}
			}
		}
	}
	
	/**
	 * Constructor used to just create a node
	 * @param c
	 * character to store in the node, use '\u0000' if this is just a connector
	 */
	private EncodingTree(char c) {
		codedChar = c;
	}
	
	/**
	 * Prints an entire tree using preorder traversal
	 * @param codeParser
	 * tree to print
	 * @param code
	 * current code of character to print, use "" when starting
	 */
	public static void printCodes(EncodingTree codeParser, String code) {
		if (codeParser.codedChar != '\u0000') {
			System.out.println("\t" + codeParser.codedChar + "\t" + code);
		} else {
			printCodes(codeParser.left, code + 0);
			printCodes(codeParser.right, code + 1);
		}
	}
}
