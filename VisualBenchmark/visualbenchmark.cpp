#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "omp.h"
#include <Windows.h>

#include "distribution.h"
#include "graph.h"
#include "set.h"
#include "sparse_matrix.h"
#include "graphics.h"
#include "optimize.h"
#include "disthelper.h"
#include "graphhelper.h"
#include "benchmark.h"
#include "randombenchmark.h"

using std::cout;
using std::endl;
using std::string;
using std::map;

//#define PTV_UPPER 400
//#define PTV_LOWER 100
//#define OAR_UPPER 400
//#define OAR_LOWER 100
#define TRIALS 10

typedef struct 
{
   double Min;
   double Time;
}MinAndTime;

template <class T>
string asString(T arg)
{
   string str;
   std::stringstream ss;
   ss << arg;
   ss >> str;

   return str;
}

void writeSol(Opt & sol, string fname, double execTime)
{
   std::ofstream out;
   out.open(fname);

   if(!out){
      cout << "Could not open file " << fname << endl;
      return;
   }

   out << "Time (s): " << execTime << endl;
   out << "Minimum: " << sol.Min << endl;

   for(unsigned i=0;i<sol.ArgMin.getSize();++i)
   {
      out << sol.ArgMin[i] << endl;
   }

   out.close();
}

int COLS;
int ROWS;
int VOX_SIZE;

int main()
{
   VOX_SIZE = 25;
   string abspath = "C:\\benchmarks\\";
   unsigned PTV_UPPER, PTV_LOWER, OAR_UPPER, OAR_LOWER;
   double up_ratio, low_ratio, size_init;
   up_ratio = .45; low_ratio = .05; size_init = 700.;
   unsigned rc_start, rc_end, rc_inc;
   rc_start = 10;
   rc_end = 10;
   rc_inc = 10;

   for(unsigned rc=rc_start;rc<=rc_end;rc+=rc_inc)
   {
      ROWS = rc;
      COLS = rc;
      VOX_SIZE = (int)(size_init/(double)(rc));
      PTV_UPPER = OAR_UPPER = (unsigned)((double)(rc*rc)*up_ratio);
      PTV_LOWER = OAR_LOWER = (unsigned)((double)(rc*rc)*low_ratio);

      string folder = "trial_" + asString(rc) + "\\";
      if( !(CreateDirectory( (abspath+folder).c_str(), NULL) ||
         ERROR_ALREADY_EXISTS == GetLastError()))
      {
         cout << "Error: could not create directiory " << (abspath+folder) << endl;
         std::cin.ignore();
         return EXIT_FAILURE;
      }


       // create window
       sf::RenderWindow Window(sf::VideoMode(1200, 960), "Radiotherapy");

       unsigned int cur_mode = SET_TYPE;
       bool solved=false;

       IloEnv env;

       std::ofstream out;
       out.open(abspath + folder + "trial");
       out << "Epsilon for CP " << EPSILON << endl;
       out << "PTV Lower Bound: " << PTV_LOWER << endl;
       out << "PTV Upper Bound: " << PTV_UPPER << endl;
       out << "OAR Lower Bound: " << OAR_LOWER << endl;
       out << "OAR Upper Bound: " << OAR_UPPER << endl;
       out << "Trials: " << TRIALS << endl;
      
       // set the benchmarks
       map< string, Benchmark<UniformDist> > setAndBenchs;


       vector<MinAndTime> MIPsols(TRIALS);
       vector<MinAndTime> CPsols(TRIALS);
       for(int count=0;count<TRIALS;++count)
       {
          cur_mode = SET_TYPE;
          Set X, Y;
          randomSetAndPTV(Y, X, ROWS, COLS, PTV_LOWER, PTV_UPPER, OAR_LOWER, OAR_UPPER);
          out << "PTV size: " << Y.size() << " OAR size: " << X.size() << endl;

          // graphics vectors
          vector< vector< Voxel > > voxels = initVoxels(Window, ROWS, COLS, VOX_SIZE);
          vector< BeamLet > top = initTop(Window, ROWS, COLS, VOX_SIZE);
          vector< BeamLet > right = initRight(Window, ROWS, COLS, VOX_SIZE);
          // create graphs
          vector<Graph*> graphs;
          
          IloInt m /* voxel count*/, n /* beamlets */ ;
          // does voxel i gets from beamlet j
          Sparse_Matrix<IloInt> vox;
          vox = getVoxMat(voxels, top, right, m, n);

          // color the voxels
          createPTVandOAR(voxels, X, Y);


          // Save the image so we know how the problem looks
          for(int dummy=0;dummy<3;++dummy)
          {
              handleEvents(Window, cur_mode, solved, voxels, top, right);
              Window.setActive(true);
              Window.clear();
              // display the visual example
              display(Window, voxels, right, top);
              Window.display();
              Window.setActive(false);
          }
          Window.capture().saveToFile(abspath + folder + "regions_" + asString(count) + ".png");
          writeSet(X, abspath + folder + "OAR_" + asString(count) + ".set");
          writeSet(Y, abspath + folder + "PTV_" + asString(count) + ".set");
           
          setAndBenchs["OAR"] = Benchmark<UniformDist>(UniformDist(-10., -9.),
             UniformDist(30., 60.), -10., -9., 2, 30., 60, 100, X);
           setAndBenchs["PTV"] = Benchmark<UniformDist>(UniformDist(30, 60.),
              UniformDist(40., 70.), 30., 60., 100, 40., 70., 5, Y);

          Opt solution(env);
            
          // START OF METHODS AND BENCHMARKING THEM
          if(!MIPMethod(env, solution, vox, setAndBenchs))
          {
            cout << "Failed to optimizate MIP" << endl;
            std::cin.ignore();
          }
          
          setDosesFromSolution(solution.ArgMin, top, right);
          updateDoses(voxels, top, right);
          solved = true;
          cur_mode = SHOW_DOSES;

          for(int dummy=0;dummy<3;++dummy)
          {
              // Save the image so we know how the problem looks
              handleEvents(Window, cur_mode, solved, voxels, top, right);
              Window.setActive(true);
              Window.clear();
              // display the visual example
              display(Window, voxels, right, top);
              Window.display();
              Window.setActive(false);
          }
          writeSol(solution, abspath + folder + "MIP_"+ asString(count) + ".sol", solution.RunTimeSeconds);
          Window.capture().saveToFile(abspath + folder + "MIP_" + asString(count) + ".png");
          MIPsols[count].Min = solution.Min;
          MIPsols[count].Time = solution.RunTimeSeconds;
          out << "MIP - min: " << solution.Min << " time (s): " << solution.RunTimeSeconds << " ";
           
          if( !genCutPlaneMeth(env, solution, vox, setAndBenchs))
          {
            cout << "Failed to solve CP " << endl;
          }
          
          setDosesFromSolution(solution.ArgMin, top, right);
          updateDoses(voxels, top, right);
          
          for(int dummy=0;dummy<3;++dummy)
          {
              // Save the image so we know how the problem looks
              handleEvents(Window, cur_mode, solved, voxels, top, right);
              Window.setActive(true);
              Window.clear();
              // display the visual example
              display(Window, voxels, right, top);
              Window.display();
              Window.setActive(false);
          }
          writeSol(solution, abspath + folder + "CPLANE_"+ asString(count) + ".sol", solution.RunTimeSeconds);
          Window.capture().saveToFile(abspath + folder + "CPLANE_" + asString(count) + ".png");
          CPsols[count].Min = solution.Min;
          CPsols[count].Time = solution.RunTimeSeconds;
          out << "CP - min: " << solution.Min << " time (s): " << solution.RunTimeSeconds << "\n";
       }

       double MIPavg=0.;
       for(unsigned i=0;i<MIPsols.size();++i)
          MIPavg += MIPsols[i].Time;

       MIPavg /= (double)(MIPsols.size());

       double CPavg=0.;
       for(unsigned i=0;i<CPsols.size();++i)
          CPavg += CPsols[i].Time;

       CPavg /= (double)(CPsols.size());

       out << "MIP avg: " << MIPavg << " CP avg: " << CPavg << endl;

       out.close();
       env.end();
   }

   cout << "Press enter to exit" << endl;
   std::cin.ignore();
   return 0;
}
