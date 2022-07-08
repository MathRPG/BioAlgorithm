#ifndef _FILE_H_
#define _FILE_H_

#include <fstream>
#include <string>
#include <vector>

namespace bio
{
	class FastaFileReader
	{
		std::ifstream file;
	 public:
		explicit FastaFileReader(const std::string& filename);
		[[nodiscard]] std::vector<std::string> read_sequences();
	};
}

#endif //_FILE_H_
