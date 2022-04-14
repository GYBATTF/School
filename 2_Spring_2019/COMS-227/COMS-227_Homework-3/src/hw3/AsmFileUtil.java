package hw3;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * Utilities for reading and compiling files with programs for the CS227Comp
 * @author Alexander Harms
 *
 */
public class AsmFileUtil {
	
	/**
	 * File extension of uncompiled program
	 */
	private static final String PRECOMPILED_FILE_EXT = ".asm227";
	
	/**
	 * File extension of compiled program
	 */
	private static final String COMPILED_FILE_EXT = ".mach227";
	
	/**
	 * ArrayList for the compile program
	 */
	private static ArrayList<String> compiled;

	/**
	 * Constructor, doesn't need to do anything
	 */
	public AsmFileUtil() {}
	
	/**
	 * Compiles a given file and writes it a file with the same name but with the extension .mach227
	 * @param filename
	 * file to read
	 * @param annotated
	 * if it needs to be annotated or not
	 * @throws FileNotFoundException
	 */
	public static void assembleAndWriteFile(String filename, boolean annotated) throws FileNotFoundException {
		// Reads and compiles the program
		compiled = readAndCompile(filename, annotated, true);
		filename = filename.substring(0, filename.indexOf(PRECOMPILED_FILE_EXT)) + COMPILED_FILE_EXT;
		PrintWriter out = new PrintWriter(filename);
		
		// Writes the program to file
		for (String line : compiled) {
				out.println(line);
		}
		
		out.close();
	}
	
	/**
	 * Assembles a program and returns it
	 * @param filename
	 * file to compile
	 * @return
	 * compiled file
	 * @throws FileNotFoundException
	 */
	public static ArrayList<String> assembleFromFile(String filename) throws FileNotFoundException {
		// Compile the program and return it
		return readAndCompile(filename, true, false);
	}
	
	/**
	 * Compiles a file and returns a memory image for the COMS227 Computer
	 * @param filename
	 * file to compile
	 * @return
	 * memory image for CS227Comp
	 * @throws FileNotFoundException
	 */
	public static int[] createMemoryImageFromFile(String filename) throws FileNotFoundException {
		// Read and compile
		compiled = readAndCompile(filename, false, false);
		int[] finished = new int [compiled.size()];
		
		// Iterate through the ArrayList and into an array
		for (int i = 0; i < compiled.size(); i++) {
			finished[i] = Integer.parseInt(compiled.get(i));
		}
		
		// Return the array
		return finished;
	}
	
	/**
	 * Helper to read and compile a file. All three function use the same thing so this does everything.
	 * @param filename
	 * file to compile
	 * @param annotated
	 * whether annotations are wanted or not
	 * @param sentinal
	 * whether or not to append the sentinal
	 * @return
	 * a compile program
	 * @throws FileNotFoundException
	 */
	private static ArrayList<String> readAndCompile(String filename, boolean annotated, boolean sentinal) throws FileNotFoundException {
		Scanner s = new Scanner(new File(filename));
		ArrayList<String> prgm = new ArrayList<>();
		
		// Read the file, add it to the compiler, then close the file
		while (s.hasNext()) {
			prgm.add(s.nextLine());
		}
		
		CS227Asm asm = new CS227Asm(prgm);
		s.close();
		
		// Assemble it
		compiled = asm.assemble();
		
		// See if it need annotations or not and if not removes them
		if (!annotated)  {
			for (int i = 0; i < compiled.size(); i++) {
				s = new Scanner(compiled.get(i));
				compiled.set(i, s.next());
			}
		}
		
		if (!sentinal) {
			// Remove the sentinal
			compiled.remove(compiled.size() - 1);
		}
		
		// Return it
		return compiled;
	}
}
