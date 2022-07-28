#ifndef _ARGS_H_
#define _ARGS_H_

#include "SmithWaterman.h"
#include "NeedlemanWunsch.h"
#include "File.h"
#include <memory>
#include <iostream>
#include <chrono>

namespace args
{
	enum class AlgorithmChoice
	{
		NONE, NEDDLEMAN_WUNSCH, SMITH_WATERMAN,
	};

	class Parser
	{
	 public:
		bool show_help = false;
		enum AlgorithmChoice algorithm = AlgorithmChoice::NONE;
		std::string sequence_file;
		bool show_matrix = false;
		bool show_alignment = false;

		void parse(const int argc, const char** argv)
		{
			std::vector<std::string_view> args(argv + 1, argv + argc);

			for (const auto& s : args)
			{
				if (s == "-h" || s == "--help")
				{
					show_help = true;
					return;
				}

				parse_single_arg(s);
			}

			if (sequence_file.empty())
				throw std::invalid_argument("Missing required argument sequence_file");

			if (algorithm == AlgorithmChoice::NONE)
				throw std::invalid_argument("Missing required argument algorithm");
		}

	 private:
		void parse_single_arg(const std::string_view& s)
		{
			if (s == "NW" || s == "SW")
			{
				if (algorithm != AlgorithmChoice::NONE)
					throw std::invalid_argument("Conflicting/duplicated arguments for algorithm");
				algorithm = (s == "-nw")
							? AlgorithmChoice::NEDDLEMAN_WUNSCH
							: AlgorithmChoice::SMITH_WATERMAN;
				return;
			}

			if (s == "-m" || s == "--matrix")
			{
				if (show_matrix)
					throw std::invalid_argument("Cannot use -m/--matrix argument twice");
				show_matrix = true;
				return;
			}

			if (s == "-a" || s == "--alignment")
			{
				if (show_alignment)
					throw std::invalid_argument("Cannot use -a/--alignment argument twice");
				show_alignment = true;
				return;
			}

			// Non-flag argument is input file path
			if (!sequence_file.empty())
				throw std::invalid_argument("Duplicated values for sequence_file argument");
			sequence_file = s;
		}
	};
}

#endif //_ARGS_H_
