#ifndef _BIO_H_
#define _BIO_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

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

	class AlignmentAlgorithm
	{
		Weights weights;

	 protected:
		std::string seq1;
		std::string seq2;
		std::vector<std::vector<int>> m;

	 private:
		static std::string prepare(const std::string& s);
		[[nodiscard]] int gapless_score_at(int i, int j) const;
		[[nodiscard]] int get_max_score() const;

	 public:
		AlignmentAlgorithm(const std::string& s1, const std::string& s2, const Weights& w)
			: weights(w), seq1(prepare(s1)), seq2(prepare(s2)),
			  m(seq1.length(), std::vector<int>(seq2.length()))
		{
		};

		virtual ~AlignmentAlgorithm() = default;
		virtual Result run_algorithm();
		[[nodiscard]] Result generate_result() const;
		void print_matrix(std::ostream& out) const;

	 protected:
		[[nodiscard]] virtual int calculate_score_at(int i, int j) const;
		virtual void update_if_needed(int i, int j);
		[[nodiscard]] virtual std::pair<int, int> traceback_start() const = 0;
		[[nodiscard]] virtual bool should_stop_traceback(int i, int j) const = 0;
	};

	class NeedlemanWunsch : public AlignmentAlgorithm
	{
		[[nodiscard]] std::pair<int, int> traceback_start() const override;
		[[nodiscard]] bool should_stop_traceback(int i, int j) const override;

	 public:
		NeedlemanWunsch(const std::string& s1, const std::string& s2, const Weights& w)
			: AlignmentAlgorithm(s1, s2, w)
		{
			for (int i = 0; i < seq1.length(); ++i)
				m[i][0] = i * w.gap;
			for (int j = 0; j < seq2.length(); ++j)
				m[0][j] = j * w.gap;
		}
	};

	class SmithWaterman : public AlignmentAlgorithm
	{
	 private:
		int max_i = 0;
		int max_j = 0;

		using AlignmentAlgorithm::AlignmentAlgorithm;
		[[nodiscard]] int calculate_score_at(int i, int j) const override;
		void update_if_needed(int i, int j) override;
		[[nodiscard]] std::pair<int, int> traceback_start() const override;
		[[nodiscard]] bool should_stop_traceback(int i, int j) const override;
	};
}

#endif //_BIO_H_