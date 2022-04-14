package client;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

import server.Server;

/**
 * 
 * @author Alexander Harms
 *
 */
public class Client {
	/**
	 * Address of the server
	 */
	private static final String SERVER_ADDRESS = "localhost";
	/**
	 * Port the server is located at
	 */
	private static final int SERVER_PORT = 9090;
	
	/**
	 * Prompts for a username and the access code and then creates a connection to
	 * the chat server to send messages that are entered 
	 * @param args
	 * unused
	 */
	public static void main(String[] args) throws Exception {
		Scanner in = new Scanner(System.in);
		String username = "";
		
		System.out.print(">Enter your name: ");
		username = in.next();
		
		Socket socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
		PrintWriter out = new PrintWriter(new BufferedOutputStream(socket.getOutputStream()));
		out.println(username);
		out.flush();

		Scanner socketIn = new Scanner("");
		try {
			socketIn = new Scanner(new BufferedInputStream(socket.getInputStream()));
		} catch (IOException e) { }
		
		new Thread(new Out(socketIn)).start();
		
		while (true) {
			if (in.hasNextLine()) {
				out.println(in.nextLine());
				out.flush();
			}
		}
	}
}

/**
 * Class to print output received from the chat server
 * @author Alexander Harms
 *
 */
class Out implements Runnable {
	/**
	 * Scanner to read messages from
	 */
	private Scanner in;
	
	/**
	 * Creates a new object that prints output from a socket connection to a chat server
	 * @param s
	 * scanner to get messages from
	 */
	Out(Scanner s) {
		in = s;
	}
	
	/**
	 * Gets an input stream from the socket and then enters an infinite loop printing 
	 * whatever messages come out of it
	 */
	@Override
	public void run() {
		while (true) {
			System.out.println(in.nextLine());
		}
	}
}