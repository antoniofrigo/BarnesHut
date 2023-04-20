#pragma once
#include "body.hpp"
#include <algorithm>
#include <cmath>
#include <memory>

enum class Quadrant {
  NE,
  NW,
  SW,
  SE,
  NONE,
};

struct Quad {
  Quad(const double& x, const double& y, const double& dimension_) {
    center = std::make_pair(x, y);
    dimension = dimension_;
  }

  inline bool isWithinQuad(std::unique_ptr<Body> body) {
    return (abs(body->pos[0] - center.first) <= dimension) &&
           (abs(body->pos[1] - center.second) <= dimension);
  }

  inline Quad getQuad(const Quadrant& q) {
    switch (q) {
    case Quadrant::NE:
      return Quad(center.first + dimension / 2,
                  center.second + dimension / 2,
                  dimension / 2);
    case Quadrant::NW:
      return Quad(center.first - dimension / 2,
                  center.second + dimension / 2,
                  dimension / 2);
    case Quadrant::SW:
      return Quad(center.first - dimension / 2,
                  center.second - dimension / 2,
                  dimension / 2);
    case Quadrant::SE:
      return Quad(center.first + dimension / 2,
                  center.second - dimension / 2,
                  dimension / 2);
    default:
      break;
    }
    // Should never run
    return Quad(0, 0, 0);
  }

  std::pair<double, double> center;
  double dimension;
};

typedef std::pair<double, double> Coord;