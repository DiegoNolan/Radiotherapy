#pragma once

#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

#include <SFML/Graphics.hpp>
#include <vector>
#include "set.h"

using std::vector;

#define ROWS 30
#define COLS 30
#define SIZE 25


enum mode
{
	SET_TYPE = 0,
	SHOW_DOSES,
	SOLVE,
	SHOW_TYPE
};

enum vox_type
{
	NONE = 0,
	PTV,
	OAR
};

sf::Color getColor(unsigned int val, unsigned int max);

class Voxel : public sf::RectangleShape
{
public:
	Voxel(){type = NONE; dose = 0; }
	void setDose(unsigned int d){dose = d; this->setFillColor(getColor(dose, 100));}
	unsigned short type;
	unsigned int dose;
};

void showTypes(vector<vector<Voxel> >& voxels);

class BeamLet : public sf::RectangleShape
{
public:
	BeamLet(){strength = 10;}
	void setStrength(unsigned int str)
	{
		strength = str;
		if(!vert){
			this->setSize(sf::Vector2f(float(SIZE)*.75f, float(strength)));
		}else{
			this->setSize(sf::Vector2f(float(strength), float(SIZE)*.75f));
		}
	}
	unsigned int strength;
	bool vert;
	
};

void updateDoses(vector< vector< Voxel > > & voxels, vector< BeamLet> & top, vector< BeamLet > & right);

vector< vector< Voxel > > initVoxels(sf::RenderWindow & window);

vector< BeamLet > initTop(sf::RenderWindow & window );

vector< BeamLet > initRight(sf::RenderWindow & window);

void handleEvents(sf::RenderWindow & Window, unsigned int & cur_mode, bool & solved,
	vector<vector<Voxel> >& voxels, vector< BeamLet >& top, vector< BeamLet>& right);

void display(sf::RenderWindow& Window, vector<vector<Voxel> >&voxels, vector<BeamLet>& right,
	vector<BeamLet>& top);

vector< vector<IloInt> > getVoxMat(vector< vector< Voxel> >& voxels,
	vector<BeamLet>& top, vector<BeamLet>& right, IloInt& m, IloInt& n);

void setPTVandOAR(vector<vector<Voxel> >& voxels, Set & X, Set & Y);

void setDosesFromSolution(IloNumArray& vals, vector<BeamLet>& top, vector<BeamLet>& right);