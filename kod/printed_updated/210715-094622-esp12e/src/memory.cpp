#include <EEPROM.h>
#include "header.h"
#include "define.h"


// Prace s pameti

void saveToEEPROM(String sToSave, int startPosition, int len) {
    // globalni ukladani do pameti
    char arr[sToSave.length() + 1];
    strcpy(arr, sToSave.c_str());
    int lenght = sToSave.length();
    for (int i = 0; i < lenght; i++) {
        EEPROM.write(startPosition + i, arr[i]);
        if (i == len - 1)
            break;
    }
    EEPROM.write(startPosition + sToSave.length(), -1);
    if (EEPROM.commit()) {
        Serial.println("EEPROM successfully committed");
    } else {
        Serial.println("ERROR! EEPROM commit failed");
    }
    delay(1000);
}

String readEEPROM(int numberOfStart, int len) {
    // globalni cteni z pameti
    String tempNetio = "";
    for (int i = 0; i < len; i++) {
        char netio = EEPROM.read(numberOfStart + i);
        if (netio == 255) {
            break;
        }
        tempNetio += netio;
    }
    return tempNetio;
}

// prace s IP

void saveToEEPROMContent(String sToSave, int startPosition, int jmp) {
    // nalezeni mista pro IP v bloku pameti
    for (int i = startPosition; i < jmp * 10 + startPosition; i += jmp) {
        char isItIp = EEPROM.read(i);
        Serial.println(isItIp);
        if (isItIp == 255) {
            Serial.print("Nalezeno místo na adrese");
            Serial.println(i);
            EEPROM.write(i, '#');
            saveToEEPROM(sToSave, i + 1, jmp - 1);
            break;
        }
    }
}

int countContent(int offset, int posB, int jmp) {
    int numberOfContent = 0;
    for (int i = offset; i < posB - 7 + offset; i += jmp) {
        char character = EEPROM.read(i);
        if (character == '#')
            numberOfContent++;
    }
    return numberOfContent;
}

String readContent(int i, int offset, int posB, int jmp) {
    int position = (i * jmp) + offset;
    Serial.print("READ: Pozice:");
    Serial.println(position);
    if (EEPROM.read(position) == '#') {
        Serial.println("pozice potvrzena");
        return readEEPROM(position + 1, jmp - 1);
    } else {
        Serial.println("Pozice Nenalezena, hledam dalsi: skacu");
        for (int j = position; j < posB + offset; j += jmp) {
            char character = EEPROM.read(j);
            if (character == 35)
                return readEEPROM(j + 1, jmp - 1);
        }
        return "?";
    }
}

void deleteContent(int i, int offset, int posB, int jmp) {
    int position = (i * jmp) + offset;
    Serial.print("DELETE: Pozice:");
    Serial.println(position);
    if (EEPROM.read(position) == '#') {
        Serial.println("pozice potvrzena");
        for (int j = 0; j < jmp; j++)
            EEPROM.write(j + position, 255);
    } else {
        Serial.println("Pozice Nenalezena, hledam dalsi: skacu");
        for (int j = position; position < posB + offset; position += jmp) {
            Serial.println(position);
            char character = EEPROM.read(position);
            if (character == 35) {
                for (int k = 0; k < jmp; k++)
                    EEPROM.write(j + position, 255);
                break;
            }
        }
    }
    EEPROM.commit();
}
