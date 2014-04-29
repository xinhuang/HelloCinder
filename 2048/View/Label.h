#pragma once

#include <cinder/Rect.h>
#include <cinder/Color.h>
#include <cinder/Vector.h>
#include <cinder/Font.h>

#include <memory>
#include <string>

class Label {
  struct Data;
  std::unique_ptr<Data> d;

  void updateCache() const;

public:
  Label();
  ~Label();

  void draw() const;

  void setText(const std::string& text);
  void setForeColor(const ci::Color& color);
  void setBackColor(const ci::Color& color);
  void setFont(const ci::Font& font);
  void setSize(const ci::Vec2f& size);
  void setLocation(const ci::Vec2f& loc);

  const ci::Vec2f& size() const;
  const ci::Vec2f& location() const;
  const ci::Font& font() const;
  ci::Rectf rect() const { return{ location(), location() + size() }; }
};