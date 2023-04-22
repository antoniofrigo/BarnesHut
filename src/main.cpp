#include "body.hpp"
#include "tree.hpp"
#include "types.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

constexpr int SCREEN_WIDTH = 700;
constexpr int SCREEN_HEIGHT = 700;
constexpr double THETA = 1;
constexpr double G = 10000;

void traverseForces(Tree* q, Body* body) {
  if (q == nullptr) {
    return;
  }
  if (body == q->body_) {
    return;
  }

  const auto x = q->cm[0] - body->pos[0];
  const auto y = q->cm[1] - body->pos[1];
  const auto r = sqrt(x * x + y * y) + 0.01;
  const auto theta = 2 * q->quad_.dimension / r;
  if (theta <= THETA  || (q->isLeaf_ && q->body_ != nullptr)) {
    const auto a = G * q->totalMass / (r * r);
    const auto cos_theta = x / r;
    const auto sin_theta = y / r;
    body->acc[0] += a * cos_theta;
    body->acc[1] += a * sin_theta;
  } else {
    for (auto&& child : q->children_) {
      traverseForces(child.get(), body);
    }
  }
}

int main() {
  auto quad = Quad(0, 0, 300.0);
  auto bodies = std::vector<Body>(2);
  for (auto& p : bodies) {
    p = generatePointReg(0, 0, 50);
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
      for (auto& p : bodies) {
        tree.insert(&p);
      }

      for (auto& p : bodies) {
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