#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <string>
#include <fstream>

std::string getFileContentsAsString(std::string filepath);

std::string trimLeadingWhiteSpace(std::string str);
std::string trimTrailingWhiteSpace(std::string str);


#endif // FILEIO_H
