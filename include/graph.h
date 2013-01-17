#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;

// how far the axis are set back to make them appear behind the graph
#define AXIS_DEPTH 0.0001f

typedef vector<GLfloat> LineStrip;

class Graph : private sf::RenderWindow
{
public:
	Graph(string graphname, int window_width, int window_height,
      double left, double right, double bottom, double top);
	~Graph();
	void update();
   // returns the index of the plot
	unsigned int addplot(double (*func)(double) );
	void save(string fname);
   void setColor(unsigned short r, unsigned short g, unsigned short b);

   // returns the index of the lpot
	template <class A>
	unsigned int addplot(A * helper, double (A::*func)(double t));
private:

	double xRange, yRange;
	double xScale, yScale, xZero, yZero;
   double rr, ll, tt, bb;
   unsigned int points;

	double convertX(double x);
	double convertY(double y);

   vector< LineStrip > verts;
   vector<GLfloat> xAxis, yAxis;
   vector< vector<GLfloat> > colors;

   unsigned short red, green, blue;
};

template <class A>
unsigned int Graph::addplot(A * helper, double (A::*func)(double t))
{
  verts.resize(verts.size()+1);
  verts[verts.size()-1].resize(points*3);

  colors.resize(colors.size()+1);
  colors[colors.size()-1].resize(3);
  colors[colors.size()-1][0] = red;
  colors[colors.size()-1][1] = green;
  colors[colors.size()-1][2] = blue;


  double gap = (rr-ll)/double(points-1);

  for(unsigned int i=0;i<points;++i)
  {
     verts[verts.size()-1][i*3] = float(convertX(double(i)*gap+ll));
     verts[verts.size()-1][i*3+1] = float(convertY( (helper->*func)(double(i)*gap+ll) ));
     verts[verts.size()-1][i*3+2] = 0.f;
  }

  return unsigned(verts.size()-1);
}
