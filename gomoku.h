#ifndef _GOMOKU
#define _GOMOKU
#include <iostream>

namespace gomoku {
	const bool C_SIX_WIN = true;
	const int BOARD_SIZE = 15;
	// const int BOARD_ARRAY_LENGTH = 2 * BOARD_SIZE * BOARD_SIZE;

	typedef int8_t Color;
	const Color COLOR_WHITE = -1;
	const Color COLOR_NONE = 0;
	const Color COLOR_BLACK = 1;

	typedef Color Board[BOARD_SIZE][BOARD_SIZE];
	//typedef float Observation[BOARD_SIZE][BOARD_SIZE][2]; // NHWC
	typedef float Observation[2][BOARD_SIZE][BOARD_SIZE]; // NCHW

	struct Position {
		int x;
		int y;
		bool operator==(const Position &p1) { return x == p1.x && y == p1.y; };
	};

	inline Position index2pos(int index) {
		return Position{index / BOARD_SIZE, index % BOARD_SIZE};
	}
	inline int pos2index(Position pos) {
		return pos.x * BOARD_SIZE + pos.y;
	}
	inline int pos2index(int x, int y) {
		return x * BOARD_SIZE + y;
	}

	class Game {
	public:
		Board board;
		bool is_over;
		Color winner;
		inline Color get(Position pos) {
			return get(pos.x, pos.y);
		}
		inline Color get(int x, int y) {
			if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
				return board[x][y];
			}
			else {
				return 0;
			}
		}
		Game();
		void move(Color color, Position pos);
		void move(Color color, int x, int y);
		void render();
		void render_result();
		bool is_swappable(); // define the swap rule here. Swap1 & Swap2 are surpported.
	//private:
		void check_is_over();
		bool is_legal_move(Color color, Position pos);
		void get_observation(Observation &obsv, Color pov);
	private:
		Position last_move;
		int steps;
	};


	//typedef int Board[2][BOARD_SIZE][BOARD_SIZE];
	//inline int get_board_index(Player player, Position pos) {
	//	return player * BOARD_SIZE * BOARD_SIZE + pos.x * BOARD_SIZE + pos.y;
	//}
	//inline int get_board_index(int player, int x, int y) {
	//	return player * BOARD_SIZE * BOARD_SIZE + x * BOARD_SIZE + y;
	//}
	//inline int get_board_value_safe(Board &board, int player, int x, int y) {
	//	int i = get_board_index(player, x, y);
	//	if (x < 0 || x >= BOARD_SIZE)
	//		return 0;
	//	else
	//		return board[i];
	//}

	//void render_board(Game &board);
	//void check_gameover(Game &board, bool &is_gameover, Player &player);
}
#endif