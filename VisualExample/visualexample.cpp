#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <iostream>
#include <string>
#include <map>

#include "distribution.h"
#include "graph.h"
#include "set.h"
#include "sparse_matrix.h"
#include "graphics.h"
#include "optimize.h"
#include "disthelper.h"
#include "graphhelper.h"
#include "benchmark.h"

using std::cout;
using std::endl;
using std::string;
using std::map;


int main()
{
	// create window
	sf::RenderWindow Window(sf::VideoMode(1200, 960), "Radiotherapy");
	
	// graphics vectors
	vector< vector< Voxel > > voxels = initVoxels(Window);
	vector< BeamLet > top = initTop(Window);
	vector< BeamLet > right = initRight(Window);
   // create graphs
   vector<Graph*> graphs;

	// mode we are in
	unsigned int cur_mode = SET_TYPE;

	// boolean for solved
	bool solved = false;

	while(Window.isOpen())
	{
		// handle events
		handleEvents(Window, cur_mode, solved, voxels, top, right);
		
      Window.setActive(true);
		Window.clear();
		// display the visual example
		display(Window, voxels, right, top);
		Window.display();
      Window.setActive(false);
      
      for(unsigned i=0;i<graphs.size();++i){
         graphs[i]->update();
      }

		if(cur_mode == SOLVE && !solved)
		{
			IloEnv env;

			IloInt m /* voxel count*/, n /* beamlets */ ;
			// does voxel i gets from beamlet j
			Sparse_Matrix<IloInt> vox;
			vox = getVoxMat(voxels, top, right, m, n);

			IloNum Ux, Ly;
			Ux = 0;	Ly = 90;
			UniformDist u(20, 50);
			UniformDist v(30, 60);

			Set X /* OAR */, Y /* PTV */;
			
			// Denote where the PTV and OAR are
			setPTVandOAR(voxels, X, Y);

			Opt solution(env);
         Opt sol2(env);

         // set the benchmarks
         map< string, Benchmark<UniformDist> > setAndBenchs;
         setAndBenchs["OAR"] = Benchmark<UniformDist>(UniformDist(-10., -9.),
            UniformDist(20., 50.), -10., -9., 2, 20., 50, 100, X);
         setAndBenchs["PTV"] = Benchmark<UniformDist>(UniformDist(30, 60.),
            UniformDist(40., 70.), 30., 60., 100, 40., 70., 5, Y);

         if( !MIPMethod(env, solution, vox, setAndBenchs)){
            cout << "Could not solve" << endl;
            std::cin.ignore();
            return EXIT_FAILURE;
         }
			/*if( !cuttingPlaneMethod(env, sol2, vox, X, Y, Ux, Ly, u, v) ){
				cout << "Couldn't solve\n";

				std::cin.ignore();
				return EXIT_FAILURE;
			}*/
         // Set the doses to the correct amount
			setDosesFromSolution(solution.ArgMin, top, right);
			updateDoses(voxels, top, right);
   
         makeGraphs(graphs, vox, solution.ArgMin, setAndBenchs);         
      
         env.end();

			solved = true;
		}
	}

   // deallocate graphs
   for(unsigned i=0;i<graphs.size();++i){
      delete graphs[i];
   }

   std::cin.ignore();
   return 0;
}

