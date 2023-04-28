#include "body.hpp"
#include "config.hpp"
#include "naive.hpp"
#include "quad.hpp"
#include "tree.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

static Config config;

constexpr int SCREEN_WIDTH = 700;
constexpr int SCREEN_HEIGHT = 700;

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
  const auto theta = 2.0 * q->quad_.dimension / r;
  // Update only if the center of mass satisifies the
  // theta criteria or it is a leaf node with a body
  if (theta <= config.THETA || (q->isLeaf_ && q->body_ != nullptr)) {
    const auto a = config.G * q->totalMass / (r * r);
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

int main(int argc, const char* argv[]) {
  if (argc < 4) {
    std::cout << "Not enough arguments. THETA = [0, infinity), NUM = [1, "
                 "infinity), TIMESTEP = [0, infinity)]"
              << std::endl;
    std::cout << "A reasonable simulatation may be" << std::endl;
    std::cout << "\tbarneshut 0.3 1000 0.1" << std::endl;
    std::cout << "To also save the frame times, use" << std::endl;
    std::cout << "\tbarneshut 0.3 1000 0.1 --frames" << std::endl;
    return 0;
  }

  config.G = 1;
  config.THETA = std::stod(argv[1]);
  config.NUM_BODIES = std::stoi(argv[2]);
  config.DT = std::stod(argv[3]);
  config.SAVE_FRAMES = (argc == 5 && (argv[4] == std::string("--frames")));

  // Initialize initial conditions
  auto quad = Quad(0, 0, 300.0);
  auto bodies = generateRotatingDisk(config.NUM_BODIES, config.DT);

  // Compare the force updates with the Naive N-Body and verify
  // Only run on small numbers
  if (config.NUM_BODIES <= 10000){
    // Create Naive simulation
    auto bodies = generateRotatingDisk(config.NUM_BODIES, config.DT);
    auto naive = NaiveNBody(bodies); // Copy bodies
    naive.updateForces(config.G);

    // Create Barnes-Hut tree
    auto tree = Tree(quad);
    for (auto& p : bodies) {
      tree.insert(&p);
    }

    // Update the forces in the tree
    for (auto& p : bodies) {
      p.resetAcc();
      traverseAndUpdateAcc(&tree, &p);
    }

    // Find the sum of the differences
    double sumDiff = 0.0;
    for (size_t i = 0; i < bodies.size(); ++i) {
      const auto diff = bodies[i].acc - naive.bodies_[i].acc;
      sumDiff += abs(diff[0]) + abs(diff[1]);
    }
    std::cout << "Theta = " << config.THETA
              << ", Sum of difference between Naive and Barnes-Hut: " << sumDiff
              << std::endl;
  }

  std::ofstream frameTimes;
  if (config.SAVE_FRAMES) {
    auto thetaStr =  std::to_string(config.THETA);
    thetaStr = thetaStr.replace(thetaStr.find("."), 1, "_");
    frameTimes.open("theta_" + thetaStr + "_num_" + std::to_string(config.NUM_BODIES) +
                    "_frame_times.txt");
    frameTimes << "THETA = " << std::to_string(config.THETA) << std::endl;
    frameTimes << "NUM_BODIES = " << config.NUM_BODIES << std::endl;
    frameTimes << "DT = " << config.DT << std::endl;
  }

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
    int frame = 0;
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
      frameTimes << frame << ", " << durationMs << std::endl;
      std::cout << "Frame " << frame << " " << durationMs
                << " ms per loop (excluding rendering)"
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
      frame++;
    }
  }
  frameTimes.close();
}