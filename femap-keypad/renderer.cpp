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
    position.x * CELL_SIZE, 
    position.y * CELL_SIZE + 1, 
    position.x * CELL_SIZE + 3, 
    position.y * CELL_SIZE + 6, 
    position.x * CELL_SIZE + 5, 
    position.y * CELL_SIZE + 1, 
    1
  );
}


void Renderer::renderGameOver(Snake *snake) {
  this->oled.fillRoundRect(30, 18, 70, 30, 2, 1);
  this->oled.setCursor(36, 26);
  this->oled.setTextSize(1);
  this->oled.setTextColor(0);
  this->oled.print("Game Over!");
  this->oled.setCursor(36, 38);
  this->oled.print("Points: ");
  this->oled.print(snake->getPoints());
}


void Renderer::finishCycle() {
  this->oled.display();
}
