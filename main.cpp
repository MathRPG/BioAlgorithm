#include <iostream>

#include "NeedlemanWunsch.h"
#include "SmithWaterman.h"

int main()
{
	/*
	 * Examples from:
	 * https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm
	 * https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm
	 * Classes
	 */

//	std::cout << " === Smith-Waterman (Local) === \n";
//	bio::SmithWaterman alg_smith("TGTTACGG", "GGTTGACTA", { .match = +3, .mismatch = -3, .gap = -2, });
//	bio::Result res = alg_smith.run_algorithm();
//	alg_smith.print_matrix(std::cout);
//	std::cout << "Score: " << res << '\n';

//	std::cout << " === Needleman-Wunsch (Global) === \n";
//	bio::NeedlemanWunsch alg2("GCATGCG", "GATTACA", { .match = +1, .mismatch = -1, .gap = -1, });
//	bio::Result res2 = alg2.run_algorithm();
//	alg2.print_matrix(std::cout);
//	std::cout << "Score: " << res2 << '\n';
//	std::cout << "Exmplo Professor";

	bio::NeedlemanWunsch alg_needle("GGATCGA", "GAATTCAGTTA", { .match = +5, .mismatch = -3, .gap = -4 });
	bio::Result result_needle = alg_needle.run_algorithm();
	alg_needle.print_matrix(std::cout);
	std::cout << "Score: " << result_needle << '\n';

	bio::SmithWaterman alg_smith("GGATCGA", "GAATTCAGTTA", { .match = +5, .mismatch = -3, .gap = -4 });
	bio::Result result_smith = alg_smith.run_algorithm();
	alg_smith.print_matrix(std::cout);
	std::cout << "Score: " << result_smith << '\n';

	return 0;
}
