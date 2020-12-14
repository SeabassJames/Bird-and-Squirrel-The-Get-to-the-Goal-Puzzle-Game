#include "MyAgent.h"
#include "MyGame.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
#include "mathtool/Box.h"

namespace GMUCS425
{

  void MyAgent::handle_event(SDL_Event & e)
  {
    if(this->movable)
    {
      //do nothing now...
    }
  }

  //update this agent's motion, looks, sound, etc
  void MyAgent::update()
  {
    //do nothing by default...
  }

  void MyAgent::display()
  {
    if(!this->visible) return; //not visible...
    //setup positions and ask sprite to draw something
    this->sprite->display(x, y, scale, degree);
  }

  //show HUD (heads-up display) or status bar
  void MyAgent::draw_HUD()
  {
    //draw nothing by defaut, your task to display the location of an agent
    //on the upper left corner
    //read http://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php
  }

  bool MyAgent::collide(MyAgent * other)
  {
    mathtool::Box2d box1, box2;
    box1.x=x;
    box1.y=y;
    box1.width=this->sprite->getWidth(scale);
    box1.height=this->sprite->getHeight(scale);

    box2.x=other->x;
    box2.y=other->y;
    box2.width=other->sprite->getWidth(other->scale);
    box2.height=other->sprite->getHeight(other->scale);

    return box1.intersect(box2) && ((this->birdcollision && other->birdcollision) || (this->squirrelcollision && other->squirrelcollision));
  }

  void MyAgent::draw_bounding_box()
  {
    float my_W=this->sprite->getWidth(scale);
    float my_H=this->sprite->getHeight(scale);

    SDL_Rect box; //create a rect
    box.x = x;  //controls the rect's x coordinate
    box.y = y; // controls the rect's y coordinte
    box.w = my_W; // controls the width of the rect
    box.h = my_H; // controls the height of the rect

    SDL_Renderer * renderer=getMyGame()->getRenderer();
    SDL_SetRenderDrawColor(renderer,255,100,0,100);
    //SDL_RenderDrawPoint(renderer, x, y);
    SDL_RenderDrawRect(renderer,&box);

    box.w/=10;
    box.h=box.w;
    SDL_SetRenderDrawColor(renderer,0,0,0,100);
    SDL_RenderFillRect(renderer,&box);
  }

  void MyZombieAgent::display()
  {
    if(!this->visible) return; //not visible...
    //setup positions and ask sprite to draw something
    this->sprite->display(x, y, scale, degree, NULL, this->left?SDL_FLIP_NONE:SDL_FLIP_HORIZONTAL);
    //draw_bounding_box();
  }

  void MyZombieAgent::update()
  {
    if(!this->collision)
    {
      if(collision_free_timer>=0) this->collision_free_timer--;
      else collide_with=NULL; //no collision
    }

    this->collision=false;

    if(orig_x==INT_MAX)
      orig_x=x;
    if(left) x-=2;
    else x+=2;
    if(x<orig_x-100) left=false;
    if(x>orig_x+100) left=true;
  }

  void MyZombieAgent::handle_event(SDL_Event & e)
  {
    //return;
    if(this->collision && collide_with!=NULL)
    {
      //std::cout<<"already in collision with "<<collide_with<<std::endl;

      return;
    }

    if(e.type==SDL_USEREVENT)
    {
      if(e.user.code == 1)
      {
        if(e.user.data1==this || e.user.data2==this)
        {
          MyAgent * other = (MyAgent *)((e.user.data1!=this)?e.user.data1:e.user.data2);

          if(other!=collide_with)
          {
            collide_with=other;
            left=!left;
          }
          this->collision_free_timer=10;
          this->collision=true;
        }
        //else collide_with=NULL; //no collision
      }
    }
  }

  void MyChickenAgent::display()
  {
    //MyAgent::display();
    if(!this->visible) return; //not visible...
    //setup positions and ask sprite to draw something
    this->sprite->display(x, y, scale, degree, NULL, this->ccw?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
    //draw_bounding_box();
  }


  void MyChickenAgent::handle_event(SDL_Event & e)
  {
    //std::cout<<"--------------------------------------"<<std::endl;
    if(this->collision && collide_with!=NULL)
    {
      //std::cout<<"already in collision with "<<collide_with<<std::endl;

      return;
    }
    //else
    //  std::cout<<"NO in collision: this->collision="<<this->collision<<" with "<<collide_with<<std::endl;

    if(e.type==SDL_USEREVENT)
    {
      if(e.user.code == 1)
      {
        if(e.user.data1==this || e.user.data2==this)
        {
          MyAgent * other = (MyAgent *)((e.user.data1!=this)?e.user.data1:e.user.data2);
          if(other!=this->collide_with)
          {
            //std::cout<<"changed to: "<<other<<" from " << collide_with<<std::endl;
            collide_with=other;
            ccw=!ccw;
          }
          this->collision_free_timer=10;
          this->collision=true;
        }//end if
      }//end if
    }//end if
  }

  void MyChickenAgent::update()
  {
    //std::cout<<"this->collision_free_timer="<<this->collision_free_timer<<std::endl;

    if(!this->collision)
    {
      if(collision_free_timer>=0) this->collision_free_timer--;
      else collide_with=NULL; //no collision
    }
    this->collision=false;

    if(radius==FLT_MAX)
    {
      radius=30;
      center_x=x; center_y=y-radius;
    }

    float angle=(ccw)?0.1:-0.1;
    float cos_t=cos(angle);
    float sin_t=sin(angle);

    float dx=x-center_x;
    float dy=y-center_y;
    float d=sqrt(dx*dx+dy*dy);
    dx=dx/d;
    dy=dy/d;

    x=(int)((cos_t*dx-sin_t*dy)*radius)+center_x;
    y=(int)((sin_t*dx+cos_t*dy)*radius)+center_y;
  }

}//end namespace
