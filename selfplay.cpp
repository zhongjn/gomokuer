#include "selfplay.h"


double get_temp(int i_step) {
	//if (i_step <= 6) {
	//	return 1.0;
	//}
	//else {
	//	return 0.0;
	//}
	if (i_step <= 8) {
		return -0.125 * i_step + 1.0;
	}
	else {
		return 0.0;
	}
}
int selfplay_init_numpy() {
	if (PyArray_API == NULL) {
		import_array();
	}
}
void selfplay::save_samples(vector<StepSample> &samples) {
	selfplay_init_numpy();

	int count = samples.size();

	auto obsvs = new Observation[count];
	auto probs = new SearchedProb[count];
	auto results = new int[count];

	int i = 0;
	for (auto &sample : samples) {
		memcpy(&obsvs[i], &sample.observation, sizeof(Observation));
		memcpy(&probs[i], &sample.prob, sizeof(SearchedProb));
		results[i] = sample.result;
		i++;
	}

	npy_intp dims_obsvs[4] = {count, 2, BOARD_SIZE, BOARD_SIZE};
	npy_intp dims_probs[2] = {count, BOARD_SIZE * BOARD_SIZE};
	npy_intp dims_results[1] = {count};

	auto py_obsvs = PyArray_SimpleNewFromData(4, dims_obsvs, NPY_FLOAT, obsvs);
	auto py_probs = PyArray_SimpleNewFromData(2, dims_probs, NPY_FLOAT, probs);
	auto py_results = PyArray_SimpleNewFromData(1, dims_results, NPY_INT, results);

	PyObject_CallMethod(py_util::module, "save_samples", "OOO", py_obsvs, py_probs, py_results);

	Py_XDECREF(py_obsvs);
	Py_XDECREF(py_probs);
	Py_XDECREF(py_results);

	delete[] obsvs;
	delete[] probs;
	delete[] results;
}

void selfplay::run(char* weight, int rounds) {
	auto evaluator = new PyEvaluator(weight);
	//auto evaluator = new SimpleEvaluator();


	for (int t = 0; t < rounds; t++) {
		vector<StepSample> samples;
		int result_cursor = 0;
		Game game;

		/*game.move(COLOR_BLACK, 5, 5);
		game.move(COLOR_BLACK, 5, 6);*/
		//game.move(COLOR_BLACK, 5, 7);
		//game.move(COLOR_BLACK, 5, 8);

		//int black[9][9] ={	{1, 0, 1, 0, 0, 0, 1, 0, 1},
		//					{0, 1, 0, 0, 1, 1, 1, 0, 0},
		//					{0, 1, 0, 1, 1, 1, 0, 0, 1},
		//					{0, 1, 1, 1, 1, 0, 1, 1, 1},
		//					{0, 0, 0, 1, 1, 1, 0, 0, 0},
		//					{1, 0, 0, 0, 0, 1, 0, 1, 0},
		//					{1, 0, 1, 1, 0, 1, 1, 0, 1},
		//					{1, 1, 0, 1, 0, 1, 1, 0, 0},
		//					{1, 0, 0, 1, 0, 0, 1, 0, 0}	};

		//int white[9][9] = {	{0, 1, 0, 1, 1, 1, 0, 1, 0},
		//					{1, 0, 1, 1, 0, 0, 0, 1, 1},
		//					{1, 0, 1, 0, 0, 0, 1, 1, 0},
		//					{1, 0, 0, 0, 0, 1, 0, 0, 0},
		//					{1, 1, 1, 0, 0, 0, 1, 1, 1},
		//					{0, 1, 1, 1, 1, 0, 1, 0, 1},
		//					{0, 1, 0, 0, 1, 0, 0, 1, 0},
		//					{0, 0, 1, 0, 1, 0, 0, 1, 1},
		//					{0, 0, 1, 0, 1, 1, 0, 1, 1}	};

		//for (int i = 0; i < 9; i++) {
		//	for (int j = 0; j < 9; j++) {
		//		if (black[i][j] == 1) {
		//			game.board[i][j] = COLOR_BLACK;
		//		}
		//		else if (white[i][j] == 1) {
		//			game.board[i][j] = COLOR_WHITE;
		//		}
		//		else {
		//			game.board[i][j] = COLOR_NONE;
		//		}
		//	}
		//}

		// game.check_is_over();
		auto state = new State(nullptr, game, COLOR_BLACK);
		auto mcts = MCTS(state, evaluator, true);
		int i_step = 0;
		while (true) {
			i_step++;
			double temp = get_temp(i_step);

			if (mcts.root->game.is_over) {
				mcts.root->game.render_result();
				//cout << "Game over! Result: ";
				//switch (mcts.root->game.winner) {
				//	case COLOR_BLACK:
				//		cout << "BLACK \"#\" WINS";

				//		break;
				//	case COLOR_WHITE:
				//		cout << "WHITE \"O\" WINS";
				//		break;
				//	case COLOR_NONE:
				//		cout << "DRAW";
				//		break;
				//}
				//cout << endl;


				for (; result_cursor < samples.size(); result_cursor++) {
					auto &sample = samples[result_cursor];
					if (mcts.root->game.winner == sample.color) {
						sample.result = 1;
					}
					else if (mcts.root->game.winner == COLOR_NONE) {
						sample.result = 0;
					}
					else {
						sample.result = -1;
					}
				}

				break;
			}
			StepSample sample;
			sample.color = mcts.root->color;

			mcts.simulate(1600);
	
			mcts.root->game.get_observation(sample.observation, mcts.root->color);
			mcts.root->get_searched_prob(sample.prob, temp);
			samples.emplace_back(sample);

			mcts.random_step(temp);
			//printf("step%d\n", i_step);
			mcts.root->game.render();

		}
		save_samples(samples);
		//system("pause");
	}
	return;
}


