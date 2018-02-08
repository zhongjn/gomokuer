//#include <time.h>
#include <fstream>
#include <chrono>
#include "weval.h"
#include "eval.h"
#include "search.h"
#include "gomoku.h"
#include "py_util.h"


using namespace eval;
using namespace search;
using namespace std;
using namespace chrono;



void weval::run(char *w0, char *w1, int rounds, char *outdir) {
	PyEvaluator eval[2] = {PyEvaluator(w0),
						   PyEvaluator(w1)};

	for (int r = 0; r < rounds; r++) {
		Game game;

		int p_first = r % 2;
		Color p0_color;
		if (p_first == 0) {
			p0_color = COLOR_BLACK;
		}
		else {
			p0_color = COLOR_WHITE;
		}

		MCTS mcts[2] = {MCTS(new State(nullptr, Game(), COLOR_BLACK), &eval[0], false),
						MCTS(new State(nullptr, Game(), COLOR_BLACK), &eval[1], false)};

		int p = p_first;
		Color c = COLOR_BLACK;
		while (!game.is_over) {

			mcts[p].simulate(1600);
			Position pos = mcts[p].random_step(0.0);
			mcts[1 - p].step(pos);

		

			game.move(c, pos);
			game.render();
			game.check_is_over();
			c = -c;
			p = 1 - p;
		}
		game.render_result();
		string result;
		if (game.winner == p0_color) {
			result = "W";
		}
		else if (game.winner == COLOR_NONE) {
			result = "D";
		}
		else {
			result = "L";
		}
	
		PyObject_CallMethod(py_util::module, "save_eval_result", "ss", outdir, result.c_str());
		
		//auto now = time_point_cast<milliseconds>(system_clock::now());
		//
		//char filename[100] = {0};
		//sprintf_s(filename, 100, "%s/%d#%c", outdir, t, status);
		//ofstream outf;
		//outf.open(filename);
		//outf.close();
		
	}
}