#include "File.h"

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

	vector<string> FastaFileReader::read_sequences()
	{
		vector<string> v;

		string line;
		string cur_seq;

		while (std::getline(file, line).good())
		{
			if (line.empty())
				continue;

			if (line[0] == '>')
			{
				if (!cur_seq.empty())
					v.push_back(cur_seq);

				cur_seq.clear();
				continue;
			}

			cur_seq += line;
		}

		// Last sequence not pushed yet
		v.push_back(cur_seq);

		return v;
	}
}