#include <iostream>
#include "gomoku.h"
#include <sstream>

using namespace gomoku;
using namespace std;

bool Game::is_swappable() {
	return false;
	// return steps == 1;
}

Game::Game() : is_over(false), winner(COLOR_NONE), last_move{-1, -1}, steps(0) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = COLOR_NONE;
		}
	}
}

void Game::move(Color p, Position pos) {
	move(p, pos.x, pos.y);
}

void Game::move(Color p, int x, int y) {
	if (!is_legal_move(p, Position{x,y})) return;
	board[x][y] = p;
	check_is_over();
	last_move = Position{x, y};
	steps++;
}

void Game::render() {
	ostringstream out;
	out << "step" << steps << endl;
	const int cell_size = 3;
	const int row_size = (cell_size + 1) * BOARD_SIZE + 1;
	char line[row_size + 1];
	char line2[row_size + 1];
	for (int i = 0; i < row_size; i++) {
		if (i % (cell_size + 1) == 0)
			line[i] = '+';
		else
			line[i] = '-';

		line2[i] = ' ';
	}
	line[row_size] = 0;
	line2[row_size] = 0;
	out << line << endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		// char row[row_size];  // '  O  '
		for (int j = 0; j < BOARD_SIZE; j++) {
			line2[j * (cell_size + 1)] = '|';
			if (last_move.x == i) {
				if (last_move.y == j) {
					line2[j * (cell_size + 1)] = '[';
				}
				else if (last_move.y == j - 1) {
					line2[j * (cell_size + 1)] = ']';
				}
			}

			int st = j * (cell_size + 1) + cell_size / 2 + 1;
			if (board[i][j] == COLOR_BLACK)
				line2[st] = '#';
			else if (board[i][j] == COLOR_WHITE)
				line2[st] = 'O';
			else
				line2[st] = ' ';
		}
		line2[row_size - 1] = '|';
		if (last_move.x == i && last_move.y == BOARD_SIZE - 1) {
			line2[row_size - 1] = ']';
		}

		out << line2 << " " << (char)('A' + i) << endl;
		out << line << endl;
	}

	char column_no[row_size] = {0};
	int offset = 0;
	for (int i = 1; i <= BOARD_SIZE; i++) {
		offset += sprintf_s(column_no + offset, row_size - offset, "  %-2d", i);
	}
	out << column_no;
	puts(out.str().c_str());
}

void gomoku::Game::render_result() {
	if (is_over) {
		cout << "Game over! Result: ";
		switch (winner) {
			case COLOR_BLACK:
				cout << "BLACK \"#\" WINS";

				break;
			case COLOR_WHITE:
				cout << "WHITE \"O\" WINS";
				break;
			case COLOR_NONE:
				cout << "DRAW";
				break;
		}
		cout << endl;
	}
	else {
		cout << "Game is not over yet." << endl;
	}
}

void Game::check_is_over() {
	int k;
	Color current;
	int i, j;

	for (i = 0; i < BOARD_SIZE - 5 + 1; i++) {
		for (j = 0; j < BOARD_SIZE - 5 + 1; j++) {
			current = board[i][j];
			if (current == COLOR_NONE) continue;
			for (k = 0; k < 5; k++) {
				if (current != board[i + k][j + k]) break;
			}
			if (k == 5 && (C_SIX_WIN ||
				get(i - 1, j - 1) != current &&
				get(i + BOARD_SIZE, j + BOARD_SIZE) != current)) {
				goto found;
			}
		}
	}
	for (i = 0; i < BOARD_SIZE - 5 + 1; i++) {
		for (j = 5 - 1; j < BOARD_SIZE - 1; j++) {
			current = board[i][j];
			if (current == COLOR_NONE) continue;
			for (k = 0; k < 5; k++) {
				if (current != board[i + k][j - k]) break;
			}
			if (k == 5 && (C_SIX_WIN ||
				get(i - 1, j + 1) != current &&
				get(i + BOARD_SIZE, j - BOARD_SIZE) != current)) {
				goto found;
			}
		}
	}
	for (i = 0; i < BOARD_SIZE - 1; i++) {
		for (j = 0; j < BOARD_SIZE - 5 + 1; j++) {
			current = board[i][j];
			if (current == COLOR_NONE) continue;
			for (k = 0; k < 5; k++) {
				if (current != board[i][j + k]) break;
			}
			if (k == 5 && (C_SIX_WIN ||
				get(i, j - 1) != current &&
				get(i, j + BOARD_SIZE) != current)) {
				goto found;
			}
		}
	}
	for (i = 0; i < BOARD_SIZE - 5 + 1; i++) {
		for (j = 0; j < BOARD_SIZE - 1; j++) {
			current = board[i][j];
			if (current == COLOR_NONE) continue;
			for (k = 0; k < 5; k++) {
				if (current != board[i + k][j]) break;
			}
			if (k == 5 && (C_SIX_WIN ||
				get(i - 1, j) != current &&
				get(i + BOARD_SIZE, j) != current)) {
				goto found;
			}
		}
	}
	bool filled = true;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (get(i, j) == COLOR_NONE) {
				filled = false;
				goto filled_break;
			}
		}
	}
filled_break:
	is_over = filled;
	winner = COLOR_NONE;
	return;
found:
	is_over = true;
	winner = current;
	return;
}

bool Game::is_legal_move(Color color, Position pos) {
	return (pos.x >= 0 && pos.x < BOARD_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE)
		&& get(pos) == COLOR_NONE;
}

void Game::get_observation(Observation &obsv, Color pov) {
	//for (int i = 0; i < BOARD_SIZE; i++) {
	//	for (int j = 0; j < BOARD_SIZE; j++) {
	//		auto c = get(i, j);
	//		if (c == pov) {
	//			obsv[i][j][0] = 1;
	//			obsv[i][j][1] = 0;
	//		}
	//		else if (c == -pov) {
	//			obsv[i][j][0] = 0;
	//			obsv[i][j][1] = 1;
	//		}
	//		else {
	//			obsv[i][j][0] = 0;
	//			obsv[i][j][1] = 0;
	//		}
	//	}
	//} // NHWC

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			auto c = get(i, j);
			if (c == pov) {
				obsv[0][i][j] = 1;
				obsv[1][i][j] = 0;
			}
			else if (c == -pov) {
				obsv[0][i][j] = 0;
				obsv[1][i][j] = 1;
			}
			else {
				obsv[0][i][j] = 0;
				obsv[1][i][j] = 0;
			}
		}
	} // NCHW
}

