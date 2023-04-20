#include <memory>
#include <iostream>
#include "tree.hpp"
#include "body.hpp"


bool QuadTree::insert(std::unique_ptr<Body> body){
  std::cout << body.get() << std::endl;
  if (!quad_.isWithinQuad(std::move(body))){
    return false;
  }
  
  if (body_ == nullptr && isLeaf_){
    body_ = std::move(body);
    return true;
  }
  
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
  
  return false;
}