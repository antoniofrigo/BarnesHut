#pragma once
#include "body.hpp"
#include "types.hpp"
#include <algorithm>
#include <array>
#include <memory>

struct QuadTree {
  QuadTree(const Quad& quad)
      : quad_(quad),
        isLeaf_(true),
        cm(std::make_pair(0.0, 0.0)),
        totalMass(0.0) {}

  bool isValid(const Body& body);
  bool insert(const std::unique_ptr<Body> body);

  std::unique_ptr<Body> body_;
  Quad quad_;
  bool isLeaf_;
  std::pair<double, double> cm;
  double totalMass;

  std::unique_ptr<QuadTree> NE;
  std::unique_ptr<QuadTree> NW;
  std::unique_ptr<QuadTree> SW;
  std::unique_ptr<QuadTree> SE;
};