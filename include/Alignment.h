#ifndef _ALIGNMENT_H_
#define _ALIGNMENT_H_

#include <string>
#include <iostream>

namespace bio
{
	struct Alignment
	{
		const int score;
		const std::string aligned_seq1;
		const std::string aligned_seq2;

		[[nodiscard]] bool has_gap_at_index(const int i) const
		{
			return aligned_seq1[i] == '-' || aligned_seq2[i] == '-';
		}

		[[nodiscard]] bool has_match_at_index(const int i) const
		{
			return aligned_seq1[i] == aligned_seq2[i];
		}

		[[nodiscard]] std::string make_divider(const int i) const
		{
			std::string divider;
			divider.reserve(CHARS_PER_LINE);

			for (int j = i; j < i + CHARS_PER_LINE && j < aligned_seq1.length(); ++j)
			{
				if (has_gap_at_index(j))
					divider += ' ';
				else if (has_match_at_index(j))
					divider += '|';
				else
					divider += '.';
			}

			return divider;
		}

		void pretty_print_from_index(const int i, std::ostream& out) const
		{
			out << '\n';
			out << aligned_seq1.substr(i, CHARS_PER_LINE) << '\n';
			out << make_divider(i) << '\n';
			out << aligned_seq2.substr(i, CHARS_PER_LINE) << '\n';
		}

		void pretty_print(std::ostream& out) const
		{
			out << "Score: " << this->score << '\n';

			for (auto i = 0; i < aligned_seq1.length(); i += CHARS_PER_LINE)
				pretty_print_from_index(i, out);

			out << '\n';
		}

	 private:
		const static auto CHARS_PER_LINE = 60;
	};
}

#endif