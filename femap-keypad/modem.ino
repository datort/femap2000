bool handleModemMessage(String message) {
  if (message.indexOf("RING") > -1) {
    return handleIncomingCall();
  } else if (message.indexOf("NO CARRIER") > -1) {
    return handleNoCarrier();
  } else if (message.indexOf("CLIP") > -1) {
    return handleCallerId(message);
  } else if (message.indexOf("SIM800") > -1) {
    return handleSetup();
  } else if (message.indexOf("+CSPN") > -1) {
    return updateServiceProvider(message);
  } else if (message.indexOf("+CSQ") > -1) {
    return updateSMeter(message);
  }

  return false;
}

bool updateSMeter(String message) {
  int start = message.indexOf("+CSQ: ") + 6;
  int end = message.indexOf(",", start);

  sMeter = message.substring(start, end).toInt();
  return true;
}

bool updateServiceProvider(String message) {
  int start = message.indexOf("+CSPN: ") + 8;
  int end = message.indexOf("\"", start);

  serviceProvider = message.substring(start, end);
  return true;
}

bool handleIncomingCall() {
  isRinging = true;
  return true;
}

bool handleNoCarrier() {
  isRinging = false;
  callerId = "";
  return true;
}

bool handleSetup() {
  Serial.println("Enabling CLIP");
  Serial1.println("AT+CLIP=1");
  return true;
}

bool handleCallerId(String message) {
  int start = message.indexOf("+CLIP: ") + 8;
  int end = message.indexOf("\"", start);

  callerId = message.substring(start, end);
  return true;
}