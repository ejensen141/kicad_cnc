#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <list>
#include <cmath>
#include "src/footprint_parse.hh"

using namespace std;


void print_help();


int main(int argc, char *argv[])
{
string file_name;
string output_file_name;
string command;
bool input_file_true = false;
bool output_file_true = false;
bool print_true = false;

switch ( argc ) {
case 2:
  file_name = argv[1];
  cout<<"Processing "<<file_name<<'\n';
  input_file_true = true;
  break;
case 3:
    file_name = argv[1];
    cout<<"Processing "<<file_name<<'\n';
    output_file_name = argv[2];
    input_file_true = true;
    output_file_true =true;
    break;
case 4:
    file_name = argv[1];
    cout<<"Processing "<<file_name<<'\n';
    output_file_name = argv[2];
    input_file_true = true;
    output_file_true =true;
    command = argv[3];
    if(command == "print")
    {
      print_true = true;
      cout<<command;
    }
    break;

default:
  print_help();
  break;
}

//string file_name = "mosfetdrive1.rpt";

kicad_footprint_report rpt;

int error;

if(input_file_true)
{
error = rpt.parse(file_name);
if(error == -1)
{ cout<<"Error: Invaild File Name \n";
  return 0;}

rpt.optimize();

//rpt.print_xy();
error = rpt.xy_to_gcode();
if(error == -1)
{return 0;}

if(output_file_true)
{
rpt.gcode_output_to_file(output_file_name);
}

cout<<"Finished Processing"<<endl;

if(print_true)
{
rpt.gcode_print_to_screen();
}

}



return 0;

}

void print_help()
{
  std::cout << "Kicad_CNC utility library " << '\n';

  std::cout << "\n\nUsage: kicadc_cnc inputfile outputfile.gcode\n";

  std::cout <<"\n\nThere is the option of adding the print command to the command line: \n\nkicad_cnc inputfile outputfile.gcode print \n\nthis will print the gcode to the screen. \n\n\n";
}






//EOF
