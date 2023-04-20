#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include "types.hpp"
#include "body.hpp"
#include "tree.hpp"

// const int SCREEN_WIDTH = 800;
// const int SCREEN_HEIGHT = 600;

int main() {
  auto quad = Quad(0,0, 400.0);
  auto tree = QuadTree(quad);
  auto points = std::vector<std::unique_ptr<Body>>(1);
  for(auto& p: points) {
    tree.insert(generatePoint(0, 0, 100));
  }

  
  // if (SDL_Init(SDL_INIT_VIDEO) < 0) {
  //   std::cout << "SDL not working..." << SDL_GetError() << std::endl;
  // } else {
  //   SDL_CreateWindow("Barnes hut", SDL_WINDOWPOS_CENTERED,
  //                    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
  //                    SDL_WINDOW_SHOWN);

  //   bool quit = false;
  //   SDL_Event e;
  //   while (!quit) {
  //     while (SDL_PollEvent(&e)) {
  //       if (e.type == SDL_QUIT) {
  //         quit = true;
  //       }
  //     }
  //   }
  // }
}