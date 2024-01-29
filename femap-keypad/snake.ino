void startSnake() {
  timer.cancel();
  snakeMode = true;

  oled.clearDisplay();
  oled.setCursor(0, 30);
  oled.setTextSize(2);
  oled.print("Snake");
}