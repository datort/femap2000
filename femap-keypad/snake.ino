#include "snake.h"
#include "position.h"
#include "fruit.h"
#include "renderer.h"

Snake snake;
Fruit fruit(&snake);

void startSnake() {
  timer.cancel();
  snakeMode = true;

  oled.clearDisplay();
  oled.setCursor(6, 24);
  oled.setTextSize(2);
  oled.print("Snake");
  oled.display();
  delay(500);

  
}

void handleSnakeInput(int numberPressed) {

}


void snakeIteration() {
  oled.clearDisplay();

  Renderer::renderBorder(oled);
  Renderer::renderSnake(oled, &snake);

  oled.display();
}