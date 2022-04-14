package application;
	
import java.awt.Point;
import java.io.FileInputStream;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.shape.Line;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.stage.Stage;

/**
 * Draws a GUI for playing games of tic-tac-toe
 * @author Alexander Harms
 *
 */
public class Main extends Application {
	private static final String GAME_NAME = "Tic-Tac-Toe";
	
	// Variables for the lines of the game
	private static final int LINE_WIDTH = 5;
	
	// Window size variables
	private static final int WINDOW_WIDTH = LINE_WIDTH * 122;
	private static final int WINDOW_HEIGHT;
	
	
	private static final Line TOP_LINE; 
	private static final Line BOTTOM_LINE;
	private static final Line RIGHT_LINE;
	private static final Line LEFT_LINE;
	
	
	// Locations of the images for the Xs and Os
	private static final Point TOP_LEFT;
	private static final Point TOP_MIDDLE;
	private static final Point TOP_RIGHT;
	private static final Point LEFT;
	private static final Point MIDDLE;
	private static final Point RIGHT;
	private static final Point BOTTOM_LEFT;
	private static final Point BOTTOM_MIDDLE;
	private static final Point BOTTOM_RIGHT;
	
	// Image variables
	private static final int IMAGE_SIZE;
	private static final int SMALL_IMAGE_SIZE;
	private static final String X_FILE = "x.jpg";
	private static final String O_FILE = "o.jpg";
	private static Image x;
	private static Image o;
	private static ImageView small_x;
	private static ImageView small_o;
	
	// Game logic
	private static final GameBoard GAMEBOARD = new GameBoard();
	
	// Text variables
	private static final int FONT_SIZE = 24;
	private static final Text CURRENT_TURN_TEXT = new Text("Current Player: ");
	private static final Text WINNER_TEXT_1 = new Text("Congratulation, ");
	private static final Text WINNER_TEXT_2 = new Text(" wins the game. Click here to restart the game.");
	private static final Text DRAW_TEXT = new Text("Draw. Click here to restart the game.");
	
	// Where everything is drawn on
	private static BorderPane root;
	
	// Setup all the variables
	static {
		root = new BorderPane();
		
		CURRENT_TURN_TEXT.setFont(new Font(FONT_SIZE));
		WINNER_TEXT_1.setFont(new Font(FONT_SIZE / 1.75));
		WINNER_TEXT_2.setFont(new Font(FONT_SIZE / 1.75));
		DRAW_TEXT.setFont(new Font(FONT_SIZE));
		
		SMALL_IMAGE_SIZE = (int) (CURRENT_TURN_TEXT.getLayoutBounds().getHeight() + DRAW_TEXT.getLayoutBounds().getHeight());
		WINDOW_HEIGHT = WINDOW_WIDTH + SMALL_IMAGE_SIZE;
		
		int oneThirdWindow = (WINDOW_WIDTH - (LINE_WIDTH * 2)) / 3;
		
		int leftLineStart = oneThirdWindow;
		int rightLineStart = oneThirdWindow + leftLineStart + LINE_WIDTH;
		int topLineStart = leftLineStart;
		int bottomLineStart = rightLineStart;
		
		TOP_LINE = new Line(0, topLineStart + (LINE_WIDTH / 2), WINDOW_WIDTH, topLineStart + (LINE_WIDTH / 2));
		BOTTOM_LINE = new Line(0, bottomLineStart + (LINE_WIDTH / 2), WINDOW_WIDTH, bottomLineStart + (LINE_WIDTH / 2));
		RIGHT_LINE = new Line(rightLineStart + (LINE_WIDTH / 2), 0, rightLineStart + (LINE_WIDTH / 2), WINDOW_WIDTH);
		LEFT_LINE = new Line(leftLineStart + (LINE_WIDTH / 2), 0, leftLineStart + (LINE_WIDTH / 2), WINDOW_WIDTH);
		TOP_LINE.setStrokeWidth(LINE_WIDTH);
		BOTTOM_LINE.setStrokeWidth(LINE_WIDTH);
		RIGHT_LINE.setStrokeWidth(LINE_WIDTH);
		LEFT_LINE.setStrokeWidth(LINE_WIDTH);
		
		double textY = RIGHT_LINE.getEndY() + CURRENT_TURN_TEXT.getLayoutBounds().getHeight();
		CURRENT_TURN_TEXT.setY(textY);
		WINNER_TEXT_1.setY(textY);
		WINNER_TEXT_2.setY(textY);
		DRAW_TEXT.setY(textY);
		CURRENT_TURN_TEXT.setX((WINDOW_WIDTH / 2) - (CURRENT_TURN_TEXT.getLayoutBounds().getWidth() / 2) - SMALL_IMAGE_SIZE);
		DRAW_TEXT.setX((WINDOW_WIDTH / 2) - (DRAW_TEXT.getLayoutBounds().getWidth() / 2));
		WINNER_TEXT_1.setX((WINDOW_WIDTH / 2) - (WINNER_TEXT_1.getLayoutBounds().getWidth() / 2) - SMALL_IMAGE_SIZE - WINNER_TEXT_1.getLayoutBounds().getWidth());
		WINNER_TEXT_2.setX(WINNER_TEXT_1.getX() + WINNER_TEXT_1.getLayoutBounds().getWidth() + SMALL_IMAGE_SIZE);
		
		TOP_LEFT = new Point(0, 0);
		TOP_MIDDLE = new Point(leftLineStart + LINE_WIDTH, 0);
		TOP_RIGHT = new Point(rightLineStart + LINE_WIDTH, 0);
		LEFT = new Point(0, topLineStart + LINE_WIDTH);
		MIDDLE = new Point(topLineStart + LINE_WIDTH, leftLineStart + LINE_WIDTH);
		RIGHT = new Point(rightLineStart + LINE_WIDTH, topLineStart + LINE_WIDTH);
		BOTTOM_LEFT = new Point(0, bottomLineStart + LINE_WIDTH);
		BOTTOM_MIDDLE = new Point(leftLineStart + LINE_WIDTH, bottomLineStart + LINE_WIDTH);
		BOTTOM_RIGHT = new Point(bottomLineStart + LINE_WIDTH, rightLineStart + LINE_WIDTH);
		
		IMAGE_SIZE = oneThirdWindow;
	}
	
	/**
	 * Initializes image files and starts the game
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		x = new Image(new FileInputStream(X_FILE));
		o = new Image(new FileInputStream(O_FILE));
		small_x = new ImageView(new Image(new FileInputStream(X_FILE)));
		small_o = new ImageView(new Image(new FileInputStream(O_FILE)));
		small_x.setFitHeight(SMALL_IMAGE_SIZE);
		small_x.setFitWidth(SMALL_IMAGE_SIZE);
		small_o.setFitHeight(SMALL_IMAGE_SIZE);
		small_o.setFitWidth(SMALL_IMAGE_SIZE);
		
		launch(args);
	}
	
	/**
	 * Sets up the window and handles clicks on the window
	 */
	@Override
	public void start(Stage primaryStage) throws Exception {
		reset();
		Scene scene = new Scene(root, WINDOW_WIDTH, WINDOW_HEIGHT);
		
		primaryStage.setTitle(GAME_NAME);
		primaryStage.setScene(scene);
		primaryStage.setResizable(false);
		
		scene.setOnMouseClicked(new EventHandler<MouseEvent>() {
			@Override
			public void handle(MouseEvent arg0) {
				clickHandler(arg0);
			}
		});
		
		primaryStage.show();
	}
	
	/**
	 * Handles clicking parts the window to 
	 * @param me
	 * MouseEvent to get x and y of where was clicked
	 */
	private static void clickHandler(MouseEvent me) {
		Positions loc = getLocationClicked(me.getX(), me.getY());
		if (GAMEBOARD.gameOver() && loc == null) {
			reset();
		} else if (!(GAMEBOARD.gameOver() || loc == null)) {
			Players winner = GAMEBOARD.markPlay(loc);
			ImageView n = winner != Players.INVALID ? getImageView(loc) : null;
			if (winner == null && winner != Players.INVALID) {
				flipPlayer();
			} else if (!(winner == null || winner == Players.INVALID)) {
				displayWinner();
			}
		}
	}
	
	/**
	 * Displays the winner on the screen
	 */
	private static void displayWinner() {
		root.getChildren().remove(CURRENT_TURN_TEXT);
		root.getChildren().remove(GAMEBOARD.getCurrentTurn() == Players.X ? small_o : small_x);
		(GAMEBOARD.getWinner() == Players.X ? small_x : small_o).setX(WINNER_TEXT_1.getLayoutBounds().getWidth() + WINNER_TEXT_1.getX());
		
		if (GAMEBOARD.getWinner() == Players.NO_ONE) {
			root.getChildren().add(DRAW_TEXT);
		} else {
			root.getChildren().add(WINNER_TEXT_1);
			root.getChildren().add(WINNER_TEXT_2);
			root.getChildren().add(GAMEBOARD.getWinner() == Players.X ? small_x : small_o);
		}
	}
	
	/**
	 * Flips the current player
	 */
	private static void flipPlayer() {
		root.getChildren().remove(GAMEBOARD.getCurrentTurn() == Players.X ? small_o : small_x);
		root.getChildren().add(GAMEBOARD.getCurrentTurn() == Players.X ? small_x : small_o);	
	}
	
	/**
	 * Resets the game
	 */
	private static void reset() {
		root.getChildren().clear();

		small_x.setX(CURRENT_TURN_TEXT.getLayoutBounds().getWidth() + CURRENT_TURN_TEXT.getX());
		small_x.setY(CURRENT_TURN_TEXT.getY() - CURRENT_TURN_TEXT.getLayoutBounds().getHeight());
		small_o.setX(CURRENT_TURN_TEXT.getLayoutBounds().getWidth() + CURRENT_TURN_TEXT.getX());
		small_o.setY(CURRENT_TURN_TEXT.getY() - CURRENT_TURN_TEXT.getLayoutBounds().getHeight());
		
		root.getChildren().add(CURRENT_TURN_TEXT);
		root.getChildren().add(small_x);
		
		root.getChildren().add(TOP_LINE);
		root.getChildren().add(BOTTOM_LINE);
		root.getChildren().add(RIGHT_LINE);
		root.getChildren().add(LEFT_LINE);
		
		GAMEBOARD.newGame();
	}
	
	/**
	 * Checks to see where on the window was clicked
	 * @param x
	 * x position in the window
	 * @param y
	 * y position in the window
	 * @return
	 * place on the gameboard clicked
	 */
	private static Positions getLocationClicked(double x, double y) {
		if (inBounds(x, y, TOP_LEFT.getX(), MIDDLE.getX(), TOP_LEFT.getY(), MIDDLE.getY())) {
			return Positions.TOP_LEFT;
		} else if (inBounds(x, y, TOP_MIDDLE.getX(), RIGHT.getX(), TOP_MIDDLE.getY(), RIGHT.getY())) {
			return Positions.TOP_MIDDLE;
		} else if (inBounds(x, y, TOP_RIGHT.getX(), WINDOW_WIDTH, TOP_RIGHT.getY(), TOP_LINE.getEndY())) {
			return Positions.TOP_RIGHT;
		} else if (inBounds(x, y, LEFT.getX(), BOTTOM_MIDDLE.getX(), LEFT.getY(), BOTTOM_MIDDLE.getY())) {
			return Positions.LEFT;
		} else if (inBounds(x, y, MIDDLE.getX(), BOTTOM_RIGHT.getX(), MIDDLE.getY(), BOTTOM_RIGHT.getY())) {
			return Positions.MIDDLE;
		} else if (inBounds(x, y, RIGHT.getX(), WINDOW_WIDTH, RIGHT.getY(), BOTTOM_LINE.getEndY())) {
			return Positions.RIGHT;
		} else if (inBounds(x, y, BOTTOM_LEFT.getX(), LEFT_LINE.getEndX(), BOTTOM_LEFT.getY(), WINDOW_WIDTH)) {
			return Positions.BOTTOM_LEFT;
		} else if (inBounds(x, y, BOTTOM_LEFT.getX(), RIGHT_LINE.getEndX(), BOTTOM_MIDDLE.getY(), WINDOW_WIDTH)) {
			return Positions.BOTTOM_MIDDLE;
		} else if (inBounds(x, y, BOTTOM_RIGHT.getX(), WINDOW_WIDTH, BOTTOM_RIGHT.getY(), WINDOW_WIDTH)) {
			return Positions.BOTTOM_RIGHT;
		} else {
			return null;
		}
	}
	
	/**
	 * Checks to see if a point was clicked in a square
	 * @param x
	 * the x position of the click
	 * @param y
	 * the y position of the click
	 * @param x1
	 * the upper left corner of the square's x position
	 * @param x2
	 * the lower right corner of the square's x position
	 * @param y1
	 * the upper left corner of the square's y position
	 * @param y2
	 * the lower right corner of the square's y position
	 * @return
	 * if the click was made in the square
	 */
	private static boolean inBounds(double x, double y, double x1, double x2, double y1, double y2) {
		return x > x1 && x < x2 && y > y1 && y < y2;
	}
	
	/**
	 * Gets an image view containing the proper x or o set in the correct location
	 * @param location
	 * the location clicked
	 * @return
	 * an imageview
	 */
	private static ImageView getImageView(Positions location) {
		ImageView image = GAMEBOARD.getCurrentTurn() == Players.X ? new ImageView(o) : new ImageView(x);
		image.setFitHeight(IMAGE_SIZE);
		image.setFitWidth(IMAGE_SIZE);
		
		switch (location) {
			case TOP_LEFT:
				return setImageXY(image, TOP_LEFT);
			case TOP_MIDDLE:
				return setImageXY(image, TOP_MIDDLE);
			case TOP_RIGHT:
				return setImageXY(image, TOP_RIGHT);
			case LEFT:
				return setImageXY(image, LEFT);
			case MIDDLE:
				return setImageXY(image, MIDDLE);
			case RIGHT:
				return setImageXY(image, RIGHT);
			case BOTTOM_LEFT:
				return setImageXY(image, BOTTOM_LEFT);
			case BOTTOM_MIDDLE:
				return setImageXY(image, BOTTOM_MIDDLE);
			case BOTTOM_RIGHT:
				return setImageXY(image, BOTTOM_RIGHT);
			default:
				return null;
		}
	}
	
	/**
	 * Sets the XY location of an image view to the specified point
	 * @param image
	 * image to set
	 * @param location
	 * location to set it to
	 * @return
	 * the imageview
	 */
	public static ImageView setImageXY(ImageView image, Point location) {
		image.setX(location.getX());
		image.setY(location.getY());
		root.getChildren().add(image);
		return null;
	}
}