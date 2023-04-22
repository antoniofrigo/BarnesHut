#include "body.hpp"
#include "tree.hpp"
#include "types.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <cmath>
#include <vector>

constexpr int SCREEN_WIDTH = 700;
constexpr int SCREEN_HEIGHT = 700;
constexpr double THETA = 0.0;
constexpr double G = 100000;

void traverseForces(Tree* q, Body* body){
  if (q == nullptr) {
    return;
  }
  if (body == q->body_) {
    return;
  }
  
  auto rSq = pow((body->pos[0] - q->cm[0]),2) + pow(body->pos[1] - q->cm[1], 2) + 0.001;
  auto thetaSq = 4 * pow(body->halfWidth,2)/rSq;
  if (thetaSq <= THETA * THETA || (q->isLeaf_ && q->body_ != nullptr)) {
    auto a = G * body->mass/rSq;
    auto cos_theta = (q->cm[0] - body->pos[0])/(sqrt(rSq));
    auto sin_theta = (q->cm[1] - body->pos[1])/(sqrt(rSq));
    body->acc[0] += a * cos_theta;
    body->acc[1] += a * sin_theta;
  } else {
    for (auto&& child : q->children_){
      traverseForces(child.get(), body);
    }
  }
}

int main() {
  auto quad = Quad(0, 0, 300.0);
  auto points = std::vector<Body>(100);
  for(auto& p: points){
    p = generatePointReg(0,0,50);
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL not working..." << SDL_GetError() << std::endl;
  } else {
    auto w_window = SDL_CreateWindow("Barnes-Hut", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                     SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    auto wRender = SDL_CreateRenderer(w_window, -1, SDL_RENDERER_SOFTWARE);
    bool quit = false;
    SDL_Event e;

    while (!quit) {
      auto tree = Tree(quad);
      for (auto& p: points){
        tree.insert(&p);
      }
      std::cout << tree.cm  << " " << tree.count << std::endl;
      for (auto& p: points){
        p.resetAcc();
        traverseForces(&tree, &p);
      }
      tree.updateAndRenderChildren(wRender);
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