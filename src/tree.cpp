#include "tree.hpp"
#include "body.hpp"
#include <iostream>
#include <memory>

bool Tree::isValid(const Body& body) { return quad_.isWithinQuad(body); }

bool Tree::insert(Body* body) {
  if (!isValid(*body)) {
    return false;
  }
  body->halfWidth = quad_.dimension;

  cm = cm * totalMass + body->pos * body->mass;
  totalMass += body->mass;
  cm = cm / totalMass;

  // Set body to be the
  if (body_ == nullptr && isLeaf_) {
    body_ = body;
    return true;
  }

  // Create the children if they don't exist
  if (isLeaf_) {
    if (NE == nullptr) {
      NE = std::make_unique<Tree>(quad_.getQuad(Quadrant::NE));
    }
    if (NW == nullptr) {
      NW = std::make_unique<Tree>(quad_.getQuad(Quadrant::NW));
    }
    if (SW == nullptr) {
      SW = std::make_unique<Tree>(quad_.getQuad(Quadrant::SW));
    }
    if (SE == nullptr) {
      SE = std::make_unique<Tree>(quad_.getQuad(Quadrant::SE));
    }
  }
  isLeaf_ = false;

  // Place old body if needed
  bool result = true;
  if (body_ != nullptr) {
    if (NE->isValid(*body_)) {
      result &= NE->insert(body_);
    } else if (NW->isValid(*body_)) {
      result &= NW->insert(body_);
    } else if (SW->isValid(*body_)) {
      result &= SW->insert(body_);
    } else if (SE->isValid(*body_)) {
      result &= SE->insert(body_);
    }
    body_ = nullptr;
  }

  // Place new one
  if (NE->isValid(*body)) {
    result &= NE->insert(body);
  } else if (NW->isValid(*body)) {
    result &= NW->insert(body);
  } else if (SW->isValid(*body)) {
    result &= SW->insert(body);
  } else if (SE->isValid(*body)) {
    result &= SE->insert(body);
  }

  return result;
}