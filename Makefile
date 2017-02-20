

all:footprint_parse
	g++ main.cpp lib/footprint_parse.a -o kicad_cnc



footprint_parse:
	g++ -c src/footprint_parse.cpp -o lib/footprint_parse.a


write_gcode:
	gcc -c src/write_gcode.cpp -o lib/write_gcode.a




clean:
	rm *.a
