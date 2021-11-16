#ifndef MAP_H
#define MAP_H

// standard include files
#include <stdio.h>

#define TRUE 1
#define FALSE 0

// project include files

// local defines
#define MAP_TABLE_SIZE 256
#define MAP_CONST        1.0
#define MAP_LINEAR       2.0

// number of floats per entry (data,func,rgb values,opacity)
#define ITEMSPERENTRY       6

////////////////////////////////////////////////
//
//    A color map object class
//
class Map {

public:
  // the data values
  float data[MAP_TABLE_SIZE];
  // mode of transformation
  float func[MAP_TABLE_SIZE];
  // the map values
  float rvalue[MAP_TABLE_SIZE];
  float gvalue[MAP_TABLE_SIZE];
  float bvalue[MAP_TABLE_SIZE];
  float opacity[MAP_TABLE_SIZE];
  // the number of entries in the table
  int entries;
  // debugging message buffer
  char message[256];

  // adds a new range to the map
  int add(float value, float rgbo[4], int *pos);

  Map();

  Map(float min, float fnc1, float rgbo1[4],
      float max, float fnc2, float rgbo2[4]);

  ~Map(void);
  
  void read_file(char*);       // reads the map function from a file
  void write_file(char*);      // writes the map function to a file

  // looks up the rgb values for a data value; returns false if out of range
  int lookup(float value, float rgbo[4]);

  // adds a new range to the map map using a constant
  void add_constant(float value, float rgbo[4]);
  
  // adds a new range to the map map using a linear interp
  void add_linear(float value, float rgbo[4]);

  // delete an entry
  int delete_entry(float value);

  // change an entry
  int change_entry(float value, float fnc, float rgbo[4]);

};

#endif
