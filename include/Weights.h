#ifndef _WEIGHTS_H_
#define _WEIGHTS_H_

namespace bio
{
	struct Weights
	{
		const int match;
		const int mismatch;
		const int gap;

		[[nodiscard]] constexpr int gapless(bool is_match) const
		{
			return is_match ? this->match : this->mismatch;
		}
	};

}

#endif