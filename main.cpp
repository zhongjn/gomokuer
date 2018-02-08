#include <Python.h>
#include <atlconv.h>
#include <numpy/arrayobject.h>
#include <iostream>

#include "py_util.h"
#include "weval.h"
#include "selfplay.h"
#include "play.h"
using namespace std;

int main(int argc, char *argv[]) {

	py_util::init_python();

	if (argc <= 1) {
		printf("commands:\n");
		printf("play        play with gomokuer\n");
		printf("  -w            weigh path\n");
		printf("  -k            times of MCTS simulation\n");
		printf("  -c            color (\"b\"=black, \"w\"=white, without quotation mark)\n");
		printf("selfplay    generate selfplay data\n");
		printf("  -w            weigh path\n");
		printf("  -r            rounds\n");
		printf("weval        evaluate two weights (result = POV of w0)\n");
		printf("  -w0           weight0 path\n");
		printf("  -w1           weight1 path\n");
		printf("  -r            rounds\n");
		printf("  -o            output directory\n");
		system("pause");
		return 0;
	}

	if (strcmp(argv[1], "selfplay") == 0) {
		char *weight = nullptr;
		int rounds = 0;
		for (int i = 2; i < argc; i++) {
			if (strcmp(argv[i], "-w") == 0) {
				weight = argv[++i];
			}
			else if (strcmp(argv[i], "-r") == 0) {
				sscanf(argv[++i], "%d", &rounds);
			}
			else {
				printf("unknown argument %s", argv[i]);
				return -1;
			}
		}
		selfplay::run(weight, rounds);
	}
	else if (strcmp(argv[1], "weval") == 0) {
		char *w0 = nullptr;
		char *w1 = nullptr;
		int rounds = 0;
		char *o = nullptr;
		for (int i = 2; i < argc; i++) {
			if (strcmp(argv[i], "-w0") == 0) {
				w0 = argv[++i];
			}
			else if (strcmp(argv[i], "-w1") == 0) {
				w1 = argv[++i];
			}
			else if (strcmp(argv[i], "-r") == 0) {
				sscanf(argv[++i], "%d", &rounds);
			}
			else if (strcmp(argv[i], "-o") == 0) {
				o = argv[++i];
			}
			else {
				printf("unknown argument %s", argv[i]);
				return -1;
			}
		}
		weval::run(w0, w1, rounds, o);
		
	}
	else if (strcmp(argv[1], "play") == 0) {
		char *w = nullptr;
		char *c = nullptr;
		int k = 0;
		for (int i = 2; i < argc; i++) {
			if (strcmp(argv[i], "-w") == 0) {
				w = argv[++i];
			}
			else if (strcmp(argv[i], "-c") == 0) {
				c = argv[++i];
			}
			else if (strcmp(argv[i], "-k") == 0) {
				sscanf(argv[++i], "%d", &k);
			}
			else {
				printf("unknown argument %s", argv[i]);
				return -1;
			}
		}
		play::run(w, k, c);
	}


	return 0;
}

