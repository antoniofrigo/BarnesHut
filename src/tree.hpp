#pragma once
#include "body.hpp"
#include "types.hpp"
#include "vec.hpp"
#include <algorithm>
#include <array>
#include <memory>
#include <ostream>

struct Tree {
  Tree(const Quad& quad)
      : quad_(quad),
        body_(nullptr),
        isLeaf_(true),
        cm(0.0, 0.0),
        totalMass(0.0),
        count(0) {}

  bool isValid(const Body& body);
  bool insert(Body* body);

  void getCMWithoutBody(const Body& body);
  void updateAndRenderChildren(SDL_Renderer* wRender);

  Quad quad_;
  Body* body_;
  bool isLeaf_;
  Vec<double> cm;
  double totalMass;
  int count;

  std::vector<std::unique_ptr<Tree>> children_;
};

inline std::ostream& operator<<(std::ostream& os, const Tree& tree) {
  os << "CM = " << tree.cm << " ";
  os << "MASS = " << tree.totalMass;
  return os;
}