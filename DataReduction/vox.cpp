#include "vox.h"

Vox::Vox(unsigned voxels, unsigned beamlets, unsigned long max_non_zeroes) :
boost::numeric::ublas::compressed_matrix<IloInt>(voxels, beamlets, max_non_zeroes)
{

}

Vox::Vox(unsigned int voxels, unsigned int beamlets,
      vector<IloInt> non_zeroes, vector<unsigned> ii, vector<unsigned> jj) :
boost::numeric::ublas::compressed_matrix<IloInt>(2000000, 2000000, 2000000)
{
   cout << "non_zeros " << non_zeroes.size() << endl;
   for(unsigned i=0;i<non_zeroes.size();++i)
   {
      this->operator()(ii[i], jj[i]) = non_zeroes[i];
      if(i%10000 == 0){
         cout << i << endl;
      }
   }
}

void Vox::write(string fname)
{
   // concat .vox to the end
   fname += ".vox";

   std::ofstream out(fname.c_str());

   if( out )
   {
      // number of rows and columns
      out << this->size1() << " " << this->size2() << endl;
      // non-zeros
      for (it1_t it1 = this->begin1(); it1 != this->end1(); it1++){
         for (it2_t it2 = it1.begin(); it2 != it1.end(); it2++){
            out << *it2 << " ";
         }
      }    
      // row index
      out << endl;
      for (it1_t it1 = this->begin1(); it1 != this->end1(); it1++){
         for (it2_t it2 = it1.begin(); it2 != it1.end(); it2++){
            out << it2.index1() << " ";
         }
      }   
      // column index
      out << endl;
      for (it1_t it1 = this->begin1(); it1 != this->end1(); it1++){
         for (it2_t it2 = it1.begin(); it2 != it1.end(); it2++){
            out << it2.index2() << " ";
         }
      }  
   }
   else
   {
      cout << "Could not open " << fname << endl;
   }
}

void Vox::read(string fname)
{
   // concat .vox to the end
   fname += ".vox";

   std::ifstream in(fname.c_str());

   if( in )
   {
      string linestring;
      std::getline( in, linestring );
      std::stringstream ss;
      ss << linestring;
      unsigned rows, columns;
      ss >> rows;
      ss >> columns;
      ss.clear();
      
      // get number of non-zeros
      std::getline( in, linestring );
      short temp;
      ss << linestring;
      unsigned count=0;
      while( ss )
      {
         ss >> temp;
         ++count;
      }


      vector<IloInt> non_zeros(count);
      vector<unsigned> ii(count);
      vector<unsigned> jj(count);
      // non-zeros
      ss.clear();
      ss << linestring;
      count = 0;
      while( ss )
      {
         ss >> non_zeros[count];
         ++count;
      }

      // rows
      ss.clear();
      std::getline( in, linestring);
      ss << linestring;
      count = 0;
      while( ss )
      {
         ss >> ii[count];
         ++count;
      }

      // columns
      ss.clear();
      std::getline( in, linestring );
      ss << linestring;
      count = 0;
      while( ss )
      {
         ss >> jj[count];
         ++count;
      }

      this->resize(rows, columns, false);
      this->reserve(non_zeros.size(), false);
      // put all elements in
      for(unsigned i=0;i<non_zeros.size();++i)
      {
         this->operator()(ii[i], jj[i]) = non_zeros[i];
      }
   }
   else
   {
      cout << "Could not open " << fname << endl;
   }
}