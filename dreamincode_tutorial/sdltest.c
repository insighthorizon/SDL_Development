/*
	SDL DEMO - SOME DOTS moving on the screen.
	stolen from Sayuri L. Kitsune (2012)
*/

/* Includes */
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

/* Defines */
#define NUM_DOTS 1024

/* Types */
typedef struct
{
  int red,green,blue; /* The shade of color in red, green, blue. Blue should be always 0 for this demo */
  float vx,vy; /* Speed the particle is moving */
  float x,y; /* Current position of particle */
}dot_t;

/* Globals */
SDL_Surface *demo_screen;
int active;
SDL_Event ev = {0};
float demo_time_measure[2] = {0.0f};
float demo_time_step = 0.0f;
dot_t demo_dots[NUM_DOTS];
int test_cntr = 0;
float test_time = 0.0f;

/* Function declarations */
float demo_convert_time(struct timespec *ts);
void demo_start_time();
void demo_end_time();
float demo_roll();
void demo_init();
void demo_handle();
void demo_draw();

/* Main */
int main(int argn,char **argv)
{
	/* Init */
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		fprintf(stderr,"Could not initialize SDL: %s\n",SDL_GetError());
	/* Error? */
	printf("SDL Initialized\n");

	/* Initialize SDL */
	/* Open main window */
	demo_screen = SDL_SetVideoMode(320,240,0,SDL_HWSURFACE|SDL_DOUBLEBUF);
	if(!demo_screen) /* Check for null pointer */
	  fprintf(stderr,"Could not set video mode: %s\n",SDL_GetError());
	/* Clear screen (fill with blue) */
	SDL_FillRect(demo_screen, NULL, SDL_MapRGBA(demo_screen->format,0,0,255,255));  
	/* Initialize game */
	demo_init();
	
	/* Main loop */
	active = 1;
	demo_start_time();
	while(active)
	{	  
	  /* Handle events */
	  while(SDL_PollEvent(&ev))
	  {
	     if(ev.type == SDL_QUIT)
	      active = 0; /* End */
	  }
	  
	  /* Clear screen */
	  SDL_FillRect(demo_screen, NULL, SDL_MapRGBA(demo_screen->format,0,0,255,255));
	   /* Measure time step */
	  demo_end_time();
	  /* Handle dots - our game */
	  demo_handle();
	  /* Draw dosts */
	  demo_draw();
	  /* Show screen */
	  SDL_Flip(demo_screen);

	  test_cntr++;
	  test_time += demo_time_step;
	  if(test_cntr == 200)
	  {
	    printf("200 frames------%01.8f\n", test_time);
	      test_cntr = 0;
	      test_time = 0.0f;
	  }
	}
	
	/* Exit */
	SDL_Quit();
	printf("SDL Shutdown\n");
	/* Done */
	return 0;
}
/* end of Main */

/* Function definitions */
float demo_convert_time(struct timespec *ts)
{
  float accu;
  /* Combine the value into floating number */
  accu = (float)ts->tv_sec; /* Seconds that have gone by */
  accu *= 1000000000.0f; /* Nanoseconds */
  accu += (float)ts->tv_nsec;
  accu /= 1000000.0f; /* Bring it to millisecond range, but keep the nanosecond resolution */
  return accu;
}

/* Measure Start time of frame */
void demo_start_time()
{
  demo_time_measure[0] = SDL_GetTicks(); // demo_convert_time(&ts);
}

/* Measure time step from Start of frame */
void demo_end_time()
{
  long delta;
  demo_time_measure[1] = SDL_GetTicks();//demo_convert_time(&ts);
  delta = demo_time_measure[1] - demo_time_measure[0]; /* Find the distance in time */
  if(delta < 0) /* Check for SDL tick overflow */
  {
    demo_time_measure[1] = 0.0f;
    demo_time_measure[0] = 0.0f;
  }
  else /* For valid delta update time step */
  {
  demo_time_measure[0] = demo_time_measure[1]; /* Prepare for next step... */
  demo_time_step = 16.0f*delta/1000.0f; /* Equals 1.0f at 16 frames a second */
  /* 16 frames a second is a magic number. If my speed is 1, i move 16 units a second. */
  }
  //  printf("time step: %01.14f\n", demo_time_step);
}

/* Returns a random floating point number between 0.0f and 1.0f */
float demo_roll()
{
  float r;
  r = (float)(rand()%RAND_MAX); /* 0 ~ RAND_MAX */
  r /= (float)(RAND_MAX-1); /* one less than RAND_MAX makes it possible for 1.0 to happen */
  return r;
}

/* Initialize dots */
void demo_init()
{
  for(int i= 0; i < NUM_DOTS; i++)
  {
    demo_dots[i].red = rand()%255;
    demo_dots[i].green = rand()%255;
    demo_dots[i].blue = 0;
    demo_dots[i].vx = demo_roll()*16.0f - 8.0f;
    demo_dots[i].vy = demo_roll()*16.0f - 8.0f;
    demo_dots[i].x = demo_roll()*319.0f;
    demo_dots[i].y = demo_roll()*239.0f;
  }
}

/* Handle dots */
void demo_handle()
{
  for(int i = 0; i < NUM_DOTS; i++)
  {
    /* Move */
    demo_dots[i].x += demo_dots[i].vx*demo_time_step;
    demo_dots[i].y += demo_dots[i].vy*demo_time_step;
    /* Hit walls? */
    if(demo_dots[i].x < 0.0f || demo_dots[i].x >= 320.0f) /* Must be >= , cause we index from zero! */
    {
      /* Undo move (demo_time_step is still same value it was before and is valid for the current frame) */
      demo_dots[i].x -= demo_dots[i].vx*demo_time_step;
      /* Reverse */
      demo_dots[i].vx = -demo_dots[i].vx;
    }
  if(demo_dots[i].y < 0.0f || demo_dots[i].y >= 240.0f) /* Must be >= , cause we index from zero! */
    {
      /* Undo move (demo_time_step is still same value it was before and is valid for the current fram) */
      demo_dots[i].y -= demo_dots[i].vy*demo_time_step;
      /* Reverse */
      demo_dots[i].vy = - demo_dots[i].vy;
    }
  }
}

/* Draw dots */
void demo_draw()
{
  int bpp,rank,x,y;
  Uint32 *pixel;
  /* Lock surface */
  SDL_LockSurface(demo_screen);
  rank = demo_screen->pitch/sizeof(Uint32);
  pixel = (Uint32*)demo_screen->pixels;
  /* Draw all dots */
  for(int i = 0; i < NUM_DOTS; i++)
  {
    /* Rasterize position as integer */
    x = (int)demo_dots[i].x;
    y = (int)demo_dots[i].y;
    /* Set pixel */
    pixel[x+y*rank] = SDL_MapRGBA(demo_screen->format,demo_dots[i].red,demo_dots[i].green,demo_dots[i].blue,255);
  }
  /* Unlock surfrace */
  SDL_UnlockSurface(demo_screen);
}
