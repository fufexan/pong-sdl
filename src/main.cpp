#include "circle.h"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>

// define window dimensions
constexpr int WINDOW_WIDTH = 854;
constexpr int WINDOW_HEIGHT = 480;

// define SDL Window related variables
SDL_Window *window = NULL;
SDL_Renderer *windowRenderer = NULL;
SDL_Event currentEvent;
int windowh, windoww;

// shapes
SDL_Rect rectangle;
Circle circle;

// render
bool quit = false;
bool goingRight = false;
bool animationRunning = true;
bool isFullScreen = false;

float elapsedTime;
float elapsedMS;

// input
int mouseX, mouseY;
// glm::vec2 mousePos, oldMousePos;

// circle
constexpr float circleSpeed = 0.5f;
glm::vec2 movementDirection;

// time derivative
constexpr float dt = 0.15f;
float posX, posY;

// functions
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
      windowRenderer = SDL_CreateRenderer(window, -1, 0);

      if (windowRenderer == NULL) {
        std::cout << "Failed to create renderer: " << SDL_GetError()
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
  SDL_GetRendererOutputSize(windowRenderer, &windoww, &windowh);
  // Update the surface
  SDL_UpdateWindowSurface(window);

  // Check for events in queue
  SDL_PollEvent(&currentEvent);

  // User requests quit
  if (currentEvent.type == SDL_QUIT) {
    quit = true;
  }

  // unneeded functionality commented out

  // Mouse event -> pressed button
  /* if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {
    if (currentEvent.button.button == SDL_BUTTON_LEFT) {
      SDL_GetMouseState(&mouseX, &mouseY);
    }
    if (currentEvent.button.button == SDL_BUTTON_RIGHT) {
      SDL_GetMouseState(&mouseX, &mouseY);
    }
  } */

  // Mouse event -> mouse movement
  /* if (currentEvent.type == SDL_MOUSEMOTION)
       SDL_GetMouseState(&mouseX, &mouseY); */

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
      animationRunning = !animationRunning;
      break;

    case SDLK_F11:
      if (!isFullScreen)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
      else
        SDL_SetWindowFullscreen(window, 0);
      isFullScreen = !isFullScreen;

      initObjects();

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
  int eq = dt * direction * 300;

  if (rectangle.y + eq > 0 && rectangle.y + rectangle.h + eq < windowh &&
      animationRunning) {
    rectangle.y += eq;
    std::cout << "w: " << windoww << " x: " << rectangle.x
              << " h: " << windowh << " y: " << rectangle.y << std::endl;
  }
}

void animationStep() {
  // ball hits paddle
  if (circle.center.y + circle.radius >= rectangle.y &&
      circle.center.y - circle.radius <= rectangle.y + rectangle.h &&
      circle.center.x + circle.radius <= rectangle.x + rectangle.w)
    posX = 1.f;
  // ball hits behind paddle
  else if (circle.center.x + circle.radius < rectangle.x + rectangle.w) {
    animationRunning = false;
    std::cout << "\nGame over! Score: \n";
  }

  // check wall conditions
  if (circle.center.x + circle.radius >= windoww)
    posX = -1.f;
  else if (circle.center.x - circle.radius <= 0)
    posX = 1.f;
  else if (circle.center.y + circle.radius >= windowh)
    posY = -1.f;
  else if (circle.center.y - circle.radius <= 0)
    posY = 1.f;

  glm::vec2 newPos = glm::vec2(posX, posY);
  movementDirection = normalize(newPos);

  circle.center += movementDirection * circleSpeed * dt * elapsedTime;

  SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
  // SDL_RenderDrawLine(windowRenderer, circle.center.x, circle.center.y,
  //                    mousePos.x, mousePos.y);
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
  SDL_FillCircle(windowRenderer, circle);

  if (animationRunning)
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
  // init rectangle
  rectangle.w = 30;
  rectangle.h = 200;
  rectangle.x = 10;
  rectangle.y = windowh / 2;

  // init cerc
  circle.radius = 20.0f;
  circle.center.x = windoww / 2.0f;
  circle.center.y = windowh / 2.0f;
  circle.color = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);

  // rng for positions
  posX = std::rand() % 3 - 1;
  posY = std::rand() % 3 - 1;

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