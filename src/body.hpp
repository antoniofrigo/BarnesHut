#pragma once
#include "vec.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <ostream>
#include <random>

// Body with mass 1
struct Body {
  Body() {}
  Body(const double x,
       const double y,
       const double dx,
       const double dy,
       const double dt_)
      : pos(x, y), vel(dx, dy), acc(0.0, 0.0), mass(1.0), dt(dt_) {}

  inline void draw(SDL_Renderer* wRender, const double maxDim) const {
    SDL_Rect r;
    r.x = maxDim / 2 + pos[0] - 1;
    r.y = maxDim / 2 + pos[1] - 1;
    r.w = 2;
    r.h = 2;
    SDL_RenderFillRect(wRender, &r);
  }

  inline void update() {
    pos += vel * dt;
    vel += acc * dt;
  }

  inline void resetAcc() {
    acc[0] = 0.0;
    acc[1] = 0.0;
  }

  Vec<double> pos;
  Vec<double> vel;
  Vec<double> acc;
  double mass;
  double dt;
};

inline std::ostream& operator<<(std::ostream& os, const Body& point) {
  os << "Pos = " << point.pos << ", ";
  os << "Vel = " << point.vel << ", ";
  os << "Acc = " << point.acc << "";
  return os;
}

inline Body generatePointReg(const double x,
                             const double y,
                             const double maxR,
                             const double dt) {
  std::random_device rd{};
  std::mt19937 gen{rd()};

  std::uniform_real_distribution<> rDist{0, maxR};
  std::uniform_real_distribution<> tDist{0, 2 * 6.28};
  const auto r = rDist(gen);
  const auto theta = tDist(gen);
  return Body(x + r * cos(theta), y + r * sin(theta), 0, 0, dt);
}

inline std::vector<Body> generateRotatingDisk(int num, const double dt) {
  auto bodies = std::vector<Body>(num);
  for (auto& body : bodies) {
    body = generatePointReg(0, 0, 100, dt);
    const auto r = sqrt(body.pos[0] * body.pos[0] + body.pos[1] * body.pos[1]);
    const auto v =
        1 / 6.28 * sqrt(num / r); // Things seem to work better with the 6.28?
    body.vel[0] = -v * body.pos[1] / r;
    body.vel[1] = v * body.pos[0] / r;
  }
  return bodies;
}