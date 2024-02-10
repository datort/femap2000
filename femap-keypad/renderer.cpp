#include "renderer.h"
#include "Arduino.h"
#include <Adafruit_SSD1306.h>

#define CELL_SIZE 8

void Renderer::setDisplay(Adafruit_SSD1306 oled) {
  this->oled = oled;
}

void Renderer::initFrame() {
  this->oled.clearDisplay();
}


  /*unsigned long time_total = 0;
  unsigned long time_last = 0;
  float framerate() {
    time_last = time_total;
    time_total = micros();
    return 1.0 / ((time_total - time_last) / 1000000.0);
  }*/

void Renderer::renderBorder() {
  this->oled.drawRect(0, 0, 128, 64, 1);
}
  
void Renderer::renderSnake(Snake *snake) {
  const uint8_t **body = snake->getBody();
  for(int i = 0; i < Snake::BODY_WIDTH; i++) {
    for(int j = 0; j < Snake::BODY_HEIGHT; j++) {
      if(body[i][j] > 0) {
        this->oled.fillRect(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, 1);
      }
    }
  }
}

void Renderer::renderFruit(Fruit * fruit) {
  Position position = fruit->getPosition();
  this->oled.drawTriangle(
    position.x, 
    position.y, 
    position.x + 3, 
    position.y -5, 
    position.x + 5, 
    position.y, 
    1
  );
  //u8g2.drawFrame(position.x * CELL_SIZE + 1, position.y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2);
}

  /*void renderGameOver(Snake *snake) {
    u8g2.drawBox(32, 20, 64, 22);
    u8g2.setDrawColor(0);
    u8g2.setCursor(34, 30);
    u8g2.print("Game Over!");
    u8g2.setCursor(34, 40);
    u8g2.print("Points: ");
    u8g2.print(snake->getPoints());
    u8g2.setDrawColor(1);
  }

  void startFrame() {
    u8g2.clearBuffer();
  }*/

void Renderer::finishCycle() {
  this->oled.display();
}
