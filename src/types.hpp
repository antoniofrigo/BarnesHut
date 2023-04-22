#pragma once
#include "body.hpp"
#include "vec.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>


enum class Quadrant {
  NE,
  NW,
  SW,
  SE,
  NONE,
};

const auto DIRECTIONS =
    std::vector<std::pair<int, int>>{{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};

struct Quad {
  Quad(const double& x, const double& y, const double& dimension_) {
    center = Vec<double>(x,y);
    dimension = dimension_;
  }

  inline void draw(SDL_Renderer* wRender, const double maxDim) const {
    for (int i = 0; i < 4; ++i) {
      const auto& d1 = DIRECTIONS[i];
      const auto& d2 = DIRECTIONS[(i + 1) % 4];
      const auto& startX = (maxDim / 2 + center[0]) + d1.first * dimension;
      const auto& startY = (maxDim / 2 + center[1]) + d1.second * dimension;
      const auto& endX = (maxDim / 2 + center[0]) + d2.first * dimension;
      const auto& endY = (maxDim / 2 + center[1]) + d2.second * dimension;
      SDL_RenderDrawLine(wRender, startX, startY, endX, endY);
    }
  }

  inline bool isWithinQuad(const Body& body) const {
    return (abs(body.pos[0] - center[0]) <= dimension) &&
           (abs(body.pos[1] - center[1]) <= dimension);
  }

  inline Quad getQuad(const Quadrant& q) const {
    switch (q) {
    case Quadrant::NE:
      return Quad(center[0] + dimension / 2,
                  center[1] + dimension / 2,
                  dimension / 2);
    case Quadrant::NW:
      return Quad(center[0] - dimension / 2,
                  center[1] + dimension / 2,
                  dimension / 2);
    case Quadrant::SW:
      return Quad(center[0] - dimension / 2,
                  center[1] - dimension / 2,
                  dimension / 2);
    case Quadrant::SE:
      return Quad(center[0] + dimension / 2,
                  center[1] - dimension / 2,
                  dimension / 2);
    default:
      break;
    }
    // Should never run
    return Quad(0, 0, 0);
  }
  
  Vec<double> center;
  // std::pair<double, double> center;
  double dimension;
};

typedef std::pair<double, double> Coord;