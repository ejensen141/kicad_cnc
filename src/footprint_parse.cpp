#include <iostream>
#include "footprint_parse.hh"
#include <algorithm>
#include <math.h>
//#include <stringstream>

using namespace std;

//??##################EXPERIMENTAL!!!!!!!!!!!!!!!!!!!!####???//
void kicad_footprint_report::optimize()
{

  std::vector<pads> unordered_list;
  std::vector<pads> ordered_list;
  pads closest_pad;
  pads readpad;
  unordered_list = list_of_pads;


  int j = 0;

  double current_x=0;
  double current_y=0;
  double r = 0;
  double smallest_r;

  int closest_pad_number = 0;

  // find closest to 0,0
  //pads readpad = unordered_list[0];
  smallest_r =1000;// sqrt(readpad.x*readpad.x + readpad.y*readpad.y);
  int array_end = unordered_list.size();


  int n = 0;
do{
  j = 0;
  smallest_r = 1000;
  // cross reference all pads to current position
  do{
   readpad = unordered_list[j];

  r = sqrt((readpad.x-current_x)*(readpad.x-current_x) + (readpad.y-current_y)*(readpad.y-current_y));
  //cout<<r<<endl;

  if(r<smallest_r)
  {
    smallest_r = r;
    closest_pad_number=j;
  }

  j++;
  }while(j<array_end);
/// END cross reference


//  cout<<closest_pad_number<<" "<<current_x<<"  "<<current_y<<"  "<<smallest_r<<endl;
  closest_pad= unordered_list[closest_pad_number];
  ordered_list.push_back(closest_pad);
  current_x = closest_pad.x;
  current_y = closest_pad.y;


  //move that pad to the end of the unordered_list
  unordered_list.erase(unordered_list.begin()+closest_pad_number);
  array_end = unordered_list.size();
  // then the above loop will not see the ones it already searched

n++;
}while(n<total_number_of_pads);

list_of_pads = ordered_list; /// replace the list with ordered version

}
// this is a new function and is experimental!

int kicad_footprint_report::gcode_print_to_screen()
{

  std::cout<<gcode;
  return 0;
}

int kicad_footprint_report::gcode_output_to_file(string file_name)
{

  std::ofstream outputFile(file_name);
  if(!outputFile.good())
  {
    return -1;
  }

  outputFile<<gcode;

  return 0;
}



int kicad_footprint_report::xy_to_gcode()
{
  //std::stringstream ss;
  std::string gcode_output_string;

  if(parsed)
  {


    //cout<<"G20 \n G28 \n G01 F10  \n";
    gcode_output_string.append("G20\nG28\nG01 F10\n");

    double Z_retract = 0.1;
    double Z_feed = 1;
    double number;
    //readout the list;
    int j = 0;
    while(j<total_number_of_pads)
    {
      //pads readpad;
      pads readpad = list_of_pads[j];
  //  cout<<"G01 X"<<readpad.x<<" Y"<<readpad.y<<" Z"<<Z_retract<<endl;
    gcode_output_string.append("G01X");
    gcode_output_string.append(std::to_string(readpad.x));
    gcode_output_string.append("Y");
    gcode_output_string.append(std::to_string(readpad.y));
    gcode_output_string.append("\n");
    gcode_output_string.append("G01 Z0 F");
    gcode_output_string.append(std::to_string(Z_feed));
    gcode_output_string.append("\nG01 E1\n");
    gcode_output_string.append("G01 Z");
    gcode_output_string.append(std::to_string(Z_retract));
    gcode_output_string.append("\n");



  //  cout<<"G01 Z0 \n";
    //cout<<"G01 E1 \n";
  //  cout<<"G01 Z"<<Z_retract<<endl;



    j++;
    }

    //cout<<gcode_output_string;
    gcode = gcode_output_string;

  }
  else{
    return -1;
  }
}

int kicad_footprint_report::parse(string filename)
{

  total_number_of_pads = parse_pads_from_file(&list_of_pads,filename);

  if(total_number_of_pads ==-1)
  {
    parsed = false;
    return -1;
  }
  else{
    parsed = true;
  }

return total_number_of_pads;
}

void kicad_footprint_report::print_xy()
{
  if(parsed)
  {
    //readout the list;
    int j = 0;
    while(j<total_number_of_pads)
    {
      //pads readpad;
      pads readpad = list_of_pads[j];
    cout<< readpad.x<<" "<<readpad.y<<" orientation: "<<readpad.orientation<<endl;


    j++;
    }
  }

}


double kicad_footprint_report::parse_drill_line(string line)
{
  char *cstr_line = new char[line.length()+1];
  strcpy(cstr_line,line.c_str());
  std::string line_array[20];

  char *pch;
  pch = strtok(cstr_line," ");
  int i = 0;
  while(pch !=NULL)
  {
    pch = strtok(NULL," ");

    if(pch != NULL)
    {
    line_array[i] = pch;
  //  cout<<i << " "<<line_array[i]<<endl;
    }
    i++;
  }

  //std::cout<<line_array[0];

  return stod(line_array[0]);
}

int kicad_footprint_report::parse_position_line(string line,pads *pad) // returns 0 for pad 1 for part -1 for error
{
  char *cstr_line = new char[line.length() + 1];
  strcpy(cstr_line, line.c_str());

  std::string line_array[20];
  double x;
  double y;
  double area_x;
  double area_y;
  double area;


  int i =0;
  char *pch;


  pch = strtok(cstr_line," ");
  while (pch != NULL)
  {
  //  printf ("%s\n",pch);
    pch = strtok (NULL, " ");

    if(pch != NULL)
    {
    line_array[i] = pch;
  //  cout<<i << " "<<line_array[i]<<endl;
    }
    i++;
  }

  if(line_array[2] == "size") // then it is a pad
  {
    x = stod(line_array[0]);
    y = stod(line_array[1]);
    //cout<<"X,Y = "<< x<<", "<<y<<endl;

    area_x = stod(line_array[3]);
    area_y = stod(line_array[4]);
  //  cout<<"Area = "<< area_x*area_y<<endl;

    pad->x= x;
    pad->y=y;
    pad->area = area_x*area_y;
    pad->part = false;

    return 0;
  }
  else{
    x = stod(line_array[0]);
    y = stod(line_array[1]);
    pad->orientation = stod(line_array[3]);
  //  cout<<"X,Y = "<< x<<", "<<y<<endl;

    pad->x = x;
    pad->y = y;
    pad->part = true;



    return 1;
  }




}



//make function to parse file
int kicad_footprint_report::parse_pads_from_file(std::vector<pads> *pad_list, string file_name)
{

  pads pad;
  //std::vector<pads> pad_list;

  std::ifstream inputFile(file_name);
  if(!inputFile.good())
  {
    return -1;
  }

  //define variables
  std:string line;

  int part_number =0;
  int pad_number = 0;

  int line_number =0;
  std::size_t found;
  std::size_t found2;
  std::size_t founddrill;

  int data_type = -1;

  double current_part_x;
  double current_part_y;
  double current_part_orientation;
  bool checked_for_drill = false;


  //main loop
  do{
  //set the drill flag to false
  checked_for_drill=false;
  //read in a line
  getline(inputFile,line);
  // check if the line is a position data line //
  found = line.find("position");
  if(found!=std::string::npos)
  {
    //if the line is a position data, check to see if it is a part or a pad
    found2 = line.find("size");
    if(found2 == std::string::npos) // if part then set partnumber++
    { part_number++;
      //cout<<"PART "<<part_number<<"\n";
    }

    data_type= parse_position_line(line,&pad); // parse the line


    if(data_type == 0) // if it is a pad (data_type =0) then:
    {
              //cout<<"pad\n";
    double x;
    double y; // intermediate variables

    x = pad.x;
    y = pad.y;

              //do a tranformation based on the part angles.
              //cout<<"x: "<<pad.x + current_part_x << " y: "<<pad.y + current_part_y <<endl;
    pad.x = (x*cos( pad.orientation*(PI/180)) + y*sin(pad.orientation*(PI/180)) )+current_part_x;
    pad.y = (-1*x*sin( pad.orientation*(PI/180)) + y*cos(pad.orientation*(PI/180)))+current_part_y;
              //pad_list.push_back(pad);
              //pad_number++;

    pad.y = -1*pad.y;

    }

    if(data_type == 1) // if the type is a part then set the current part xy pos.
    {
    //std::cout << "part" << part_number<< "  x:"<<pad.x<<" y:"<<pad.y<< '\n';
    current_part_x = pad.x;
    current_part_y = pad.y;
    current_part_orientation = pad.orientation;
    }

  //cout<<line<<" "<<found<<endl;
  }

  /// END POSITION DATA LINE


  /// check and see if there is drill data
  bool is_drill = false;
  double drill;
  founddrill = line.find("drill");
  if(founddrill!=std::string::npos)
  {
  //parse drill line;
  drill = parse_drill_line(line);
  if(drill>0)
  {
    is_drill = true;
  }
  else{
    is_drill = false;
  }
  checked_for_drill=true;
  }


  // add pad to list if it is an smd pad
  if(!is_drill && data_type == 0 && checked_for_drill)
  {
  pad_list->push_back(pad);
  pad_number++;

  }


  line_number++;
  }while(!inputFile.eof());

  inputFile.close();

  int number_of_parts = part_number;
  int number_of_pads = pad_number;


  return number_of_pads;
}
