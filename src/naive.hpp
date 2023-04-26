#pragma once
#include "body.hpp"
#include <cmath>
#include <vector>
#include <iostream>

struct NaiveNBody {
  NaiveNBody(std::vector<Body> bodies) : bodies_(bodies) {}

  void updateForces(const double & G) {
    for (auto& p : bodies_) {
      p.resetAcc();
      for (auto& q : bodies_) {
        if (&p != &q) {
          const auto x = q.pos[0] - p.pos[0];
          const auto y = q.pos[1] - p.pos[1];
          const auto r = std::max(sqrt(x * x + y * y), 1.0);
          const auto a = G * q.mass / (r * r);
          const auto cos_theta = x / r;
          const auto sin_theta = y / r;
          p.acc[0] += a * cos_theta;
          p.acc[1] += a * sin_theta;
        }
      }
    }
  }
  
  void update() {
    for (auto & p : bodies_){
      p.update();
    }
  }

  std::vector<Body> bodies_;
};