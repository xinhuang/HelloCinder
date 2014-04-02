#pragma once

#include <cinder/app/AppBasic.h>

#include <memory>
#include <vector>

struct Piece;

class Game2048App : public ci::app::AppBasic {
public:
  Game2048App();
  ~Game2048App();

  void setup() final;

  void draw() final;
  void update() final;
  void keyUp(ci::app::KeyEvent e) final;

private:
  void spawn();
  bool moveAll(const ci::Vec2i& offset);
  void drawBoard(const ci::Vec2f& pos, const ci::Vec2f& size) const;
  void drawPiece(const ci::Vec2f& pos, int value, float width, float height) const;
  std::vector<ci::Vec2i> getFreeSpaces() const;
  bool isOccupied(const ci::Vec2i& pos) const;
  Piece& at(const ci::Vec2i& pos);

private:
  struct Data;
  std::unique_ptr<Data> d;
};
