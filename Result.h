#ifndef _RESULT_H_
#define _RESULT_H_

#include <string>
#include <iostream>

namespace bio
{
	struct Result
	{
		const int score;
		std::string aligned_seq1;
		std::string aligned_seq2;

		friend std::ostream& operator<<(std::ostream& out, const Result& self)
		{
			out << self.score << '\n';
			out << self.aligned_seq1 << '\n';
			out << std::string(self.aligned_seq1.length(), '|') << '\n';
			out << self.aligned_seq2 << '\n';
			return out;
		}
	};
}

#endif