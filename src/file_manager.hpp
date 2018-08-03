#pragma once

#include <string>

/**
 * FileManager, makes it easier to manage files. 
 */
class FileManager
{
public:
	FileManager();
	virtual ~FileManager();

	/**
	 * Read a plain file to string.
	 */
	static const std::string ReadFile(const std::string& filepath);
};
