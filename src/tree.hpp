#pragma once
#include "body.hpp"
#include "types.hpp"
#include <algorithm>
#include <array>
#include <memory>

struct QuadTree {
  QuadTree(const Quad& quad) : quad_(quad), isLeaf_(true) {}
  bool insert(const std::unique_ptr<Body> body);

  std::unique_ptr<Body> body_;
  Quad quad_;
  bool isLeaf_;

  std::unique_ptr<QuadTree> NE;
  std::unique_ptr<QuadTree> NW;
  std::unique_ptr<QuadTree> SW;
  std::unique_ptr<QuadTree> SE;
};