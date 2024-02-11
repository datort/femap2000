#pragma once

#include "Arduino.h"
#include "snake.h"
#include "position.h"
#include "fruit.h"
#include <Adafruit_SSD1306.h>

class Renderer {
  public:
    void setDisplay(Adafruit_SSD1306 oled);
    void initFrame();
    void renderBorder();
    void renderSnake(Snake *snake);
    void renderGameOver(Snake *snake);
    void renderFruit(Fruit *fruit);
    void finishCycle();
  private:
    Adafruit_SSD1306 oled;
};
