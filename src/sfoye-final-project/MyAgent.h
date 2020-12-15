#pragma once

#include "MySprite.h"
#include "mathtool/Point.h"

//#include "MyAgent.h"
//#include "MyGame.h"
//#include <SDL_image.h>
//#include <SDL_ttf.h>
//#include <sstream>
#include "mathtool/Box.h"

namespace GMUCS425
{
    //this defines the transform of the agent
    class MyAgent
    {
      public:
		  /*
        //movable: true if this agent can move
        //collision: true if this agent can collision with other agents
        MyAgent(bool movable=true, bool collision=true)
        {
          x=y=degree=0;
          scale=1;
          visible=true;
          sprite=NULL;
          this->movable=movable;
          this->collision=collision;
        }
		*/
		//movable: true if this agent can move
		//bird/squirrel collision: true if this agent can collision with other agents
		MyAgent(bool movable = true, bool bcollision = true, bool scollision = true, bool collision = true)
		{
			x = y = degree = 0;
			scale = 1;
			visible = true;
			sprite = NULL;
			this->movable = movable;
			this->birdcollision = bcollision;
			this->squirrelcollision = scollision;
			this->collision = collision;
			this->isBird = false;
			this->isSquirrel = false;
			this->isAlive = true;
			this->isBad = false;
		}

        //react to the events
        virtual void handle_event(SDL_Event & e);

        //update this agent's motion, looks, sound, etc
        virtual void update();

        //render this agent
        virtual void display();

        //show HUD (heads-up display) or status bar
        virtual void draw_HUD();

        //transforms
        void rotate(float degree){ this->degree+=degree; }
        void rotateTo(float degree){ this->degree=degree; }

        void tranlate(float x, float y){ this->x+=x; this->y+=y; }
        void tranlateTo(float x, float y){ this->x=x; this->y=y; }

        void scaleTo(float s){ this->scale=s; }

        //display
        void show(){ visible=true; }
        void hide(){ visible=false; }

        //sprite, aks costume
        void setSprite(MySprite * sprite){ this->sprite=sprite; }
		MySprite* getSprite() { return this->sprite; }
		MySprite* getSprite() const { return this->sprite; }
		//float getWidth() const { return getSprite()->getWidth(scale); }
		//float getHeight() const { return getSprite()->getHeight(scale); }

        //motion/animation
        //void glide(float x, float y, float seconds);

        bool collide(MyAgent * other);
		bool MyAgent::collide(MyAgent * other) const
		{
			mathtool::Box2d box1, box2;
			box1.x = x;
			box1.y = y;
			box1.width = this->sprite->getWidth(scale);
			box1.height = this->sprite->getHeight(scale);

			box2.x = other->x;
			box2.y = other->y;
			box2.width = other->sprite->getWidth(other->scale);
			box2.height = other->sprite->getHeight(other->scale);

			return box1.intersect(box2);
		}

        //ACCESS METHODS
        bool is_movable() const { return movable; }
        bool is_visible() const { return visible; }

        float getX() const { return x; }
        float getY() const { return y; }
        float getAngle() const { return degree; }
        float getScale() const { return scale; }

      //protected:

        void draw_bounding_box();

        //current position and orientations
        float x,y;
        float degree; //rotation
        float scale;

    		bool visible;
    		bool movable;
    		bool collision;
			bool birdcollision;
			bool squirrelcollision;

			bool isBird;
			bool isSquirrel;
			bool isBad;
			bool isAlive;
        MySprite * sprite; //current sprite
        //it is possible that you can have more than one sprites
        //vector<MySprite *> sprites; //the sprites
    };

    class MyZombieAgent : public MyAgent
    {
      public:
        MyZombieAgent(bool movable=true, bool collision=true)
        :MyAgent(movable, false, collision){
			orig_x=INT_MAX; left=true; collide_with=NULL;
			this->isBad = true;
		}
        virtual void update();
        virtual void display();
        virtual void handle_event(SDL_Event & e);
      private:
        int orig_x;
        bool left;
        MyAgent * collide_with;
        int collision_free_timer=10;
    };


    class MyChickenAgent : public MyAgent
    {
      public:
        MyChickenAgent(bool movable=true, bool collision=true):MyAgent(movable,collision, false)
        {radius=FLT_MAX; center_x=center_y=INT_MAX; ccw=false; collide_with=NULL;
		this->isBad = true;
		}
        virtual void update();
        virtual void display();
        virtual void handle_event(SDL_Event & e);
      private:
        float radius;
        int center_x, center_y;
        bool ccw;
        MyAgent * collide_with;
        int collision_free_timer=10;
    };

	class MyNetAgent : public MyAgent
	{
		public:
			MyNetAgent(bool movable = false, bool collision = true) :MyAgent(false, true, false, true)
			{
				//radius = FLT_MAX; center_x = center_y = INT_MAX; ccw = false; collide_with = NULL;
			}
			//virtual void update();
			//virtual void display();
			//virtual void handle_event(SDL_Event & e);
		private:
			//float radius;
			//int center_x, center_y;
			//bool ccw;
			//MyAgent * collide_with;
			//int collision_free_timer = 10;
	};

	class MyPondAgent : public MyAgent
	{
	public:
		MyPondAgent(bool movable = false, bool collision = true) :MyAgent(false, false, true, true)
		{
			//radius = FLT_MAX; center_x = center_y = INT_MAX; ccw = false; collide_with = NULL;
		}
		//virtual void update();
		//virtual void display();
		//virtual void handle_event(SDL_Event & e);
	private:
		//float radius;
		//int center_x, center_y;
		//bool ccw;
		//MyAgent * collide_with;
		//int collision_free_timer = 10;
	};
}//end namespace
