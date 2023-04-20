#pragma once
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
  }
  double pos[2];
  double vel[2];
  double force[2];
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
