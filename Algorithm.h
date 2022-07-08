#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Weights.h"
#include "Result.h"

namespace bio
{

	class Algorithm
	{
		Weights weights;

		static std::string prepare_sequence(const std::string& s)
		{
			return std::string(" ") + s;
		};

		[[nodiscard]] int gapless_score_at(int i, int j) const
		{
			bool is_match = seq1[i] == seq2[j];
			return weights.gapless(is_match);
		};

		[[nodiscard]] int get_max_score() const
		{
			auto [i, j] = traceback_start();
			return m[i][j];
		};

		[[nodiscard]] Result traceback() const;

	 protected:
		std::string seq1;
		std::string seq2;
		std::vector<std::vector<int>> m;

		[[nodiscard]] virtual int calculate_score_at(int i, int j) const
		{
			return std::max({
				m[i - 1][j - 1] + gapless_score_at(i, j),
				m[i][j - 1] + weights.gap,
				m[i - 1][j] + weights.gap,
			});
		};

		virtual void after_cell_calculation(int i, int j)
		{
			// By default, do nothing
		};

		[[nodiscard]] virtual std::pair<int, int> traceback_start() const = 0;
		[[nodiscard]] virtual bool should_stop_traceback(int i, int j) const = 0;

	 public:
		Algorithm(const std::string& s1, const std::string& s2, const Weights& w)
			: weights(w), seq1(prepare_sequence(s1)), seq2(prepare_sequence(s2)),
			  m(seq1.length(), std::vector<int>(seq2.length()))
		{
		};

		virtual ~Algorithm() = default;

		virtual Result run_algorithm()
		{
			for (int i = 1; i < seq1.length(); ++i)
			{
				for (int j = 1; j < seq2.length(); ++j)
				{
					m[i][j] = calculate_score_at(i, j);
					after_cell_calculation(i, j);
				}
			}

			return traceback();
		};

		void print_matrix(std::ostream& out) const;
	};

}

#endif