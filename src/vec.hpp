#pragma once
#include <iostream>
#include <ostream>

template <typename T_DATA>
struct Vec {
  Vec() {}
  Vec(const T_DATA& x, const T_DATA& y, const T_DATA& z) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }

  Vec(const double& x, const double& y) {
    data[0] = x;
    data[1] = y;
  }
  
  inline const T_DATA& operator[](const int& index) const{
    return data[index];
  }

  inline T_DATA& operator[](const int& index) {
    return data[index];
  }


  inline Vec& operator+=(const Vec& other) {
    data[0] += other.data[0];
    data[1] += other.data[1];
    data[2] += other.data[2];
    return *this;
  }

  inline Vec& operator-=(const Vec& other) {
    data[0] -= other.data[0];
    data[1] -= other.data[2];
    data[2] -= other.data[2];
    return *this;
  }


  inline Vec operator*(const Vec& other) {
    return Vec(data[0] * other.data[0],
               data[1] * other.data[1],
               data[2] * other.data[2]);
  }

  template <typename T_SCALAR>
  inline Vec operator*(const T_SCALAR& scalar) {
    return Vec(data[0] * scalar,
               data[1] * scalar,
               data[2] * scalar);
  }

  template <typename T_SCALAR>
  inline Vec operator/(const T_SCALAR& scalar) {
    return Vec(data[0] / scalar,
               data[1] / scalar,
               data[2] / scalar);
  }

  T_DATA data[3];
};


template <class T_DATA>
inline Vec<T_DATA> operator+(Vec<T_DATA> lhs, const Vec<T_DATA>& other) {
  lhs += other;
  return lhs;
}

template <class T_DATA>
inline Vec<T_DATA> operator-(Vec<T_DATA> lhs, const Vec<T_DATA>& other) {
  lhs -= other;
  return lhs;
}

template <class T_DATA>
std::ostream& operator<<(std::ostream& os, const Vec<T_DATA>& vec){
  os << "{ " << vec[0] << ", " << vec[1] <<", " << vec[2] << "}";
  return os;
}