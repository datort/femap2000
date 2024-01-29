void drawSMeter() {
  int offsetX = 112;
  oled.drawPixel(offsetX + 0, 0, 1);
  oled.drawPixel(offsetX + 4, 0, 1);
  oled.drawPixel(offsetX + 0, 1, 1);
  oled.drawPixel(offsetX + 4, 1, 1);
  oled.drawPixel(offsetX + 0, 2, 1);
  oled.drawPixel(offsetX + 4, 2, 1);

  oled.drawPixel(offsetX + 1, 3, 1);
  oled.drawPixel(offsetX + 3, 3, 1);

  oled.drawPixel(offsetX + 2, 4, 1);
  oled.drawPixel(offsetX + 2, 5, 1);
  oled.drawPixel(offsetX + 2, 6, 1);

  oled.drawPixel(offsetX + 7, 6, 1);
  oled.drawPixel(offsetX + 9, 6, 1);
  oled.drawPixel(offsetX + 11, 6, 1);
  oled.drawPixel(offsetX + 13, 6, 1);
  oled.drawPixel(offsetX + 15, 6, 1);

  if (sMeter > 1) {
    oled.drawPixel(offsetX + 7, 5, 1);
  }

  if (sMeter >= 10) {
    oled.drawPixel(offsetX + 9, 5, 1);
    oled.drawPixel(offsetX + 9, 4, 1);
  }

  if (sMeter >= 18) {
    oled.drawPixel(offsetX + 11, 5, 1);
    oled.drawPixel(offsetX + 11, 4, 1);
    oled.drawPixel(offsetX + 11, 3, 1);
  }

  if (sMeter >= 22) {
    oled.drawPixel(offsetX + 13, 5, 1);
    oled.drawPixel(offsetX + 13, 4, 1);
    oled.drawPixel(offsetX + 13, 3, 1);
    oled.drawPixel(offsetX + 13, 2, 1);
  }

  if (sMeter >= 28) {
    oled.drawPixel(offsetX + 15, 5, 1);
    oled.drawPixel(offsetX + 15, 4, 1);
    oled.drawPixel(offsetX + 15, 3, 1);
    oled.drawPixel(offsetX + 15, 2, 1);
    oled.drawPixel(offsetX + 15, 1, 1);
  }
}

void drawServiceProvider() {
  oled.setCursor(0, 0);
  oled.print(serviceProvider);
}

void drawIncomingCall() {
  if (!isRinging) return;

  oled.setTextSize(1);
  oled.setCursor(0, 30);
  oled.print("Incoming call");

  if (callerId.startsWith("+")) {
    oled.setCursor(0, 40);
    oled.print(callerId);
  }
}

void drawActiveCall() {
  if (!isActiveCall) return;

  oled.setTextSize(1);
  oled.setCursor(0, 30);
  oled.print("Active call");

  if (callerId.startsWith("+")) {
    oled.setCursor(0, 40);
    oled.print(callerId);
  }
}

void drawHeadsetState() {
  oled.setTextSize(1);
  oled.setCursor(0, 55);
  oled.print(isPickedUp ? "picked up" : "hung up");
}

void drawDialedNumber() {
  if (!dialedNumber || isRinging || isActiveCall) return;

  oled.setCursor(0, 30);
  oled.setTextSize(dialedNumber.length() > 10 ? 1 : 2);
  oled.print(dialedNumber);
}