#include "MyPerlin.h"
namespace GMUCS425
{

//creating perlin noise
float MyPerlin::noise(int h, int w, float CW, float CH, MyInterpolator * interpolator)
{
  int row=(int)(h/CH);
  int col=(int)(w/CW);

  float r_y=(h/CH)-row;
  float r_x=(w/CW)-col;

  row=row%mWidth;
  col=col%mHeight;

  mathtool::Vector2d& v0=mGradients[row][col];
  mathtool::Vector2d& v1=mGradients[row][(col+1)%mHeight];
  mathtool::Vector2d& v2=mGradients[(row+1)%mWidth][(col+1)%mHeight];
  mathtool::Vector2d& v3=mGradients[(row+1)%mWidth][col];

  mathtool::Vector2d u0=mathtool::Vector2d(r_x,r_y);
  mathtool::Vector2d u1=mathtool::Vector2d(r_x-1,r_y);
  mathtool::Vector2d u2=mathtool::Vector2d(r_x-1,r_y-1);
  mathtool::Vector2d u3=mathtool::Vector2d(r_x,r_y-1);

  float d0=v0*u0;
  float d1=v1*u1;
  float d2=v2*u2;
  float d3=v3*u3;

  float vx1=interpolator->interpolate(d0,d1,r_x);
  float vx2=interpolator->interpolate(d3,d2,r_x);
  float vy=interpolator->interpolate(vx1,vx2,r_y);

  return vy;
}

//TODO:
//
//generate perlin noise and store results in pixels
//the values should be between 0 and 255
//The argument "pixels" is pointing to an array of size = width*height*sizeof(Uint32)
void MyPerlin::generate(Uint32 * pixels, int width, int height, MyInterpolator * interpolator)
{
  float CW=width*1.0/(mWidth-1);
  float CH=height*1.0/(mHeight-1);

  //replace the following lines
  for(int h=0;h<height;h++)
  {
    for(int w=0;w<width;w++)
    {
      float vy=noise(h, w, CW, CH, interpolator);
      int v=(int)(255*(vy+1)/2);
      int id=h*width+w;
      pixels[id]=(v | v<<8 | v<<16);
    }
  }//end for h
}


//TODO:
//
//generate n copies of perlin noises controlled using the given persistence
//and store averaged results in pixels, the values should be between 0 and 255
//Recall that: frequency = 2^i and amplitude = persistence^i
//where i = 0 ~ n-1
//
void MyPerlin::generate(Uint32 * pixels, int width, int height, MyInterpolator * interpolator, float persistence, int n)
{
  using namespace std;

  float CW=width*1.0/(mWidth-1);
  float CH=height*1.0/(mHeight-1);
  
  //replace the following lines
  for(int h=0;h<height;h++)
  {
    for(int w=0;w<width;w++)
    {
      //frequency = 2^i and amplitude = persistence^i
      float vy=0;
      float freq=1;
      float amp=1;
      float total=0;
      for(int i=0;i<n;i++)
      {
        vy+=(amp*noise(h*freq, w*freq, CW, CH, interpolator));
        total+=amp;
        freq*=2;
        amp*=persistence;
      }

      int v=(int)(255*(vy/total+1)/2);

      int id=h*width+w;
      pixels[id]=(v | v<<8 | v<<16);
    }
  }//end for h

}

}//end namespace GMUCS425
