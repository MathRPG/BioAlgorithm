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

		void pretty_print(std::ostream& out) const
		{
			const auto len = this->aligned_seq1.length();
			auto cur = 0;

			while (true)
			{
				auto last_cur = cur;

				for (auto const& s : { aligned_seq1, aligned_seq2 })
				{
					for (auto i = cur; i < last_cur + CHARS_PER_LINE && i < len; ++i)
					{
						out << s[i];
					}
					out << '\n';
				}
				out << '\n';

				cur += CHARS_PER_LINE;

				if (cur >= len)
					break;
			}
		}

	 private:
		const static auto CHARS_PER_LINE = 60;
	};
}

#endif