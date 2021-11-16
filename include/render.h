/////////////////////////////////////////////////////////////////////
//
//                       Volume Renderer Class
//                            Han-Wei Shen 
//                        hwshen@nas.nasa.gov 
//
//         MRJ Technology Solutions/NASA Ames Research Center 
//
#ifndef RENDER_H 
#define RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "image.h"
#include "Map.h"
#include "minmax.h"

#define EPS 1.0E-6

#define TRUE 1
#define FALSE 0

//#define Ka 0.5
//#define Kd 0.5

#define Ka 0.6
#define Kd 0.4

#define Ks 0.1
#define Alpha 2.0

//#define ambient_light 0.3
//#define light_strength 0.9
#define ambient_light  1
#define light_strength 5

static glm::vec4 eye_W(0.0f, 0.0f, 1.0f, 0.0f); 	/* eye vector */
static glm::vec4 light_W(0.40824829f, 0.40824829f, 0.816496f, 0.0f);  

void Image_to_File(image_type*, char*); 

///////////////////////////////////////////////////////
//
//  Used by those vectors such as gradient, light, etc. 
//
typedef struct _uvw
{
  float u,v,w; 
} uvw; 

///////////////////////////////////////////////////////
//
// This structure saves some state shared 
// between get_value and get_normal. This 
// makes get_normal faster since it can reuse 
// some of the work done by get_value.
//
struct interpolation_state
{
  unsigned long offsets[8]; 
  float tx,ty,tz; 
}; 

union VolumePtr {
  float* fVolume; 
}; 

//////////////////////////////////////////////////////
class volumeRender {
public: 

  enum VolumeType {
    RAW    = 1
  }; 

protected:

  VolumePtr vptr; 

  void get_bounds(); 
  void update_bounds(glm::vec4 q); 

  uvw  *gradient;           // volume gradient
  int has_gradient; 
				 
  Map  *map;                // color map

  float* lookup; 
  int lookupSize; 

  void set_color_map(char* mapname);  // old color map file
  void set_color_map(Map*);           // old color map file

  // viewing bounding box  (data space)
  int vxmin, vxmax, vymin, vymax, vzmin, vzmax; 				 
  int vxdim, vydim, vzdim; 

  // actual in core data bounding box (data space)
  int lxmin, lxmax, lymin, lymax, lzmin, lzmax; 				 
  int lxdim, lydim, lzdim, lxdimlydim; 

  // rendering range, i.e.  bounding box (data space)
  int rxmin, rxmax, rymin, rymax, rzmin, rzmax; 				 
  int rxdim, rydim, rzdim; 

  int udim, vdim;           // image dimensions

  // image bound 
  int umin, umax, vmin, vmax, wmin, wmax; 

  uvw eye, light, h, eye_nonN;   // eye, light, and half vector
                            // in the data coordinate system

  glm::mat4 screen_to_data;    // mapping between different 
  glm::mat4 data_to_view;     // coordinate systems
  glm::mat4 view_to_data; 

  int UNIFORM_FLAG; 
  float UNIFORM_VAL; 

  int get_value(glm::vec4 p, float*,
                interpolation_state*); 

  int get_normal(uvw*, interpolation_state*); 

  void local_lighting(glm::vec4, interpolation_state*, 
                      float obj_color[4], float result[3]); 

  void local_lighting(glm::vec4, 
                      float obj_color[4], float result[3]); 

  void depth_lighting(float*, interpolation_state*, 
                      float obj_color[4], float result[3]); 

  void compute_gradient(float*, int); // compute gradients for 
                                     // a slice of the data



  int mapLookup(float, float*); 

  int check_inbound(float[4]); 				     

  void update_transform(); 
  void update_viewing(); 

  void render();  // regular volume rendering 





  // set the range of data that will be actually accessible 
  // (i.e. in core) 
  //
  void set_data_and_bbx(int imin, int imax, int jmin, 
		    int jmax, int kmin, int kmax, 
		    void* data, 
                    int computeGrd = 1, uvw* grad = NULL); 

  // a simple setup routine that will call set_vewing_bbx(), 
  // set_data_and_bbx(), and set_clipping_bbx() using 
  // default parameters 
  //
  void set_volume_simple(int imin, int imax, int jmin, 
		  int jmax, int kmin, int kmax, 
		  void* volume); 



  void get_opacity(float, float*, 
		   interpolation_state*); 

public:

  volumeRender(int xdim, int ydim, int zdim, 
	       int udim, int vdim, 
	       void *volume); 

  ~volumeRender(); 

  void execute(int is_uniform = 0, 
	       float mean = 0.0 ); 

  int  readCmapFile(char *filename);

  glm::mat4 data_to_screen;    // transformation matrixes for 

  void setColorMap(int size, float* ctable); 

  // viewpoint 
  void set_view();

  void get_viewing_bbx(int& imin,int& imax,int& jmin,
		       int& jmax,int& kmin,int& kmax) {
       imin = vxmin; imax = vxmax;  jmin = vymin; jmax= vymax; 
       kmin = vzmin; kmax = vzmax; 				 
  }


  void set_clipping_bbx(int imin, int imax, int jmin, 
		      int jmax, int kmin, int kmax); 


  void out_to_image(char* fname);  // to be implemented 			
  //-----------------------------------------
  // following are just some query functions for the 
  // rendering setup -- not essential for performing 
  // the actual rendering 
  // 
  void get_view_vector(float&, float&, float&);  // in data space  
  void get_eye_vector(float&, float&, float&);  // in data space  

  float distance_to_viewplane(float, float, float); 

  void get_image_dims(int& usize, int& vsize) {
                      usize = udim; vsize = vdim; }

  // set the viewing bounding box in data space 
  //
  void set_viewing_bbx(int imin, int imax, int jmin, 
		       int jmax, int kmin, int kmax); 

  uvw* get_gradient() {return gradient;}

  void computeVolMinMax(float*, int, float &vol_min, float &vol_max);

  image_type *image;               // output image

}; 

#endif
