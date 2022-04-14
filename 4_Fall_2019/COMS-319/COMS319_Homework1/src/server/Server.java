package server;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Scanner;
/**
 * Server to send and recieve messages from multiple chat clients with
 * @author Alexander Harms
 *
 */
public class Server {
	/**
	 * The access code to access the server
	 */
	static final String ACCESS_CODE = "cs319";
	/**
	 * Port to talk on
	 */
	public static final int SERVER_PORT = 9090;
	/**
	 * list of clients 
	 */
	static ArrayList<Client> clients = new ArrayList<>();

	/**
	 * waits for a client connection and then adds it to the list and starts a thread for it
	 * @param args
	 * unused
	 */
	public static void main(String[] args) throws Exception {
		ServerSocket serverSocket = new ServerSocket(SERVER_PORT);
		
		int id = 0;
		while (true) {
			Socket socket = serverSocket.accept();
			Client client = new Client(socket, id++);
			Thread thread = new Thread(client);
			thread.start();
			clients.add(client);
		}
	}
}
/**
 * Object to store clients and their connections in
 * @author Alexander Harms
 *
 */
class Client implements Runnable {
	/**
	 * socket to talk with
	 */
	private Socket socket;
	/**
	 * username of the client
	 */
	private String username;
	/**
	 * id of the client
	 */
	private int id;
	/**
	 * PrintWriter to send messages with
	 */
	private PrintWriter out;
	
	/**
	 * Creates a new client with the specified id and socket connection
	 * @param socket
	 * socket to talk with
	 * @param id
	 * id of the client
	 */
	Client(Socket socket, int id) {
		this.socket = socket;
		this.id = id;
		out = null;
		try {
			out = new PrintWriter(new BufferedOutputStream(socket.getOutputStream()));
		} catch (IOException e) { }
	}
	
	/**
	 * Gets the username for the client and then enters an infinite loop waiting for new messages
	 * when a new message comes in it is then sent to all other clients
	 */
	public void run() {
		Scanner in = new Scanner("");
		try {
			in = new Scanner(new BufferedInputStream(socket.getInputStream()));
			username = in.nextLine();

			String accessCode = "";
			do {
				sendMessage(">Enter access code: ");
				accessCode = in.next();
				
				if (!accessCode.equals(Server.ACCESS_CODE)) {
					sendMessage("incorrect access code");
				}
				
			} while (!accessCode.equals(Server.ACCESS_CODE));
		} catch (IOException e) { }
		
		while (true) {
			String message = in.nextLine();
			if (!message.equals("")) {
				message = username + ">> " + message;
				System.out.println(message);
				for (Client ci : Server.clients) {
					if (ci.getId() != this.id) {
						ci.sendMessage(message);
					}
				}
			}
		}
	}
	
	/**
	 * Sends a message to the client
	 * @param m
	 * the message to send
	 */
	public void sendMessage(String m) {
		out.println(m);
		out.flush();
	}
	
	/**
	 * Returns the ID of this client
	 * @return
	 * the id of the client
	 */
	public int getId() {
		return id;
	}
}
