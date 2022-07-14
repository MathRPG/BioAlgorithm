#include "../include/File.h"

#include <iostream>
#include <vector>

namespace bio
{
	using std::string, std::vector;

	FastaFileReader::FastaFileReader(const string& filename)
		: file(filename)
	{
		if (!file.good())
		{
			std::cerr << "Unable to open file.\n";
			exit(EXIT_FAILURE);
		}
	}

	string FastaFileReader::get_next_sequence()
	{
		string sequence;

		while (true)
		{
			string line;

			if (!std::getline(file, line).good())
				return sequence;

			if (line.empty())
				continue;

			if (line[0] == '>')
				return sequence;

			sequence += line;
		}
	}

	vector<string> FastaFileReader::read_sequences()
	{
		vector<string> seqs;

		get_next_sequence(); // Discard first empty sequence

		while (true)
		{
			string sequence = get_next_sequence();

			if (sequence.empty())
				return seqs;

			seqs.push_back(sequence);
		}
	}
}