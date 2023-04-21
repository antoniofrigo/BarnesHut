#include "body.hpp"
#include "tree.hpp"
#include "types.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

void traverse(QuadTree* q, SDL_Renderer* wRender) {
  if (q == nullptr) {
    return;
  }

  SDL_SetRenderDrawColor(wRender, 0, 120, 255, 30);
  q->quad_.draw(wRender, 700.0);
  if (q->body_ != nullptr) {
    SDL_SetRenderDrawColor(wRender, 0, 255, 30, 130);
    q->body_->draw(wRender, 700.0);
    q->body_->update();
  }
  traverse((q->NE).get(), wRender);
  traverse((q->NW).get(), wRender);
  traverse((q->SW).get(), wRender);
  traverse((q->SE).get(), wRender);
}

int main() {
  auto quad = Quad(0, 0, 300.0);
  auto points = std::vector<Body>(10000);
  for(auto& p: points){
    p = generatePointReg(0,0,50);
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL not working..." << SDL_GetError() << std::endl;
  } else {
    auto w_window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                     SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    auto wRender = SDL_CreateRenderer(w_window, -1, SDL_RENDERER_SOFTWARE);
    bool quit = false;
    SDL_Event e;

    while (!quit) {
      auto tree = QuadTree(quad);
      for (auto& p: points){
        tree.insert(&p);
      }
      traverse(&tree, wRender);
      SDL_RenderPresent(wRender);
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          quit = true;
        }
      }
      SDL_Delay(16);
      SDL_SetRenderDrawColor(wRender, 0, 0, 0, 0);
      SDL_RenderClear(wRender);
    }
  }
}