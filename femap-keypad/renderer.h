#pragma once

#include "Arduino.h"
#include "snake.h"
#include "position.h"
#include "fruit.h"
#include <Adafruit_SSD1306.h>

namespace Renderer {
  //void initialize();
  void renderBorder(Adafruit_SSD1306 oled);
  void renderSnake(Adafruit_SSD1306 oled, Snake *snake);
  /*void renderFruit(Fruit *fruit);
  void renderGameOver(Snake *snake);
  void startFrame();
  void endFrame();*/
}
