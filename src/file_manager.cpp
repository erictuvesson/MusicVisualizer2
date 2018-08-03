#include "file_manager.hpp"

#include <fstream>
#include <streambuf>

FileManager::FileManager()
{
	
}

FileManager::~FileManager()
{

}

const std::string FileManager::ReadFile(const std::string& filepath)
{
	std::ifstream fileStream(filepath);
	return std::string(std::istreambuf_iterator<char>(fileStream),
					   std::istreambuf_iterator<char>());
}
