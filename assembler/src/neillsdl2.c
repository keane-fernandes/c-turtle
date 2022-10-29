#include "neillsdl2.h"

/* Do something standard everytime an error is trapped */
#define ON_ERROR(STR) fprintf(stderr, \
         "\n%s: %s\n", STR, SDL_GetError()); \
         SDL_Quit(); exit(1);

/* 
   Set up a simple (WIDTH, HEIGHT) window.
   Attempt to hide the renderer etc. from user. 
*/
void Neill_SDL_Init(SDL_Simplewin *sw)
{
   /* SDL_Init returns -1 if there is an error */
   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      ON_ERROR("Unable to initialize SDL");
   } 

   sw->finished = 0;
   sw->win= SDL_CreateWindow("Turtle Graphics",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WWIDTH, WHEIGHT,
                          SDL_WINDOW_SHOWN);
   if(sw->win == NULL){
      ON_ERROR("Unable to initialize SDL Window");
   }

/* 
   Change SOFTWARE -> SDL_RENDERER_SOFTWARE to SDL_RENDERER ACCELERATED
   if you have a graphics card i.e. are not using remote access 
*/
   sw->renderer = SDL_CreateRenderer(sw->win, -1,
                   SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
   if(sw->renderer == NULL){
      ON_ERROR("Unable to initialize SDL Renderer");
   }
   SDL_SetRenderDrawBlendMode(sw->renderer,SDL_BLENDMODE_BLEND);

   /* Create texture for display */
   sw->display = SDL_CreateTexture(sw->renderer, SDL_PIXELFORMAT_RGBA8888,
                   SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);
   if(sw->display == NULL){
      ON_ERROR("Unable to initialize SDL texture");
   }
   SDL_SetRenderTarget(sw->renderer, sw->display);

   /* Clear screen, & set draw colour to black */
   SDL_SetRenderDrawColor(sw->renderer, 0, 0, 0, 255);
   SDL_RenderClear(sw->renderer);

}

/* Housekeeping to do with the render buffer & updating the screen */
void Neill_SDL_UpdateScreen(SDL_Simplewin *sw)
{
   SDL_SetRenderTarget(sw->renderer, NULL);
   SDL_RenderCopy(sw->renderer, sw->display, NULL, NULL);
   SDL_RenderPresent(sw->renderer);
   SDL_SetRenderTarget(sw->renderer, sw->display);
}

/* Gobble all events & ignore most */
void Neill_SDL_Events(SDL_Simplewin *sw)
{
   SDL_Event event;
   while(SDL_PollEvent(&event)) 
   {      
       switch (event.type){
          case SDL_QUIT:
          case SDL_MOUSEBUTTONDOWN:
          case SDL_KEYDOWN:
             sw->finished = 1;
       }
    }
}

/* Trivial wrapper to avoid complexities of renderer & alpha channels */
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b)
{
   SDL_SetRenderDrawColor(sw->renderer, r, g, b, SDL_ALPHA_OPAQUE);
}
