#ifndef write_gcode_h
#define write_gcode_h


class gcode_xyzabc_data_point{

  double x;
  double y;
  double z;

  double a;
  double b;
  double c;

  double size; // drill/tool/etc.
  double extruder_amount; // for 3D printer
  double solder_paste_dispenser; // for solder paste dispenser
  double pick_place_head_size;
};


class xy_to_gcode{

public:
  std::vector<gcode_xyzabc_data_point> gcode_points;

  void input_data();
  void set_offset(double x, double y, double z);

};


#endif
