//
// Created by Sn0w1eo on 07.12.2017.
//

#ifndef DSV_ANALYSER_DSV_ANALYSER_H
#define DSV_ANALYSER_DSV_ANALYSER_H

#include <cstdio>
#include <string>
#include <vector>

// FIELD TYPES OF DSV
#define DSV_UNDEFINED_TYPE  0x00
#define DSV_TEXT_TYPE       0x01
#define DSV_INTEGER_TYPE    0x02
#define DSV_DOUBLE_TYPE     0x04

// ASCII codes of 0 and 9
#define ASCII_LOW_NUMBER  48
#define ASCII_HIGH_NUMBER 57


struct DSV_FieldInfo {
    std::string   Title;
    unsigned int  Length;
    int           Type;
};



class DSV_Analyser {
private:
    int   buffer;
    char  delimiter;
    char* filepath;
    char  decimalMark;

    FILE* dsv_file;
    bool  eof;

    bool hasHeader;
    int  totalColumns;
    int  currentColumn;
    unsigned int currentFieldLength;

    int Next();
    int getNextFieldType();
    int getNumberType();
    int getMantissa();
    void ReadHeader();
public:
    std::vector<DSV_FieldInfo> Columns;
    DSV_Analyser(const char* _filepath, char _delimiter, char _decimalMark = '.');
    ~DSV_Analyser();
    void Analyse(bool _hasHeader);
};


#endif //DSV_ANALYSER_DSV_ANALYSER_H
