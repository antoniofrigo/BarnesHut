#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <ostream>
#include <random>

struct Body {
  Body() {}
  Body(const double x, const double y, const double dx, const double dy) {
    pos[0] = x;
    pos[1] = y;
    vel[0] = dx;
    vel[1] = dy;
    mass = 1.0;
  }

  inline void draw(SDL_Renderer* wRender, const double maxDim) const {
    SDL_Rect r;
    r.x = maxDim / 2 + pos[0] - 1;
    r.y = maxDim / 2 + pos[1] - 1;
    r.w = 2;
    r.h = 2;
    SDL_RenderFillRect(wRender, &r);
  }
  
  inline void updateForce(){

  }

  inline void update(){
    pos[0] += vel[0] * 0.001;
    pos[1] += vel[1] * 0.001;
  }

  double pos[2];
  double vel[2];
  double force[2];
  double mass;
};

inline std::ostream& operator<<(std::ostream& os, const Body& point) {
  os << "Pos = {" << point.pos[0] << ", " << point.pos[1] << "}, ";
  os << "Vel = {" << point.vel[0] << ", " << point.vel[1] << "}";
  return os;
}

inline std::unique_ptr<Body> generatePoint(const double x,
                                           const double y,
                                           const double stdev) {
  std::random_device rd{};
  std::mt19937 gen{rd()};

  std::normal_distribution<> xDist{x, stdev};
  std::normal_distribution<> yDist{y, stdev};
  std::normal_distribution<> vel{0, 1000};
  return std::make_unique<Body>(xDist(gen), yDist(gen), vel(gen), vel(gen));
}
