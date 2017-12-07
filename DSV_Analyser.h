//
// Created by Sn0w1eo on 07.12.2017.
//

#include <cstdio>
#include <string>

#ifndef DSV_ANALYSER_DSV_ANALYSER_H
#define DSV_ANALYSER_DSV_ANALYSER_H

// FIELD TYPES OF DSV
#define DSV_TEXT_TYPE    0x01
#define DSV_INTEGER_TYPE 0x02
#define DSV_DOUBLE_TYPE  0x03

// ASCII codes of 0 and 9
#define ASCII_LOW_NUMBER  48
#define ASCII_HIGH_NUMBER 57

class DSV_Analyser {
private:
    int   buffer;
    char  delimiter;
    char* filepath;
    char  decimalMark;

    FILE* dsv_file;
    bool  eof;

    int Next();
    int getNextFieldType();
    int getNumberType();
    int getMantissa();
public:
    DSV_Analyser(const char* _filepath, char _delimiter, char _decimalMark = '.');
    ~DSV_Analyser();
    void Analyse();
};


#endif //DSV_ANALYSER_DSV_ANALYSER_H
