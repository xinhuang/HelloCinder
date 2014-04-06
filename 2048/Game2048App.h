#pragma once

#include <cinder/app/AppBasic.h>

#include <memory>
#include <vector>

class Piece;
class Cell;

class Game2048App : public ci::app::AppBasic {
public:
  Game2048App();
  ~Game2048App();

  void setup() final;

  void draw() final;
  void update() final;
  void keyUp(ci::app::KeyEvent e) final;
  void resize() final;

private:
  void spawn();
  bool moveAll(const ci::Vec2i& dir);
  void drawBoard(const ci::Vec2f& pos, const ci::Vec2f& size) const;
  void drawPiece(const ci::Vec2f& pos, const Piece& piece, float width, float height) const;
  std::vector<ci::Vec2i> getFreeSpaces() const;
  bool isOccupied(const ci::Vec2i& pos) const;

  std::unique_ptr<Cell>& at(const ci::Vec2i& pos);
  const std::unique_ptr<Cell>& at(const ci::Vec2i& pos) const;

  void clear(const ci::Vec2i& pos);
  std::vector<int> buildTraversals(int max, int dir) const;
  bool moveToFurthest(ci::Vec2i pos, const ci::Vec2i& dir);

private:
  struct Data;
  std::unique_ptr<Data> d;
};
