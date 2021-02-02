#pragma once
#include <string>
#include<iostream>
#include<fstream>
#include<sstream>

class IOFile
{
public:
	static std::string readFile(const char * path);
	static int saveFile(const char* path); 
};

std::string IOFile::readFile(const char* path)
{ 
	std::ifstream file;
	// ensure ifstream objects can throw exceptions:
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try 
	{
		// open the file
		file.open(path);
		
		// stream the content to a buffer
		std::stringstream stream;
		stream << file.rdbuf();
		
		// close the file
		file.close();

		// convert the stream into a string and return it 
		return stream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::IOFile::readFile::" << path <<" \ t "<< e.what() << std::endl; 
		return NULL;
	}
}
int IOFile::saveFile(const char* path)
{ 
	std::cout << "IOFile::saveFile::Not implemented" << std::endl;
	return -1;
}