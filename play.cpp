#include "play.h"
#include "search.h"
#include <conio.h>
using namespace search;
using namespace std;
void play::run(char *w, int k, char *c) {
	auto eval = PyEvaluator(w);
	Color self_color;
	if (*c == 'b') {
		self_color = COLOR_BLACK;
	}
	else if (*c == 'w') {
		self_color = COLOR_WHITE;
	}
	else {
		printf("please pick a color\n");
		std::system("pause");
		exit(1);
	}
	auto mcts = MCTS(new State(nullptr, Game(), COLOR_BLACK), &eval, false);
	Color current_color = COLOR_BLACK;

	Game game;
	game.render();
	while (!game.is_over) {
		
		Position pos;
		if (self_color != current_color) {
			bool legal = false;
			while (!legal) {
				printf("your move: ");
				char move_ic;
				int move_j;
				cin >> move_ic >> move_j;
				//scanf_s("%d", &move_j);
				pos = Position{move_ic - 'A', move_j - 1};
				legal = game.is_legal_move(current_color, pos);
				if (!legal) {
					printf("illegal input!\n");
				}
				//cin.clear();
				//cin.sync_with_stdio();
				//cin.sync();
				//fflush(stdin);

				//ch = getchar();
				Game game_temp = game;
				game_temp.move(current_color, pos);
				game_temp.render();
				cout << "confirm? [y]/n" << endl;
				char ch;
				ch = _getch();
				if (ch == 'n') {
					game.render();
					legal = false;
				}
			}
			mcts.step(pos);
		}
		else {
			mcts.simulate(k);

			cout << "value=" << mcts.root->sum_value / mcts.root->visit_count << endl;
			pos = mcts.random_step(0.0);
		}

		game.move(current_color, pos);

		game.render();
		current_color = -current_color;
	}
	game.render_result();
	std::system("pause");
}