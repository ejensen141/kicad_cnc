#ifndef FOOT_PRINT_PARSE_H
#define FOOT_PRINT_PARSE_H

#include <list>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <list>
#include <cmath>

#ifndef PI
#define PI 3.141592
#endif

using namespace std;

class parts{
  double x;
  double y;
};

class board{

public:
  double x;
  double y; // left top corner point everything is referenced to this point

  double size_x;
  double size_y;    //width and height of board
};

class pads{

public:
  double x;
  double y;
  double area;
  bool part;
  double orientation;
};



class kicad_footprint_report
{
	//std::string file_name;
	std::vector<pads> list_of_pads;
  int total_number_of_pads;
	bool parsed = false;
  string gcode;

public:
	//std::string file_name;
	//std::vector<pads> pad;

  double paste_modifier = 1;
  // This is the constant multiplied by the area of the pad

	int parse(string filename);
	double parse_drill_line(string line);
	int parse_position_line(string line,pads *pad);
	int parse_pads_from_file(std::vector<pads> *pad_list, string file_name);
	void print_xy();


  void optimize();
  int xy_to_gcode();
  int gcode_output_to_file(string file_name);
  int gcode_print_to_screen();
};















#endif
