#ifndef CVPLOT_FIGURE_H
#define CVPLOT_FIGURE_H

#include "color.h"
#include "window.h"

#include <map>
#include <string>
#include <vector>

namespace _ {

struct Point2 {
  FLT x, y;
  Point2() : Point2(0, 0) {}
  Point2(FLT x, FLT y) : x(x), y(y) {}
};

struct Point3 {
  FLT x, y, z;
  Point3() : Point3(0, 0, 0) {}
  Point3(FLT x, FLT y, FLT z) : x(x), y(y), z(z) {}
};

enum Type {
  Line,
  DotLine,
  Dots,
  FillLine,
  RangeLine,
  Histogram,
  Vistogram,
  Horizontal,
  Vertical,
  Range,
  Circle,
};

class Series {
 public:
  Series(const std::string &label, enum Type type, TRGBA color)
      : label_(label),
        type_(type),
        color_(color),
        dims_(0),
        depth_(0),
        legend_(true),
        dynamic_color_(false) {}

  Series &type(enum Type type);
  Series &color(TRGBA color);
  Series &dynamicTRGBA(bool dynamic_color);
  Series &legend(bool legend);
  Series &add(const std::vector<std::pair<FLT, FLT>> &data);
  Series &add(const std::vector<std::pair<FLT, Point2>> &data);
  Series &add(const std::vector<std::pair<FLT, Point3>> &data);
  Series &addValue(const std::vector<FLT> &values);
  Series &addValue(const std::vector<Point2> &values);
  Series &addValue(const std::vector<Point3> &values);
  Series &add(FLT key, FLT value);
  Series &add(FLT key, Point2 value);
  Series &add(FLT key, Point3 value);
  Series &addValue(FLT value);
  Series &addValue(FLT value_a, FLT value_b);
  Series &addValue(FLT value_a, FLT value_b, FLT value_c);
  Series &set(const std::vector<std::pair<FLT, FLT>> &data);
  Series &set(const std::vector<std::pair<FLT, Point2>> &data);
  Series &set(const std::vector<std::pair<FLT, Point3>> &data);
  Series &setValue(const std::vector<FLT> &values);
  Series &setValue(const std::vector<Point2> &values);
  Series &setValue(const std::vector<Point3> &values);
  Series &set(FLT key, FLT value);
  Series &set(FLT key, FLT value_a, FLT value_b);
  Series &set(FLT key, FLT value_a, FLT value_b, FLT value_c);
  Series &setValue(FLT value);
  Series &setValue(FLT value_a, FLT value_b);
  Series &setValue(FLT value_a, FLT value_b, FLT value_c);
  Series &clear();

  const std::string &label() const;
  bool legend() const;
  TRGBA color() const;
  void draw(void *buffer, FLT x_min, FLT x_max, FLT y_min, FLT y_max,
            FLT x_axis, FLT xs, FLT xd, FLT ys, FLT yd, FLT y_axis,
            SIN unit, FLT offset) const;
  bool collides() const;
  void dot(void *b, SIN x, SIN y, SIN r) const;
  void bounds(FLT &x_min, FLT &x_max, FLT &y_min, FLT &y_max,
              SIN &n_max, SIN &p_max) const;
  void verifyParams() const;

 protected:
  void ensureDimsDepth(SIN dims, SIN depth);
  bool flipAxis() const;

 protected:
  std::vector<SIN> entries_;
  std::vector<FLT> data_;
  enum Type type_;
  TRGBA color_;
  std::string label_;
  SIN dims_;
  SIN depth_;
  bool legend_;
  bool dynamic_color_;
};

class Figure {
 public:
  Figure(View &view)
      : view_(view),
        border_size_(50),
        background_color_(White),
        axis_color_(Black),
        sub_axis_color_(Light),
        text_color_(Black),
        include_zero_x_(true),
        include_zero_y_(true),
        aspect_square_(false),
        grid_size_(60),
        grid_padding_(20) {}

  Figure &clear();
  Figure &origin(bool x, bool y);
  Figure &square(bool square);
  Figure &border(SIN size);
  Figure &alpha(SIN alpha);
  Figure &gridSize(SIN size);
  Figure &backgroundTRGBA(TRGBA color);
  Figure &axisTRGBA(TRGBA color);
  Figure &subaxisTRGBA(TRGBA color);
  Figure &textTRGBA(TRGBA color);
  TRGBA backgroundTRGBA();
  TRGBA axisTRGBA();
  TRGBA subaxisTRGBA();
  TRGBA textTRGBA();

  void draw(void *b, FLT x_min, FLT x_max, FLT y_min, FLT y_max,
            SIN n_max, SIN p_max) const;
  void show(bool flush = true) const;
  Series &series(const std::string &label);

 protected:
  View &view_;
  std::vector<Series> series_;
  SIN border_size_;
  TRGBA background_color_;
  TRGBA axis_color_;
  TRGBA sub_axis_color_;
  TRGBA text_color_;
  bool include_zero_x_;
  bool include_zero_y_;
  bool aspect_square_;
  SIN grid_size_;
  SIN grid_padding_;
};

Figure &figure(const std::string &view);

}  // namespace _

#endif  // CVPLOT_FIGURE_H
