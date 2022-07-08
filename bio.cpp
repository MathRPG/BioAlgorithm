#include "bio.h"

#include <array>
#include <iomanip>
#include <string>

namespace bio
{
	using std::string;

	string AlignmentAlgorithm::prepare(const string& s)
	{
		return string(" ") + s;
	}

	int AlignmentAlgorithm::gapless_score_at(int i, int j) const
	{
		bool is_match = seq1[i] == seq2[j];
		return weights.gapless(is_match);
	}

	int AlignmentAlgorithm::calculate_score_at(int i, int j) const
	{
		return std::max({
			m[i - 1][j - 1] + gapless_score_at(i, j),
			m[i][j - 1] + weights.gap,
			m[i - 1][j] + weights.gap,
		});
	}

	int SmithWaterman::calculate_score_at(int i, int j) const
	{
		int temp_score = AlignmentAlgorithm::calculate_score_at(i, j);
		return std::max(temp_score, 0);
	}

	void AlignmentAlgorithm::update_if_needed(int i, int j)
	{
		/* do nothing */
	}

	void SmithWaterman::update_if_needed(int i, int j)
	{
		if (m[i][j] > m[max_i][max_j])
		{
			max_i = i;
			max_j = j;
		}
	}

	Result AlignmentAlgorithm::run_algorithm()
	{
		for (int i = 1; i < seq1.length(); ++i)
		{
			for (int j = 1; j < seq2.length(); ++j)
			{
				m[i][j] = calculate_score_at(i, j);
				update_if_needed(i, j);
			}
		}

		return generate_result();
	}

	int AlignmentAlgorithm::get_max_score() const
	{
		auto [i, j] = traceback_start();
		return m[i][j];
	}

	Result AlignmentAlgorithm::generate_result() const
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

	void AlignmentAlgorithm::print_matrix(std::ostream& out) const
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

	std::pair<int, int> NeedlemanWunsch::traceback_start() const
	{
		return std::make_pair(seq1.length() - 1, seq2.length() - 1);
	}

	std::pair<int, int> SmithWaterman::traceback_start() const
	{
		return std::make_pair(max_i, max_j);
	}

	bool NeedlemanWunsch::should_stop_traceback(int i, int j) const
	{
		return i == 0 && j == 0;
	}

	bool SmithWaterman::should_stop_traceback(int i, int j) const
	{
		return m[i][j] == 0;
	}
}