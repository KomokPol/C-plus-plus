#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <cmath>
class Vector;
class AbstractShape;
class Point;
class Segment;
class Ray;
class Line;
class Polygon;
class Circle;
class Vector {
  int a_;
  int b_;

 public:
  Vector() : a_(0), b_(0) {
  }
  Vector(int a, int b) : a_(a), b_(b) {
  }
  Vector(const Point& a, const Point& b);
  Vector operator+() const {
    return {a_, b_};
  }
  Vector operator-() const {
    return {-a_, -b_};
  }
  Vector operator+(const Vector& other) const {
    int new_a = a_ + other.a_;
    int new_b = b_ + other.b_;
    return {new_a, new_b};
  }
  Vector operator-(const Vector& other) const {
    int new_a = a_ - other.a_;
    int new_b = b_ - other.b_;
    return {new_a, new_b};
  }
  Vector operator*(int num) const {
    int new_a = a_ * num;
    int new_b = b_ * num;
    return {new_a, new_b};
  }
  Vector& operator+=(const Vector& other) {
    a_ += other.a_;
    b_ += other.b_;
    return *this;
  }
  Vector& operator-=(const Vector& other) {
    a_ -= other.a_;
    b_ -= other.b_;
    return *this;
  }
  Vector& operator*=(int num) {
    a_ *= num;
    b_ *= num;
    return *this;
  }
  double Length() {
    return std::sqrt(a_ * a_ + b_ * b_);
  }
  int GetA() const {
    return a_;
  }
  int GetB() const {
    return b_;
  }
};
class AbstractShape {
 public:
  virtual bool ContainsPoint(const Point& point) const = 0;
  virtual bool CrossSegment(const Segment& segment) const = 0;
  virtual AbstractShape* Clone() const = 0;
  virtual AbstractShape* Move(const Vector& vector) = 0;
  virtual void Print() const = 0;
  virtual ~AbstractShape() = default;
};
class Point : public AbstractShape {
  int x_;
  int y_;

 public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {
  }
  bool ContainsPoint(const Point& point) const override {
    return (x_ == point.x_ && y_ == point.y_);
  }
  bool CrossSegment(const Segment& segment) const override;
  AbstractShape* Clone() const override {
    return new Point(x_, y_);
  }
  AbstractShape* Move(const Vector& vector) override {
    int new_x = x_ + vector.GetA();
    int new_y = y_ + vector.GetB();
    return new Point(new_x, new_y);
  }
  void Print() const override {
    std::cout << x_ << " " << y_ << '\n';
  }
  int GetX() const {
    return x_;
  }
  int GetY() const {
    return y_;
  }
};
Vector::Vector(const Point& a, const Point& b) {
  a_ = b.GetX() - a.GetX();
  b_ = b.GetY() - a.GetY();
}
inline int SquareDistance(const Point& p1, const Point& p2) {
  int dx = p1.GetX() - p2.GetX();
  int dy = p1.GetY() - p2.GetY();
  return dx * dx + dy * dy;
}
class Segment : public AbstractShape {
  int ax_;
  int ay_;
  int bx_;
  int by_;

 public:
  Segment() = default;
  Segment(int x1, int y1, int x2, int y2) : ax_(x1), ay_(y1), bx_(x2), by_(y2) {
  }
  Segment(const Point& a, const Point& b) : Segment(a.GetX(), a.GetY(), b.GetX(), b.GetY()) {
  }
  bool ContainsPoint(const Point& point) const override {
    int px = point.GetX();
    int py = point.GetY();
    int cross = (px - ax_) * (by_ - ay_) - (py - ay_) * (bx_ - ax_);
    if (cross != 0) {
      return false;
    }
    if (px < std::min(ax_, bx_) || px > std::max(ax_, bx_)) {
      return false;
    }
    if (py < std::min(ay_, by_) || py > std::max(ay_, by_)) {
      return false;
    }
    return true;
  }
  bool CrossSegment(const Segment& seg) const override {
    auto orientation = [](int ax, int ay, int bx, int by, int cx, int cy) -> int {
      int val = (by - ay) * (cx - bx) - (bx - ax) * (cy - by);
      if (val == 0) {
        return 0;
      }
      return (val > 0) ? 1 : 2;
    };

    auto on_segment = [](int ax, int ay, int bx, int by, int cx, int cy) -> bool {
      return (bx >= std::min(ax, cx) && bx <= std::max(ax, cx) && by >= std::min(ay, cy) && by <= std::max(ay, cy));
    };

    int o1 = orientation(ax_, ay_, bx_, by_, seg.ax_, seg.ay_);
    int o2 = orientation(ax_, ay_, bx_, by_, seg.bx_, seg.by_);
    int o3 = orientation(seg.ax_, seg.ay_, seg.bx_, seg.by_, ax_, ay_);
    int o4 = orientation(seg.ax_, seg.ay_, seg.bx_, seg.by_, bx_, by_);

    if (o1 != o2 && o3 != o4) {
      return true;
    }
    if (o1 == 0 && on_segment(ax_, ay_, seg.ax_, seg.ay_, bx_, by_)) {
      return true;
    }
    if (o2 == 0 && on_segment(ax_, ay_, seg.bx_, seg.by_, bx_, by_)) {
      return true;
    }
    if (o3 == 0 && on_segment(seg.ax_, seg.ay_, ax_, ay_, seg.bx_, seg.by_)) {
      return true;
    }
    if (o4 == 0 && on_segment(seg.ax_, seg.ay_, bx_, by_, seg.bx_, seg.by_)) {
      return true;
    }
    return false;
  }
  AbstractShape* Clone() const override {
    return new Segment(ax_, ay_, bx_, by_);
  }
  AbstractShape* Move(const Vector& vector) override {
    int dx = vector.GetA();
    int dy = vector.GetB();
    return new Segment(ax_ + dx, ay_ + dy, bx_ + dx, by_ + dy);
  }
  void Print() const override {
    std::cout << ax_ << " " << ay_ << " " << bx_ << " " << by_ << '\n';
  }
  int GetX1() const {
    return ax_;
  }
  int GetY1() const {
    return ay_;
  }
  int GetX2() const {
    return bx_;
  }
  int GetY2() const {
    return by_;
  }
};
bool Point::CrossSegment(const Segment& segment) const {
  return segment.ContainsPoint(*this);
}
class Ray : public AbstractShape {
  Point start_;
  Point through_;

 public:
  Ray(int sx, int sy, int tx, int ty) : start_(sx, sy), through_(tx, ty) {
  }
  Ray(const Point& s, const Point& t) : start_(s), through_(t) {
  }
  bool ContainsPoint(const Point& point) const override {
    int vx = through_.GetX() - start_.GetX();
    int vy = through_.GetY() - start_.GetY();
    int wx = point.GetX() - start_.GetX();
    int wy = point.GetY() - start_.GetY();
    int cross = vx * wy - vy * wx;
    if (cross != 0) {
      return false;
    }
    int dot = vx * wx + vy * wy;
    return dot >= 0;
  }
  bool CrossSegment(const Segment& seg) const override {
    return ContainsPoint(Point(seg.GetX1(), seg.GetY1())) || ContainsPoint(Point(seg.GetX2(), seg.GetY2()));
  }
  AbstractShape* Clone() const override {
    return new Ray(start_.GetX(), start_.GetY(), through_.GetX(), through_.GetY());
  }
  AbstractShape* Move(const Vector& vector) override {
    int dx = vector.GetA();
    int dy = vector.GetB();
    return new Ray(start_.GetX() + dx, start_.GetY() + dy, through_.GetX() + dx, through_.GetY() + dy);
  }

  void Print() const override {
    std::cout << start_.GetX() << " " << start_.GetY() << " " << through_.GetX() << " " << through_.GetY() << '\n';
  }
};
class Line : public AbstractShape {
  Point p1_;
  Point p2_;

 public:
  Line(int x1, int y1, int x2, int y2) : p1_(x1, y1), p2_(x2, y2) {
  }
  Line(const Point& a, const Point& b) : p1_(a), p2_(b) {
  }
  bool ContainsPoint(const Point& point) const override {
    int vx = p2_.GetX() - p1_.GetX();
    int vy = p2_.GetY() - p1_.GetY();
    int wx = point.GetX() - p1_.GetX();
    int wy = point.GetY() - p1_.GetY();
    int cross = vx * wy - vy * wx;
    return cross == 0;
  }
  bool CrossSegment(const Segment& segment) const override {
    Point a(segment.GetX1(), segment.GetY1());
    Point b(segment.GetX2(), segment.GetY2());
    if (ContainsPoint(a) || ContainsPoint(b)) {
      return true;
    }
    auto side = [this](const Point& pt) -> int {
      int vx = p2_.GetX() - p1_.GetX();
      int vy = p2_.GetY() - p1_.GetY();
      int wx = pt.GetX() - p1_.GetX();
      int wy = pt.GetY() - p1_.GetY();
      int cp = vx * wy - vy * wx;
      return (cp > 0) ? 1 : ((cp < 0) ? -1 : 0);
    };
    int side_a = side(a);
    int side_b = side(b);
    return (side_a * side_b < 0);
  }
  AbstractShape* Clone() const override {
    return new Line(p1_.GetX(), p1_.GetY(), p2_.GetX(), p2_.GetY());
  }
  AbstractShape* Move(const Vector& vector) override {
    int dx = vector.GetA();
    int dy = vector.GetB();
    return new Line(p1_.GetX() + dx, p1_.GetY() + dy, p2_.GetX() + dx, p2_.GetY() + dy);
  }
  void Print() const override {
    std::cout << p1_.GetX() << " " << p1_.GetY() << " " << p2_.GetX() << " " << p2_.GetY() << '\n';
  }
};
class Polygon : public AbstractShape {
  std::vector<Point> vertices_;

 public:
  Polygon(const std::vector<Point>& vertices, size_t /* n_points */) : vertices_(vertices) {
  }
  bool ContainsPoint(const Point& point) const override {
    size_t n = vertices_.size();
    bool inside = false;
    for (size_t i = 0; i < n; ++i) {
      const Point& curr = vertices_[i];
      const Point& next = vertices_[(i + 1) % n];
      Segment edge(curr, next);
      if (edge.ContainsPoint(point)) {
        return true;
      }
      if (((curr.GetY() > point.GetY()) != (next.GetY() > point.GetY()))) {
        double intersect = static_cast<double>(next.GetX() - curr.GetX()) *
                               (static_cast<double>(point.GetY() - curr.GetY()) / (next.GetY() - curr.GetY())) +
                           curr.GetX();
        if (point.GetX() < intersect) {
          inside = !inside;
        }
      }
    }
    return inside;
  }
  bool CrossSegment(const Segment& segment) const override {
    size_t n = vertices_.size();
    for (size_t i = 0; i < n; ++i) {
      Segment edge(vertices_[i], vertices_[(i + 1) % n]);
      if (edge.CrossSegment(segment)) {
        return true;
      }
    }
    return false;
  }
  AbstractShape* Clone() const override {
    return new Polygon(vertices_, vertices_.size());
  }
  AbstractShape* Move(const Vector& vector) override {
    std::vector<Point> moved;
    moved.reserve(vertices_.size());
    for (const auto& vertex : vertices_) {
      moved.emplace_back(vertex.GetX() + vector.GetA(), vertex.GetY() + vector.GetB());
    }
    return new Polygon(moved, moved.size());
  }
  void Print() const override {
    std::cout << vertices_.size() << "\n";
    for (const auto& vertex : vertices_) {
      std::cout << vertex.GetX() << " " << vertex.GetY() << " ";
    }
    std::cout << '\n';
  }
};
class Circle : public AbstractShape {
  Point center_;
  int radius_;

 public:
  Circle(const Point& center, int radius) : center_(center), radius_(radius) {
  }
  bool ContainsPoint(const Point& point) const override {
    int dx = center_.GetX() - point.GetX();
    int dy = center_.GetY() - point.GetY();
    int dist = dx * dx + dy * dy;
    return dist <= radius_ * radius_;
  }
  bool CrossSegment(const Segment& segment) const override {
    Point p(segment.GetX1(), segment.GetY1());
    Point q(segment.GetX2(), segment.GetY2());
    auto strictly_in = [this](const Point& pt) -> bool {
      int dx = center_.GetX() - pt.GetX();
      int dy = center_.GetY() - pt.GetY();
      int dist_sq = dx * dx + dy * dy;
      return dist_sq < radius_ * radius_;
    };
    bool s_p = strictly_in(p);
    bool s_q = strictly_in(q);
    if (s_p && s_q) {
      return false;
    }
    if (s_p ^ s_q) {
      return true;
    }
    int vx = q.GetX() - p.GetX();
    int vy = q.GetY() - p.GetY();
    int wx = center_.GetX() - p.GetX();
    int wy = center_.GetY() - p.GetY();
    double dot = vx * wx + vy * wy;
    auto len_sq = static_cast<double>(vx * vx + vy * vy);
    if (len_sq == 0.0) {
      return ContainsPoint(p);
    }
    double t = dot / len_sq;
    if (t < 0.0) {
      t = 0.0;
    } else if (t > 1.0) {
      t = 1.0;
    }
    double proj_x = p.GetX() + t * vx;
    double proj_y = p.GetY() + t * vy;
    double dx = center_.GetX() - proj_x;
    double dy = center_.GetY() - proj_y;
    double dist_sq = dx * dx + dy * dy;
    return dist_sq <= static_cast<double>(radius_ * radius_);
  }
  AbstractShape* Clone() const override {
    return new Circle(center_, radius_);
  }
  AbstractShape* Move(const Vector& vector) override {
    int new_x = center_.GetX() + vector.GetA();
    int new_y = center_.GetY() + vector.GetB();
    return new Circle(Point(new_x, new_y), radius_);
  }
  void Print() const override {
    std::cout << center_.GetX() << " " << center_.GetY() << " " << radius_ << "\n";
  }
};
void CheckFunctions(const AbstractShape* shape, const Point& a, const Point& b) {
  if (shape->ContainsPoint(a)) {
    std::cout << "Given shape contains point A" << std::endl;
  } else {
    std::cout << "Given shape does not contain point A" << std::endl;
  }

  Segment ab_segment(a, b);
  if (shape->CrossSegment(ab_segment)) {
    std::cout << "Given shape crosses segment AB" << std::endl;
  } else {
    std::cout << "Given shape does not cross segment AB" << std::endl;
  }

  Vector ab(a, b);
  AbstractShape* cloned_shape = shape->Clone();
  AbstractShape* moved = cloned_shape->Move(ab);
  moved->Print();
  delete moved;
  delete cloned_shape;
}
int main() {
  AbstractShape* shape = nullptr;
  char command[10] = {};
  std::cin >> command;

  if (!std::strcmp(command, "point")) {
    int x = 0;
    int y = 0;
    std::cin >> x >> y;
    shape = new Point(x, y);
  } else if (!std::strcmp(command, "segment")) {
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    std::cin >> x1 >> y1 >> x2 >> y2;
    shape = new Segment(Point(x1, y1), Point(x2, y2));
  } else if (!std::strcmp(command, "ray")) {
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    std::cin >> x1 >> y1 >> x2 >> y2;
    shape = new Ray(Point(x1, y1), Point(x2, y2));
  } else if (!std::strcmp(command, "line")) {
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    std::cin >> x1 >> y1 >> x2 >> y2;
    shape = new Line(Point(x1, y1), Point(x2, y2));
  } else if (!std::strcmp(command, "polygon")) {
    size_t n_points = 0;
    std::cin >> n_points;
    std::vector<Point> points;
    points.reserve(n_points);
    for (size_t i = 0; i < n_points; ++i) {
      int x = 0;
      int y = 0;
      std::cin >> x >> y;
      points.emplace_back(x, y);
    }
    shape = new Polygon(points, n_points);
  } else if (!std::strcmp(command, "circle")) {
    int x = 0;
    int y = 0;
    std::cin >> x >> y;
    Point center(x, y);
    int radius = 0;
    std::cin >> radius;
    shape = new Circle(center, radius);
  } else {
    std::cerr << "Undefined command" << std::endl;
    return 1;
  }

  int ax = 0;
  int ay = 0;
  int bx = 0;
  int by = 0;
  std::cin >> ax >> ay >> bx >> by;

  Point a(ax, ay);
  Point b(bx, by);

  CheckFunctions(shape, a, b);

  delete shape;
  return 0;
}