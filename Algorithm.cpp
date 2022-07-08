#include <array>
#include <iomanip>
#include <string>

#include "Algorithm.h"
#include "Result.h"

namespace bio
{
	using std::string;

	Result Algorithm::traceback() const
	{
		const auto alignment_size = std::max(seq1.length(), seq2.length());

		std::string aligned1;
		aligned1.reserve(alignment_size);

		std::string aligned2;
		aligned2.reserve(alignment_size);

		for (auto [i, j] = traceback_start(); !should_stop_traceback(i, j);)
		{
			const int score = m[i][j];

			if (score == m[i - 1][j] + weights.gap)
			{
				aligned1 += seq1[i];
				aligned2 += '-';
				i--;
				continue;
			}

			if (score == m[i][j - 1] + weights.gap)
			{
				aligned1 += '-';
				aligned2 += seq2[j];
				j--;
				continue;
			}

			aligned1 += seq1[i];
			aligned2 += seq2[j];
			i--;
			j--;
		}

		std::reverse(aligned1.begin(), aligned1.end());
		std::reverse(aligned2.begin(), aligned2.end());

		return { get_max_score(), aligned1, aligned2 };
	}

	void Algorithm::print_matrix(std::ostream& out) const
	{
		auto print_spaced = [&](const char& c)
		{ out << ' ' << c << " |"; };

		out << "Seq1: " << seq1 << '\n';
		out << "Seq2: " << seq2 << '\n';

		out << "   |";
		for (const auto& c : seq2)
			print_spaced(c);
		out << '\n';

		for (int i = 0; i < m.size(); ++i)
		{
			print_spaced(seq1[i]);
			for (const auto& r : m[i])
			{
				out << std::setw(3) << r << '|';
			}
			out << '\n';
		}

		out << '\n';
	}
}