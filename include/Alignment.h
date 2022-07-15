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

		friend std::ostream& operator<<(std::ostream& out, const Alignment& self)
		{
			out << "Score: " << self.score << '\n';
			out << self.aligned_seq1 << '\n';
			out << std::string(self.aligned_seq1.length(), '|') << '\n';
			out << self.aligned_seq2 << '\n';
			return out;
		}
	};
}

#endif