#include "fileio.h"

std::string getFileContentsAsString(std::string filepath)
{
   // Loads histrogram
   std::ifstream input;
   input.open(filepath);

   if( !input ){
      std::cout << "Error could not open file with path " << filepath << std::endl;
      throw(-1);
   }

   return std::string( (std::istreambuf_iterator<char>(input) ),
      (std::istreambuf_iterator<char>() ) );
}

std::string trimLeadingWhiteSpace(std::string str)
{
   for(unsigned i=0;i<str.length();++i)
   {
      if(str[i] != ' ')
      {
         return str.substr(i, std::string::npos);
      }
   }

   return str;
}

std::string trimTrailingWhiteSpace(std::string str)
{
   for(int i=str.length()-1;i>=0;--i)
   {
      if(str[i] != ' ')
      {
         str.substr(0, i);
      }
   }

   return str;
}
