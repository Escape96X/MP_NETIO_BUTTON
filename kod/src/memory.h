#ifndef _memory_h_
#define _memory_h_

#include <WString.h>

String readEEPROM(int numberOfStart, int len);

void saveToEEPROM(String sTosave, int startPosition, int len);

void saveToEEPROMContent(String sToSave, int startPosition, int jmp);

int countContent(int offset, int posB, int jmp);

String readContent(int i, int offset, int posB, int jmp);

void deleteContent(int i, int offset, int posB, int jmp);

void debugPair();

#endif