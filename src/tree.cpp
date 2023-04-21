#include <memory>
#include <iostream>
#include "tree.hpp"
#include "body.hpp"


bool QuadTree::isValid(const Body& body){
  return quad_.isWithinQuad(body);
}

bool QuadTree::insert(std::unique_ptr<Body> body){
  if (!isValid(*body)){
    return false;
  }
  
  cm.first = cm.first * totalMass + body->pos[0] * body->mass;
  cm.second = cm.second * totalMass + body->pos[1] * body->mass;
  totalMass += body->mass;
  cm.first /= totalMass;
  cm.second /= totalMass;
  
  // Set body to be the 
  if (body_ == nullptr && isLeaf_){
    body_ = std::move(body);
    return true;
  }
  
  // Create the children if they don't exist
  isLeaf_ = false;
  if (NE == nullptr) {
    NE = std::make_unique<QuadTree>(quad_.getQuad(Quadrant::NE));
  }
  if (NW == nullptr) {
    NW = std::make_unique<QuadTree>(quad_.getQuad(Quadrant::NW));
  }
  if (SW == nullptr) {
    SW = std::make_unique<QuadTree>(quad_.getQuad(Quadrant::SW));
  }
  if (SE == nullptr) {
    SE = std::make_unique<QuadTree>(quad_.getQuad(Quadrant::SE));
  }

  // Place old body
  bool result = true;
  if (body_ != nullptr) {
    if (NE->isValid(*body_)) {
      result &= NE->insert(std::move(body_));
    } else if (NW->isValid(*body_)) {
      result &= NW->insert(std::move(body_));
    } else if (SW->isValid(*body_)) {
      result &= SW->insert(std::move(body_));
    } else if (SE->isValid(*body_)) {
      result &= SE->insert(std::move(body_));
    }
  }
  
  // Place new one
  if (NE->isValid(*body)) {
    result &=  NE->insert(std::move(body));
  } else if (NW->isValid(*body)) {
    result &= NW->insert(std::move(body));
  } else if (SW->isValid(*body)) {
    result &= SW->insert(std::move(body));
  } else if (SE->isValid(*body)) {
    result &= SE->insert(std::move(body));
  }

  return result;
}