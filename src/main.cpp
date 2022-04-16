#include "circle.h"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>

// define window dimensions
const int WINDOW_WIDTH = 1800;
const int WINDOW_HEIGHT = 1000;

// define SDL Window related variables
SDL_Window *window = NULL;
SDL_Renderer *windowRenderer = NULL;
SDL_Event currentEvent;

// shapes
SDL_Rect rectangle;
Circle cerc;

// render
bool quit = false;
bool goingRight = false;
bool animationRuning = true;
bool isFullScreen = false;

float elapsedTime;
float elapsedMS;

// input
int *windowh, *windoww;
int mouseX, mouseY;
glm::vec2 mousePos, oldMousePos;

// circle
float circleSpeed = 0.5f;
glm::vec2 movementDirection;
float dt = 0.1f;
float posX, posY;

void moveRect(float direction);
void initObjects();

bool initWindow() {

  bool success = true;

  // Try to initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {

    std::cout << "SDL initialization failed" << std::endl;
    success = false;

  } else {

    // Try to create the window
    window = SDL_CreateWindow(
        "Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
        WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == NULL) {

      std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
      success = false;

    } else {

      // Create a renderer for the current window
      windowRenderer = SDL_CreateRenderer(
          window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

      if (windowRenderer == NULL) {

        std::cout << "Failed to create the renderer: " << SDL_GetError()
                  << std::endl;
        success = false;

      } else {

        // Set background color
        SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);

        // Apply background color
        SDL_RenderClear(windowRenderer);
      }
    }
  }

  return success;
}

void processEvents() {
  // Check for events in queue
  SDL_PollEvent(&currentEvent);

  // User requests quit
  if (currentEvent.type == SDL_QUIT) {
    quit = true;
  }

  // Mouse event -> pressed button
  if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {
    if (currentEvent.button.button == SDL_BUTTON_LEFT) {
      SDL_GetMouseState(&mouseX, &mouseY);
    }
    if (currentEvent.button.button == SDL_BUTTON_RIGHT) {
      SDL_GetMouseState(&mouseX, &mouseY);
    }
  }

  // Mouse event -> mouse movement
  if (currentEvent.type == SDL_MOUSEMOTION)
    SDL_GetMouseState(&mouseX, &mouseY);

  // Keyboard event
  if (currentEvent.type == SDL_KEYDOWN) {

    switch (currentEvent.key.keysym.sym) {

    case SDLK_UP:
      moveRect(-1.0f);
      break;

    case SDLK_DOWN:
      moveRect(1.0f);
      break;

    case SDLK_LEFT:
      break;

    case SDLK_RIGHT:
      break;

    case SDLK_r:
      initObjects();
      break;

    case SDLK_s:
      animationRuning = !animationRuning;
      break;

    case SDLK_F11:
      if (!isFullScreen)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
      else
        SDL_SetWindowFullscreen(window, 0);
      isFullScreen = !isFullScreen;

      initObjects();
      // rectangle.x = SDL_GetWindowSurface(window)->w / 2 - rectangle.w / 2;
      // rectangle.y = SDL_GetWindowSurface(window)->h - rectangle.h;

      break;

    case SDLK_ESCAPE:

      quit = true;
      break;
    default:
      break;
    }
  }
}

void moveRect(float direction) {
  SDL_Surface *s = SDL_GetWindowSurface(window);
  int eq = dt * elapsedTime * direction;

  if (0 < rectangle.y + eq && rectangle.y + rectangle.h + eq < s->h)
    rectangle.y += eq;

  std::cout << "w: " << s->w << " x: " << rectangle.x << " h: " << s->h
            << " y: " << rectangle.y << std::endl;
}

void animationStep() {

  // check wall conditions
  int x = cerc.center.x + cerc.center.r;
  int y = cerc.center.y + cerc.center.r;
  SDL_Surface *s = SDL_GetWindowSurface(window);

  if (x >= s->w)
    posX = 1;
  if (x <= 0)
    posX = -1;
  if (y >= s->h)
    posY = 1;
  if (y <= 0)
    posY = -1;

  // check paddle
  if (y >= rectangle.y && y <= rectangle.y + rectangle.h &&
      x == rectangle.x + rectangle.w)

    posY = 1;

  glm::vec2 newPos = glm::vec2(posX, posY);
  movementDirection = glm::normalize(cerc.center + newPos);

  cerc.center += movementDirection * circleSpeed * dt * elapsedTime;

  SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(windowRenderer, cerc.center.x, cerc.center.y, mousePos.x,
                     mousePos.y);

  // ball hits behind paddle
  if (cerc.center.x + cerc.center.r < rectangle.x + rectangle.w) {
    animationRuning = false;
    std::cout << "\nGame over! Score: \n";
  }
}

void drawFrame() {
  Uint32 start = SDL_GetTicks();

  // clear the background
  SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);
  SDL_RenderClear(windowRenderer);

  // draw scene
  // TODO

  // draw rectangle
  SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
  SDL_RenderFillRect(windowRenderer, &rectangle);

  // draw circle
  // functions for circle are defined in "circle.h"
  SDL_FillCircle(windowRenderer, cerc);

  if (animationRuning)
    animationStep();

  // Update window
  SDL_RenderPresent(windowRenderer);

  Uint32 end = SDL_GetTicks();

  elapsedTime = (end - start) / 0.1f;
}

void cleanup() {

  // Destroy renderer
  if (windowRenderer) {
    SDL_DestroyRenderer(windowRenderer);
    windowRenderer = NULL;
  }

  // Destroy window
  if (window) {
    SDL_DestroyWindow(window);
    window = NULL;
  }

  // Quit SDL
  SDL_Quit();
}

void initObjects() {

  SDL_Surface *s = SDL_GetWindowSurface(window);

  // init rectangle
  rectangle.w = 30;
  rectangle.h = 200;
  rectangle.x = rectangle.w / 2;
  rectangle.y = s->h / 2;

  // init cerc
  cerc.radius = 20.0f;
  cerc.center.x = s->w / 2.0f;
  cerc.center.y = s->h / 2.0f;
  cerc.color = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);

  // rng for positions
  posX = std::rand() % 3 - 1, posY = rand() % 3 - 1;
  if (posX == 0)
    posX += -1;
  if (posY == 0)
    posY += 1;
}

int main() {

  // seed rng
  std::srand(time(NULL));

  // Initialize window
  if (!initWindow()) {
    std::cout << "Failed to initialize" << std::endl;
    return -1;
  }

  initObjects();

  // Game loop
  while (!quit) {
    processEvents();
    drawFrame();
  }

  cleanup();
  return 0;
}