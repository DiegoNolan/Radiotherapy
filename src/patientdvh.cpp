#include "patientdvh.h"

PatientDVH::PatientDVH()
{

}

PatientDVH::PatientDVH(string filename)
{
   string f_contents = getFileContentsAsString(filename);

   // get the patient name
   getPatientName(f_contents);
   cout << patientName << endl;

   // itarate through and get each structure
   size_t start_pos = f_contents.find("Structure:", 0);
   while(start_pos != string::npos) // while we keeping finding new structures
   {
      size_t pos = start_pos;
      structures.push_back( Histogram( getStructureName(f_contents, pos) ) );
      vector<Histogram_Triplet> data;

      // iterater through data
      bool indata = false;
      
      pos = findFirstDataLine(f_contents, pos);

      Histogram_Triplet temp_trp;
      while( isDataLine(f_contents, pos) )
      {
         
         temp_trp.relDose = getNextNumber(f_contents, pos);
         temp_trp.dose = getNextNumber(f_contents, pos);
         temp_trp.ratioTotStruct = getNextNumber(f_contents, pos);

         data.push_back(temp_trp);
      }

      structures[structures.size()-1].setData(data);

      start_pos = f_contents.find("Structure:", pos);
   }
}

PatientDVH::~PatientDVH()
{

}


// private members
void PatientDVH::getPatientName(string & f_contents)
{
   for(unsigned i=0;i<f_contents.length();++i)
   {
      if(f_contents[i] == ':') // find the first colon, first line is Patinet Name
      {
         for(unsigned j=i+1;j<f_contents.length();++j) // find new line
         {
            if(f_contents[j] == '\n')
            {
               patientName = trimLeadingWhiteSpace(
                  trimTrailingWhiteSpace(
                  f_contents.substr(i+1, j-(i+1)) ) );
               return;
            }
         }
      }
   }

   patientName = "Not Found";
   return;
}

string PatientDVH::getStructureName(string & f_contents, size_t & pos)
{
   for(;pos<f_contents.length();++pos)
   {
      if(f_contents[pos] == ':')
      {
         size_t pos_store = pos+1;
         for(;pos<f_contents.length();++pos)
         {
            if(f_contents[pos] == '\n')
            {
               return trimLeadingWhiteSpace(
                  trimTrailingWhiteSpace(
                  f_contents.substr(pos_store, pos-pos_store) ) );
            }
         }
      }
   }

   return string();
}

size_t PatientDVH::findFirstDataLine(string & f_contents, size_t start_pos)
{
   size_t pos;
   for(pos = start_pos; pos<f_contents.length();++pos)
   {
      // newline with a space, could be data line
      if(f_contents[pos] == ' ' && f_contents[pos-1] == '\n')
      { 
         for(size_t n_pos = pos;
            n_pos<f_contents.length() && f_contents[n_pos] != '\n'
            ;++n_pos)
         {
            // found something besides blank space
            if(f_contents[n_pos] != ' ' && f_contents[n_pos] != '\n' && f_contents[n_pos] != '\t' )
            {
               return pos;
            }
         }
      }
   }

   return pos;
}

bool PatientDVH::isDataLine(string & f_contents, size_t pos)
{
   for(size_t n_pos = pos+1;
      n_pos<f_contents.length() && f_contents[n_pos] != '\n'
      ;++n_pos)
   {
      // found something besides blank space
      if(f_contents[n_pos] != ' ' && f_contents[n_pos] != '\n' && f_contents[n_pos] != '\t' )
      {
         return true;
      }
   }

   for(size_t n_pos = pos;
      n_pos >= 0 && f_contents[n_pos] != '\n'
      ;--n_pos)
   {
      if(f_contents[n_pos] != ' ' && f_contents[n_pos] != '\n' && f_contents[n_pos] != '\t' )
      {
         return true;
      }
   }

   return false;
}

double PatientDVH::getNextNumber(string & f_contents, size_t & pos)
{
   unsigned number = 0;
   unsigned digs_aft_dec_pt = 0;
   unsigned num_aft_exp = 0;
   bool found_dec_pt = false;
   bool found_num = false;
   bool found_exp = false;
   for(;pos<f_contents.length();++pos)
   {
      if(f_contents[pos] != ' ' && f_contents[pos] != '\t' && f_contents[pos] != '\n')
      {
         found_num = true;

         if(f_contents[pos] >= '0' && f_contents[pos] <= '9')
         {
            if(found_dec_pt && !found_exp){
               ++digs_aft_dec_pt;
            }
            
            if(!found_exp){
               number *= 10;
               number += unsigned(f_contents[pos] - '0');
            }else{
               num_aft_exp *= 10;
               num_aft_exp += unsigned(f_contents[pos] - '0');
            }
         }
         else if(f_contents[pos] == '.')
         {
            found_dec_pt = true;
         }
         else if(f_contents[pos] == 'e' || f_contents[pos] == '-')
         {
            found_exp = true;
         }
         else
         {
            cout << "Unknown character type during parse " << f_contents[pos] << endl;
         }
      }

      if((f_contents[pos] == ' ' || f_contents[pos] == '\t' || f_contents[pos] == '\n') &&
         found_num)
      {
         if(!found_exp){
            return double(number)/pow( 10., double(digs_aft_dec_pt) );
         }else{
            double temp_num = double(number)/pow( 10., double(digs_aft_dec_pt) );
            return temp_num/(pow( 10., double(num_aft_exp) ) );
         }
      }
   }
}
