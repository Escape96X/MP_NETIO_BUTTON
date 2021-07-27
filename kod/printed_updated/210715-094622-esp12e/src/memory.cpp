#include <EEPROM.h>
#include "header.h"
#include "define.h"


// Prace s pameti

void saveToEEPROM(String sToSave, int startPosition, int len) {
    // globalni ukladani do pameti
    char arr[sToSave.length() + 1];
    strcpy(arr, sToSave.c_str());
    for (int i = 0; i < sToSave.length(); i++) {
        EEPROM.write(startPosition + i, arr[i]);
        if (i == len - 1)
            break;
    }
    EEPROM.write(startPosition + sToSave.length(), -1);
    EEPROM.commit();
    delay(500);
}

String readEEPROM(int numberOfStart, int len) {
    // globalni cteni z pameti
    String tempNetio = "";
    for (int i = 0; i < len; i++) {
        char netio = EEPROM.read(numberOfStart + i);
        if (netio == 255)
            break;
        tempNetio += netio;
    }
    return tempNetio;
}

// prace s IP

void saveToEEPROMContent(String sToSave, int startPosition, int jmp) {
    // nalezeni mista pro IP v bloku pameti
    for (int i = startPosition; i < jmp*10 + startPosition; i += jmp) {
        char isItIp = EEPROM.read(i);
        if (isItIp == 255) {
            EEPROM.write(i, '#');
            saveToEEPROM(sToSave, i + 1, jmp-1);
            break;
        }
    }
}

int countContent(int offset, int posB, int jmp) {
    int numberOfContent = 0;
    for (int i = offset; i < posB-7 + offset; i += jmp) {
        char character = EEPROM.read(i);
        if (character == '#')
            numberOfContent++;
    }
    return numberOfContent;
}

String readContent(int i, int offset, int posB, int jmp) {
    int position = (i * jmp) + offset;
    if (EEPROM.read(position) == '#')
        return readEEPROM(position + 1, jmp-1);
    else {
        for (int j = position; position < posB + offset; position += jmp) {
            char character = EEPROM.read(position);
            if (character == 35)
                return readEEPROM(position + 1, jmp-1);
        }
        return "?";
    }
}

void deleteContent(int i, int offset, int posB, int jmp) {
    int position = (i * jmp) + offset;
    if (EEPROM.read(position) == '#') {
        for (int j = 0; j < jmp; j++)
            EEPROM.write(j + position, 255);
    } else {
        for (int j = position; position < posB + offset; position += jmp) {
            char character = EEPROM.read(position);
            if (character == 35) {
                for (int k = 0; k < jmp; k++)
                    EEPROM.write(j + position, 255);
            }
        }
    }
    EEPROM.commit();
}
