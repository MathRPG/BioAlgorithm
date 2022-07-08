#ifndef _SMITHWATERMAN_H_
#define _SMITHWATERMAN_H_

#include "Algorithm.h"

namespace bio
{
	class SmithWaterman : public Algorithm
	{
		using Algorithm::Algorithm; // Uses 0-initialized matrix

		int max_i = 0;
		int max_j = 0;

		[[nodiscard]] int calculate_score_at(int i, int j) const override
		{
			// Does not allow negative values in the matrix
			int score = Algorithm::calculate_score_at(i, j);
			return std::max(score, 0);
		};

		void after_cell_calculation(int i, int j) override
		{
			// Needs to update max value found so far
			if (m[i][j] > m[max_i][max_j])
			{
				max_i = i;
				max_j = j;
			}
		};

		[[nodiscard]] std::pair<int, int> traceback_start() const override
		{
			return std::make_pair(max_i, max_j);
		};

		[[nodiscard]] bool should_stop_traceback(int i, int j) const override
		{
			return m[i][j] == 0;
		};
	};
}

#endif