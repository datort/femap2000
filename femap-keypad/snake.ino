#include "snake.h"
#include "position.h"
#include "fruit.h"
#include "renderer.h"

Snake snake;
Fruit fruit(&snake);
Renderer renderer;

Direction lastDirection = LEFT;
Direction nextDirection = LEFT;

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
  else if (numberPressed == 4) nextDirection = LEFT;
  else if (numberPressed == 6) nextDirection = RIGHT;
  else if (numberPressed == 8) nextDirection = DOWN;
  else {
    Serial.print("Number not supported by snake game: ");
    Serial.println(numberPressed);
  }
}


bool snakeIteration(void *) {
  if (lastDirection != nextDirection) {
    snake.turn(nextDirection);
    lastDirection = nextDirection;
  }

  bool resetFruit = false;

  if (snake.nextHeadPosition() == fruit.getPosition()) {
    snake.grow();
    resetFruit = true;
  }

  snake.advance();
  if (resetFruit) fruit.randomize(&snake);

  renderer.initFrame();
  renderer.renderBorder();
  renderer.renderSnake(&snake);
  renderer.renderFruit(&fruit);
  if (!snake.isAlive()) renderer.renderGameOver(&snake);
  renderer.finishCycle();

  return true;
}