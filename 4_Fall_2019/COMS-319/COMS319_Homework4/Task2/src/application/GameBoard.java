package application;

/**
 * Object for handling tic-tac-toe game logic
 * @author Alexander Harms
 *
 */
public class GameBoard {
	// Location in the array for each position
	private final Point TOP_LEFT = new Point(0, 0);
	private final Point TOP_MIDDLE = new Point(1, 0);
	private final Point TOP_RIGHT = new Point(2, 0);
	private final Point LEFT = new Point(0, 1);
	private final Point MIDDLE = new Point(1, 1);
	private final Point RIGHT = new Point(2, 1);
	private final Point BOTTOM_LEFT = new Point(0, 2);
	private final Point BOTTOM_MIDDLE = new Point(1, 2);
	private final Point BOTTOM_RIGHT = new Point(2, 2);
	
	// Locations to check for diagonals
	private final Point[] BACKSLASH = {TOP_LEFT, MIDDLE, BOTTOM_RIGHT};
	private final Point[] SLASH = {TOP_RIGHT, MIDDLE, BOTTOM_LEFT};
	
	private final int TOTAL_POSSIBLE_PLAYS = 9;
	
	// Game board and number of turns made
	private Players plays[][];
	private int numberOfPlays;
	
	/**
	 * Constructs a new game board
	 */
	public GameBoard() {
		newGame();
	}
	
	// Resets the game
	public void newGame() {
		plays = new Players[3][3];
		numberOfPlays = 1;
	}
	
	/**
	 * Returns if the game is over or not
	 * @return
	 * if the game is over
	 */
	public boolean gameOver() {
		return getWinner() != null;
	}
	
	/**
	 * Returns whos turn it is
	 * @return
	 * the enum x or o depending on whos turn it is
	 */
	public Players getCurrentTurn() {
		return numberOfPlays % 2 == 1 ? Players.X : Players.O;
	}
	
	/**
	 * Marks the play made
	 * @param position
	 * the position played
	 * @return
	 * the winner from the play, if there is any
	 */
	public Players markPlay(Positions position) {
		switch (position) {
			case TOP_LEFT:
				return checkPos(TOP_LEFT);
			case TOP_MIDDLE:
				return checkPos(TOP_MIDDLE);
			case TOP_RIGHT:
				return checkPos(TOP_RIGHT);
			case LEFT:
				return checkPos(LEFT);
			case MIDDLE:
				return checkPos(MIDDLE);
			case RIGHT:
				return checkPos(RIGHT);
			case BOTTOM_LEFT:
				return checkPos(BOTTOM_LEFT);
			case BOTTOM_MIDDLE:
				return checkPos(BOTTOM_MIDDLE);
			case BOTTOM_RIGHT:
				return checkPos(BOTTOM_RIGHT);
			default:
				return null;
		}
	}
	
	/**
	 * Checks to see if the play made was valid
	 * @param p
	 * position played
	 * @return
	 * winner if the position is a valid play
	 */
	private Players checkPos(Point p) {
		if (p.getAt() != null) {
			return Players.INVALID;
		} else {
			p.markCurrentPlayer();
			numberOfPlays++;
			return getWinner();
		}
	}
	
	/**
	 * Checks to see if any players made a winning move
	 * @return
	 * the winner
	 */
	public Players getWinner() {
		if (SLASH[0].getAt() == SLASH[1].getAt() && SLASH[1].getAt() == SLASH[2].getAt() && SLASH[0].getAt() != null) {
			return SLASH[0].getAt();
		}

		if (BACKSLASH[0].getAt() == BACKSLASH[1].getAt() && BACKSLASH[1].getAt() == BACKSLASH[2].getAt() && BACKSLASH[0].getAt() != null) {
			return BACKSLASH[0].getAt();
		}
		
		for (Players[] col : plays) {
			boolean cols = col[0] == col[1] && col[1] == col [2] && col[0] != null;
			if (cols) {
				return col[0];
			}
		}
		
		for (int i = 0; i < plays[0].length; i++) {
			boolean rows = plays[0][i] == plays[1][i] && plays[1][i] == plays[2][i] && plays[0][i] != null;
			if (rows) {
				return plays[0][i];
			}
		}

		if (numberOfPlays > TOTAL_POSSIBLE_PLAYS) {
			return Players.NO_ONE;
		}
		
		return null;
	}
	
	/**
	 * Class to store a position in the gameboard and
	 * to manage plays made at that position
	 */
	private class Point {
		private int x;
		private int y;
		
		public Point(int x, int y) {
			this.x = x;
			this.y = y;
		}
		
		public Players getAt() {
			return plays[x][y];
		}
		
		public void markCurrentPlayer() {
			plays[x][y] = getCurrentTurn();
		}
	}
}