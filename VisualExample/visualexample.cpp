#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <iostream>
#include <string>

#include "distribution.h"
#include "graph.h"
#include "set.h"
#include "sparse_matrix.h"
#include "graphics.h"
#include "optimize.h"
#include "disthelper.h"

using std::cout;
using std::endl;
using std::string;


int main()
{
	// create window
	sf::RenderWindow Window(sf::VideoMode(1200, 960), "Radiotherapy");
	
	// graphics vectors
	vector< vector< Voxel > > voxels = initVoxels(Window);
	vector< BeamLet > top = initTop(Window);
	vector< BeamLet > right = initRight(Window);
   // create graphs
   Graph distOnX("Dist on X", 900, 600, -1., 100., -.5, 1.5);
   Graph distOnY("Dist on Y", 900, 600, -1., 100., -.5, 1.5);
   Graph intDistOnX("Int Dist on X", 900, 600, -1., 100., -2, 40.);
   Graph intDistOnY("Int Dist on Y", 900, 600, -1., 100., -2, 40.);

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
      
      distOnX.update();
      distOnY.update();
      intDistOnX.update();
      intDistOnY.update();

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

			if( !cuttingPlaneMethod(env, solution, vox, X, Y, Ux, Ly, u, v) ){
				cout << "Couldn't solve\n";

				std::cin.ignore();
				return EXIT_FAILURE;
			}
         // Set the doses to the correct amount
			setDosesFromSolution(solution.ArgMin, top, right);
			updateDoses(voxels, top, right);

         DistHelper disthelper(&vox, &X, &Y, &solution.ArgMin);

         distOnX.addplot(&disthelper, &DistHelper::distOnX);
         distOnX.setColor(0, 255, 0);
         distOnX.addplot(&u, &UniformDist::cdf);
         distOnY.addplot(&disthelper, &DistHelper::distOnY);
         distOnY.setColor(0, 255, 0);
         distOnY.addplot(&v, &UniformDist::cdf);
         intDistOnX.addplot(&disthelper, &DistHelper::intDistOnX);
         intDistOnX.setColor(0,255,0);
         intDistOnX.addplot(&u, &UniformDist::intToInf);
         intDistOnY.addplot(&disthelper, &DistHelper::intDistOnY);
         intDistOnY.setColor(0,255,0);
         intDistOnY.addplot(&v, &UniformDist::intTo);
         
         env.end();

			solved = true;
		}
	}

   std::cin.ignore();
   return 0;
}

