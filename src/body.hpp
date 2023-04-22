#pragma once
#include "vec.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <ostream>
#include <random>

struct Body {
  Body() {}
  Body(const double x, const double y, const double dx, const double dy)
      : pos(x, y), vel(dx, dy), acc(0.0, 0.0), mass(1.0) {}

  inline void draw(SDL_Renderer* wRender, const double maxDim) const {
    SDL_Rect r;
    r.x = maxDim / 2 + pos[0] - 1;
    r.y = maxDim / 2 + pos[1] - 1;
    r.w = 2;
    r.h = 2;
    SDL_RenderFillRect(wRender, &r);
  }

  inline void update() {
    pos += vel * 0.001;
    vel += acc * 0.001;
  }

  inline void resetAcc() {
    acc[0] = 0.0;
    acc[1] = 0.0;
  }

  Vec<double> pos;
  Vec<double> vel;
  Vec<double> acc;
  double mass;
  double halfWidth;
};

inline std::ostream& operator<<(std::ostream& os, const Body& point) {
  os << "Pos = " << point.pos << ", ";
  os << "Vel = " << point.vel << ", ";
  os << "Acc = " << point.acc << "";
  return os;
}

inline Body generatePointReg(const double x,
                             const double y,
                             const double stdev) {
  std::random_device rd{};
  std::mt19937 gen{rd()};

  std::normal_distribution<> xDist{x, stdev};
  std::normal_distribution<> yDist{y, stdev};
  std::normal_distribution<> vel{0, 100};
  return Body(xDist(gen), yDist(gen), 0, 0);
}
