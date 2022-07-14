#ifndef _NEEDLEMANWUNSCH_H_
#define _NEEDLEMANWUNSCH_H_

#include "Algorithm.h"
#include "Weights.h"

namespace bio
{
	class NeedlemanWunsch : public bio::Algorithm
	{
	 public:
		NeedlemanWunsch(const std::string& s1, const std::string& s2, const bio::Weights& w)
			: Algorithm(s1, s2, w)
		{
			// Initialize first row and first column with gap penalties
			for (int i = 0; i < seq1.length(); ++i)
				m[i][0] = i * w.gap;
			for (int j = 0; j < seq2.length(); ++j)
				m[0][j] = j * w.gap;
		}

	 private:
		[[nodiscard]] std::pair<int, int> traceback_start() const override
		{
			// Global starts at lower-right of the matrix
			return std::make_pair(seq1.length() - 1, seq2.length() - 1);
		};

		[[nodiscard]] bool should_stop_traceback(int i, int j) const override
		{
			// Global stops traceback at first cell of the matrix
			return i == 0 && j == 0;
		};
	};
}

#endif //_NEEDLEMANWUNSCH_H_