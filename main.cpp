#include <iostream>
#include <memory>

#include "include/File.h"
#include "include/NeedlemanWunsch.h"
#include "include/SmithWaterman.h"

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
			if (s == "-nw" || s == "-sw")
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

std::unique_ptr<bio::Algorithm> make_instance(
	const args::AlgorithmChoice& c, const std::string& s1, const std::string& s2, const bio::Weights& w)
{
	switch (c)
	{
	case args::AlgorithmChoice::NEDDLEMAN_WUNSCH:
		return std::make_unique<bio::NeedlemanWunsch>(s1, s2, w);
	case args::AlgorithmChoice::SMITH_WATERMAN:
		return std::make_unique<bio::SmithWaterman>(s1, s2, w);
	default:
		return nullptr;
	}
}

void show_usage(const char* program_name)
{
	std::cerr << "Usage: " << program_name << " algorithm sequence_file [-m, --matrix] [-a, --alignment]\n";
	std::cerr << "Where:\n"
				 " algorithm : Algorithm to use for sequence alignment. Either\n"
				 "              -nw for Needleman_Wunsch or\n"
				 "              -sw for Smith-Waterman\n"
				 " sequence_file : FASTA file containing two input sequences to be aligned\n"
				 " -m, --matrix : show computation matrix (default FALSE)\n"
				 " -a, --alignment : show resulting alignment (default FALSE)\n";
}

int main(const int argc, const char* argv[])
{
	const char* const program_name = argv[0];

	args::Parser p;

	try
	{
		p.parse(argc, argv);
	}
	catch (const std::invalid_argument& ex)
	{
		std::cerr << program_name << ": " << ex.what() << '\n';
		std::cerr << "Run with -h or --help for usage info\n";
		return EXIT_FAILURE;
	}

	if (p.show_help)
	{
		show_usage(program_name);
		return EXIT_SUCCESS;
	}

	std::cout << "Opening file...\n";

	auto sequences = bio::FastaFileReader(p.sequence_file).read_sequences();

	std::cout << "Running Algorithm...\n";

	auto aligner = make_instance(p.algorithm, sequences[0], sequences[1], { +3, -3, -4 });
	auto alignment = aligner->run_algorithm();

	if (p.show_matrix)
		aligner->print_matrix(std::cout);

	if (p.show_alignment)
		std::cout << alignment;

	return 0;
}
