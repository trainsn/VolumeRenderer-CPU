////////////////////////////////////////////////////////
//
//   Han-Wei Shen 
//   01/25/1996
//

#ifndef RENDER_AUX_H
#define RENDER_AUX_H

float Dot(uvw *v1, uvw *v2); 

void Normalize(uvw *v); 

float clamp(float value, float min, float max); 

float lerp(float t, float y0, float y1); 

float ipow( float x, int n); 

#endif
