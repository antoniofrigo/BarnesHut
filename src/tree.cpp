#include "tree.hpp"
#include "body.hpp"
#include <iostream>
#include <memory>

bool Tree::isValid(const Body& body) { return quad_.isWithinQuad(body); }

void Tree::updateAndRenderChildren(SDL_Renderer* wRender) {
  SDL_SetRenderDrawColor(wRender, 0, 120, 255, 30);
  quad_.draw(wRender, 700.0);
  if (body_ != nullptr) {
    SDL_SetRenderDrawColor(wRender, 0, 255, 30, 130);
    body_->update();
    body_->draw(wRender, 700.0);
  }

  for (auto&& child : children_) {
    child->updateAndRenderChildren(wRender);
  }
}

bool Tree::insert(Body* body) {
  // Do nothing if the body's coordinates are not within the quad
  if (!isValid(*body)) {
    return false;
  }
  count_ += 1;

  // Update center of mass
  cm = cm * totalMass + body->pos * body->mass;
  totalMass += body->mass;
  cm = cm / totalMass;

  // Keep the node as a leaf and add the body
  if (body_ == nullptr && isLeaf_) {
    body_ = body;
    return true;
  }

  // Create the children if they don't exist
  if (isLeaf_ && children_.size() == 0) {
    children_.emplace_back(std::make_unique<Tree>(quad_.getQuad(Quadrant::NE)));
    children_.emplace_back(std::make_unique<Tree>(quad_.getQuad(Quadrant::NW)));
    children_.emplace_back(std::make_unique<Tree>(quad_.getQuad(Quadrant::SW)));
    children_.emplace_back(std::make_unique<Tree>(quad_.getQuad(Quadrant::SE)));
  }
  isLeaf_ = false;

  // Place old body if needed
  bool oldResult = false;
  if (body_ != nullptr) {
    for (auto& child : children_) {
      if (child->isValid(*body_)) {
        oldResult |= child->insert(body_);
      }
    }
    body_ = nullptr;
  }

  // Place the new body
  bool newResult = false;
  for (auto& child : children_) {
    if (child->isValid(*body)) {
      newResult |= child->insert(body);
    }
  }

  // Return whether or not everything worked
  return oldResult & newResult;
}