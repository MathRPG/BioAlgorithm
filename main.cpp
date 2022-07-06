#include <iostream>

#include "bio.h"

int main()
{
	/*
	 * Examples from:
	 * https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm
	 * https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm
	 */

	std::cout << " === Smith-Waterman (Local) === \n";

	bio::SmithWaterman alg("TGTTACGG", "GGTTGACTA", { .match = +3, .mismatch = -3, .gap = -2, });
	bio::Result res = alg.run_algorithm();
	alg.print_matrix(std::cout);
	std::cout << "Score: " << res << '\n';

	std::cout << " === Needleman-Wunsch (Global) === \n";

	bio::NeedlemanWunsch alg2("GCATGCG", "GATTACA", { .match = +1, .mismatch = -1, .gap = -1, });
	bio::Result res2 = alg2.run_algorithm();
	alg2.print_matrix(std::cout);
	std::cout << "Score: " << res2 << '\n';

	return 0;
}
