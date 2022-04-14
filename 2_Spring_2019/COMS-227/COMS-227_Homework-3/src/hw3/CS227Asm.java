package hw3;

import java.util.ArrayList;
import java.util.Scanner;

import api.Instruction;
import api.NVPair;
import api.SymbolTable;

/**
 * Assembler for assembly language programs for the COMS227 computer
 * @author Alexander Harms
 *
 */
public class CS227Asm {
	
	/**
	 * sentinel value
	 */
	private static String SENTINEL = "-99999";
	
	/**
	 *  Stores the program given in the constructor
	 */
	private ArrayList<String> program;
	
	/**
	 *  Stores NVPairs of data
	 */
	private SymbolTable data;
	
	/**
	 *  Stores NVPairs of labels
	 */
	private SymbolTable labels;
	
	/**
	 *  Stores a table of instructions
	 */
	private ArrayList<Instruction> instructions;

	/**
	 * Creates the assembler for the CS227 computer from the ArrayList of strings given. 
	 * Initializes all the arrays.
	 * @param program
	 * program given as an ArrayList of strings
	 */
	public CS227Asm(ArrayList<String> program) {
		this.program = program;
		data = new SymbolTable();
		labels = new SymbolTable();
		instructions = new ArrayList<>();
	}
	
	/**
	 * Labels the jump points in the instruction stream
	 */
	public void addLabelAnnotations() {
		for (int i = 0; i < labels.size(); i++) {
			// Write the description to the instruction stream
			instructions.get(labels.getByIndex(i).getValue()).addLabelToDescription(labels.getByIndex(i).getName());
		}
	}
	
	/**
	 * Assembles the code loaded into this class
	 * @return
	 * the assembled class
	 */
	public ArrayList<String> assemble() {
		// Run all the individual assembly routines
		parseData();
		parseLabels();
		parseInstructions();
		setOperandValues();
		addLabelAnnotations();
		
		// Compiles the code into a single array
		return writeCode();
	}
	
	/**
	 * Returns a SymbolTable of data with the variable name and its value
	 * @return
	 * a SymbolTable of data variables
	 */
	public SymbolTable getData() {
		return data;
	}
	
	/**
	 * Returns an ArrayList of the instructions and operands
	 * @return
	 * an ArrayList of instructions and operands
	 */
	public ArrayList<Instruction> getInstructionStream() {
		return instructions;
	}
	
	/**
	 * Returns a SymbolTable of labels and its location
	 * @return
	 * a SymbolTable of labels
	 */
	public SymbolTable getLabels() {
		return labels;
	}
	
	/**
	 * Parses the data section of the provided program and adds it to the data SymbolTable
	 */
	public void parseData() {
		// Iterates through just the part of the program containing data items and adds them to the array
		for (int i = program.indexOf("data:") + 1; !("labels:".equals(program.get(i))); i++) {
			Scanner s = new Scanner(program.get(i));
			data.add(s.next(), s.nextInt());
		}
	}
	
	/**
	 * Parses the instruction section of the program and adds it to a memory file.
	 * Also adds jump locations to the labels SymbolTable.
	 */
	public void parseInstructions() {
		// Iterates through just the part of the program containing instruction items and adds them to the array
		for (int i = program.indexOf("instructions:") + 1, ic = 0; i < program.size(); i++, ic++) {
			String ins = program.get(i).trim();
			
			// Fixes issues if a jump point has a comment causing it not to be counted as a jump point
			Scanner s = new Scanner(ins);
			String jumpOnly = s.next();
			
			// Checks if its a jump point
			if (labels.containsName(jumpOnly)) {
				labels.findByName(jumpOnly).setValue(ic--);
			// Otherwise adds the instruction the instruction array
			} else {
				instructions.add(new Instruction(ins));
			}
		}
	}
	
	/**
	 * Parses the label section of the program and adds it to a SymbolTable
	 */
	public void parseLabels() {
		// Iterates through just the part of the program containing label items and adds them to the array
		for (int i = program.indexOf("labels:") + 1; !("instructions:".equals(program.get(i))); i++) {
			labels.add(program.get(i));
		}
	}
	
	/**
	 * Sets the operands and sets data locations
	 */
	public void setOperandValues() {
		// Iterates through the instructions
		for (Instruction ins : instructions) {
			// If it requires a data address writes that address
			if (ins.requiresDataAddress()) {
				ins.setOperand(data.indexOf(data.findByName(ins.getOperandString())) + instructions.size());
			}
			// If it requires a jump target writes the target
			if (ins.requiresJumpTarget()) {
				ins.setOperand(labels.findByName(ins.getOperandString()).getValue());
			}
		}
	}
	
	/**
	 * Assembles the code and write the sentinel value to it
	 * @return
	 * the assembled code
	 */
	public ArrayList<String> writeCode() {
		ArrayList<String> assembled = new ArrayList<>();
		
		// Adds the instructions to the finished array
		for (Instruction ins : instructions) {
			assembled.add(ins.toString());
		}
		
		// Adds the data to the finished array
		for (int i = 0; i < data.size(); i++) {
			NVPair tmp = data.getByIndex(i);
			assembled.add(String.format("%c%04d %s", '+', tmp.getValue(), tmp.getName()));
		}
		
		// Appends the sentinel value
		assembled.add(SENTINEL);
		
		// Returns the finished code
		return assembled;
	}
}
