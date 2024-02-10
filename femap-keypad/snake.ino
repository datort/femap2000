#include "snake.h"
#include "position.h"
#include "fruit.h"
#include "renderer.h"

Snake snake;
Fruit fruit(&snake);
Renderer renderer;

Direction nextDirection;

void startSnake() {
  timer.cancel();
  snakeMode = true;

  oled.clearDisplay();
  oled.setCursor(6, 24);
  oled.setTextSize(2);
  oled.print("Snake");
  oled.display();
  delay(500);
  renderer.setDisplay(oled);
  timer.every(500, snakeIteration);
}

void handleSnakeInput(int numberPressed) {
  if (numberPressed == 2) nextDirection = UP;
  if (numberPressed == 4) nextDirection = LEFT;
  if (numberPressed == 6) nextDirection = RIGHT;
  if (numberPressed == 8) nextDirection = DOWN;
}


bool snakeIteration(void *) {
  if (nextDirection.length() > 0) {
    snake.turn(nextDirection);
    delete nextDirection;
  }


  snake.advance();

  renderer.initFrame();
  renderer.renderBorder();
  renderer.renderSnake(&snake);
  renderer.renderFruit(&fruit);

  //oled.display();
  renderer.finishCycle();
  return true;
}