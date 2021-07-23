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

void saveToEEPROMIP(String sToSave, int startPosition) {
    // nalezeni mista pro IP v bloku pameti
    for (int i = 0 + startPosition; i < 160 + startPosition; i += IP_JMP) {
        char isItIp = EEPROM.read(i);
        if (isItIp == 255) {
            EEPROM.write(i, '#');
            saveToEEPROM(sToSave, i + 1, 15);
            break;
        }
    }
}

int countIP(int offset) {
    int numberOfIP = 0;
    for (int i = offset; i < 159 + offset; i += IP_JMP) {
        char character = EEPROM.read(i);
        if (character == '#')
            numberOfIP++;
    }
    return numberOfIP;
}

String readIP(int i, int offset) {
    int position = (i * IP_JMP) + offset;
    if (EEPROM.read(position) == '#')
        return readEEPROM(position + 1, 15);
    else {
        for (int j = position; position < IP_POSB + offset; position += IP_JMP) {
            char character = EEPROM.read(position);
            if (character == 35)
                return readEEPROM(position + 1, 15);
        }
        return "?";
    }
}

void deleteIP(int i, int offset) {
    int position = (i * IP_JMP) + offset;
    if (EEPROM.read(position) == '#') {
        for (int j = 0; j < IP_JMP; j++)
            EEPROM.write(j + position, 255);
    } else {
        for (int j = position; position < IP_POSB + offset; position += IP_JMP) {
            char character = EEPROM.read(position);
            if (character == 35) {
                for (int k = 0; k < IP_JMP; k++)
                    EEPROM.write(j + position, 255);
            }
        }
    }
    EEPROM.commit();
}
