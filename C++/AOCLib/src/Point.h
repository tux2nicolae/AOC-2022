#pragma once

/**
 * Advent of code 2022
 * @author : Nicolae Telechi
 */
namespace AOC
{
struct Point
{
  bool operator==(const Point & second) const;
  bool operator!=(const Point & second) const;
  bool operator<(const Point & second) const;

  Point   operator+(const Point & second) const;
  Point & operator+=(const Point & second);

  Point   operator-(const Point & second) const;
  Point & operator-=(const Point & second);

  long long &       operator[](size_t pos);
  const long long & operator[](size_t pos) const;

  bool IsInBoundary(const Point & from, const Point & to) const;

  Point GetLeft() const;
  Point GetRight() const;
  Point GetTop() const;
  Point GetBottom() const;

  Point GetTopLeft() const;
  Point GetTopRight() const;
  Point GetBottomLeft() const;
  Point GetBottomRight() const;

  Point GetNeighbour(const char direction);
  Point GetNeighbour(const string & direction);

  vector<Point> GetDirect2DNeighbours() const;
  vector<Point> GetDirect3DNeighbours() const;
  vector<Point> GetAll2DNeighbours() const;
  vector<Point> GetAll3DNeighbours() const;
  vector<Point> GetAll4DNeighbours() const;

  void RotateRight(const Point & origin = { 0, 0 });
  void RotateLeft(const Point & origin = { 0, 0 });

  void FlipX();
  void FlipY();
  void FlipZ();

  long long x{ 0 };
  long long y{ 0 };
  long long z{ 0 };
  long long w{ 0 };
};

template <typename T = long long, typename... Args>
Point MakePoint(Args... args)
{
  return Point{ static_cast<T>(args)... };
};
}  // namespace AOC
