#include <chrono>
#include <iostream>
#include <memory>

#include "include/Args.h"
#include "include/File.h"
#include "include/NeedlemanWunsch.h"
#include "include/SmithWaterman.h"

using namespace args;

std::unique_ptr<bio::Algorithm> make_algo_instance(
	const AlgorithmChoice& c, const std::string& s1, const std::string& s2, const bio::Weights& w)
{
	switch (c)
	{
	case AlgorithmChoice::NEDDLEMAN_WUNSCH:
		return std::make_unique<bio::NeedlemanWunsch>(s1, s2, w);
	case AlgorithmChoice::SMITH_WATERMAN:
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
				 "              NW for Needleman_Wunsch or\n"
				 "              SW for Smith-Waterman\n"
				 " sequence_file : FASTA file containing two input sequences to be aligned\n"
				 " -m, --matrix : show computation matrix (default FALSE)\n"
				 " -a, --alignment : show resulting alignment (default FALSE)\n";
}

int main(const int argc, const char* argv[])
{
	auto clock_start = std::chrono::high_resolution_clock::now();

	const char* const program_name = argv[0];

	Parser p;

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

	std::cout << "Opening file " << p.sequence_file << " ...\n";

	auto sequences = bio::FastaFileReader(p.sequence_file).read_sequences();

	std::cout << "Running Algorithm...\n";

	// TODO: Weights from command line
	auto aligner = make_algo_instance(p.algorithm, sequences[0], sequences[1], { +5, -3, -4 });
	auto alignment = aligner->run_algorithm();

	if (p.show_matrix)
		aligner->print_matrix(std::cout);

	if (p.show_alignment)
		alignment.pretty_print(std::cout);

	auto clock_end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_start);

	std::cout << "Execution time: " << duration.count() << "ms\n";

	return 0;
}
