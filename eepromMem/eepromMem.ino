#include <EEPROM.h>

#define prefix "eeprom"

void setup() {
  Serial.begin(9600);
  delay(3000); //чтобы успеть открыть монитор порта
  Serial.println("to run commands use prefix: " prefix);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    commands(input);
  }
}

void commands(String cmd) {
  if (!cmd.startsWith(prefix)) {
    Serial.println("error invalid prefix");
    return;
  }

  cmd = cmd.substring(String(prefix).length());
  cmd.trim();

  if (cmd.indexOf("-w") != -1) {
  int address = getVal(cmd, "-a");
  int value = getVal(cmd, "-v");
  
  if (address == -1 || value == -1) {
    Serial.println("error missing address or value");
    return;
  }
  
  if (address < 0 || address >= EEPROM.length() - 1) {
    Serial.println("error address out of range");
    return;
  }
  
  EEPROM.put(address, value);
  Serial.println("SUCCESS WRITE");
    
  } else if (cmd.indexOf("-r") != -1) {
  int address = getVal(cmd, "-a");
  
  if (address == -1) {
    Serial.println("error missing address");
    return;
  }
  
  if (address < 0 || address >= EEPROM.length() - 1) {
    Serial.println("error address out of range");
    return;
  }
  
  int value = EEPROM.get(address, value);
  Serial.print("Value at address ");
  Serial.print(address);
  Serial.print(": ");
  Serial.println(value);
    
  } else if (cmd.indexOf("-e") != -1) {
    int address = getVal(cmd, "-a");
    
    if (address == -1) {
      Serial.println("error missing address");
      return;
    }
    
    if (address < 0 || address >= EEPROM.length()) {
      Serial.println("error address out of range");
      return;
    }
    
    EEPROM.write(address, 0);
    Serial.println("address erased");
    
  } else if (cmd.indexOf("-d") != -1) {
    dumpEeprom();
    
  } else {
    Serial.println("unknown command");
  }
}

int getVal(String cmd, String param) {
  int paramIndex = cmd.indexOf(param);
  if (paramIndex == -1) return -1;
  
  int valStart = cmd.indexOf(" ", paramIndex) + 1;
  if (valStart == 0) return -1;
  
  int valEnd = cmd.indexOf(" ", valStart);
  if (valEnd == -1) valEnd = cmd.length();
  
  String valueStr = cmd.substring(valStart, valEnd);
  return valueStr.toInt();
}

void dumpEeprom() {
  int bytesInLine = 8;
  
  for (int i = 0; i < EEPROM.length(); i += bytesInLine) {
    Serial.print(i < 16 ? "000" : (i < 100 ? "00" : (i < 1000 ? "0" : "")));
    Serial.print(i, DEC);
    Serial.print(" : ");
    
    for (int j = 0; j < bytesInLine; j++) {
      if (i + j < EEPROM.length()) {
        byte val = EEPROM.read(i + j);
        if (val < 16) Serial.print("0");
        Serial.print(val, HEX);
        Serial.print(" ");
      }
    }
    Serial.println();
  }
}

//запись: eeprom -w -a адрес -v значение
//чтение: eeprom -r -a адрес
//стереть: eeprom -e -a адрес
//дамп всех ячеек: eeprom -d
