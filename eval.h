#ifndef _EVALUATOR
#define _EVALUATOR
#include <vector>
#include "numpy/arrayobject.h"  
#include <Python.h>
#include "gomoku.h"
using namespace gomoku;
namespace eval {
	typedef float Policy[BOARD_SIZE * BOARD_SIZE];

	struct Evaluation {
		Policy policy;
		float value;
		void policy_set(int i, int j, float prob);
	};

	class Evaluator {
	public:
		Evaluator() {};

		virtual ~Evaluator() {};
		virtual std::vector<Evaluation*> evaluate(std::vector<Game*> games, std::vector<Color> pov) = 0;
	};

	#pragma region py
	class PyEvaluator : public Evaluator {
	public:
		bool init_succeeded;
		PyEvaluator(char *weight);
		~PyEvaluator();
		std::vector<Evaluation*> evaluate(std::vector<Game*> games, std::vector<Color> pov);
	private:
		PyObject *py_network;
	};
	#pragma endregion

	#pragma region simple
	class SimpleEvaluator : public Evaluator {
	public:
		SimpleEvaluator();
		~SimpleEvaluator();
		std::vector<Evaluation*> evaluate(std::vector<Game*> games, std::vector<Color> pov);
	};
	#pragma endregion

	//#pragma region tf
	//class TFEvaluator : public Evaluator {
	//public:
	//	TFEvaluator();
	//	~TFEvaluator();
	//	std::vector<Evaluation*> evaluate(std::vector<Game*> games, std::vector<Color> pov);
	//};
	//#pragma endregion
}

#endif