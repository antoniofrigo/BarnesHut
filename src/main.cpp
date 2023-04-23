#include "body.hpp"
#include "tree.hpp"
#include "quad.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

constexpr int SCREEN_WIDTH = 700;
constexpr int SCREEN_HEIGHT = 700;
constexpr double THETA = 1;
constexpr double G = 1;

// Recusively traverse the tree and update the acceleration for a given body
void traverseAndUpdateAcc(Tree* q, Body* body) {
  if (q == nullptr) {
    return;
  }
  if (body == q->body_) {
    return;
  }

  const auto x = q->cm[0] - body->pos[0];
  const auto y = q->cm[1] - body->pos[1];
  const auto r = std::max(sqrt(x * x + y * y), 1.0);
  const auto theta = 2 * q->quad_.dimension / r;
  // Update only if the center of mass satisifies the 
  // theta criteria or it is a leaf node with a body 
  if (theta <= THETA || (q->isLeaf_ && q->body_ != nullptr)) {
    const auto a = G * q->totalMass / (r * r);
    const auto cos_theta = x / r;
    const auto sin_theta = y / r;
    body->acc[0] += a * cos_theta;
    body->acc[1] += a * sin_theta;
  } else {
    // Recurse the child nodes
    for (auto&& child : q->children_) {
      traverseAndUpdateAcc(child.get(), body);
    }
  }
}

int main() {
  // Initialize initial conditions
  auto quad = Quad(0, 0, 300.0);
  auto bodies = generateRotatingDisk(10000);

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL not working..." << SDL_GetError() << std::endl;
  } else {
    // Window initialization + SDL
    auto w_window = SDL_CreateWindow("Barnes-Hut", SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                     SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    auto wRender = SDL_CreateRenderer(w_window, -1, SDL_RENDERER_SOFTWARE);
    bool quit = false;
    SDL_Event e;

    // Main loop
    while (!quit) {
      const auto start = std::chrono::system_clock::now();

      // Build tree
      auto tree = Tree(quad);
      for (auto& p : bodies) {
        tree.insert(&p);
      }

      // Update the forces
      for (auto& p : bodies) {
        p.resetAcc();
        traverseAndUpdateAcc(&tree, &p);
      }
      const auto end = std::chrono::system_clock::now();

      // Update the positions and render the result (excluded from timing)
      tree.updateAndRenderChildren(wRender);

      // Print timing
      const auto durationMs =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              .count();
      std::cout << durationMs << " ms per loop (excluding rendering)"
                << "\r" << std::flush;
      SDL_RenderPresent(wRender);

      // Handle quit events
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          quit = true;
        }
      }

      // Have consistent framerate if 0 < durationMs < 16ms
      SDL_Delay(std::max(16.0 - durationMs, 0.0));
      // Clear the window
      SDL_SetRenderDrawColor(wRender, 0, 0, 0, 0);
      SDL_RenderClear(wRender);
    }
  }
}