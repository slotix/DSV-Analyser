//
// Created by Sn0w1eo on 07.12.2017.
//
#include "DSV_Analyser.h"
#include <cstring>


// CONSTRUCTORS
DSV_Analyser::DSV_Analyser(const char* _filepath, const char _delimiter, const char _decimalMark) :
        eof(false), buffer(0), delimiter(_delimiter), filepath(strdup(_filepath)), dsv_file(nullptr), decimalMark(_decimalMark) {}
DSV_Analyser::~DSV_Analyser() {
    if (dsv_file != nullptr) fclose(dsv_file);
};

// PUBLIC METHODS
void DSV_Analyser::Analyse() {
    int result;
    if (dsv_file != nullptr) fclose(dsv_file);
    dsv_file = fopen(filepath, "r");
    while(!eof) {
        result = Next();
        // printf("%d",result);
    }
    fclose(dsv_file);
}

// PRIVATE METHODS
int DSV_Analyser::Next() {
    try {
        return getNextFieldType();
    } catch(int e) {
        eof = true;
        return e;
    }
}

// returns DSV_TEXT_TYPE
int DSV_Analyser::getNextFieldType() {
    bool first_byte = true;
    int buffer = 0;
    while( true ) {
        if (buffer==EOF) throw DSV_TEXT_TYPE;
        buffer = fgetc(dsv_file);
        if (buffer==delimiter) break;
        if (buffer >= ASCII_LOW_NUMBER && buffer <= ASCII_HIGH_NUMBER && first_byte) {
            return getNumberType();
        }
        first_byte = false;
    }
    return DSV_TEXT_TYPE;
}

// detects INTEGER type the field
// if NOT returns DSV_TEXT_TYPE
int DSV_Analyser::getNumberType() {
    int result = DSV_INTEGER_TYPE;
    while(true) {
        if (buffer==EOF) throw DSV_INTEGER_TYPE;
        buffer = fgetc(dsv_file);
        if (buffer==delimiter) break;
        if (buffer==decimalMark) {
            return getMantissa();
        }
        if (buffer < ASCII_LOW_NUMBER || buffer > ASCII_HIGH_NUMBER) result = DSV_TEXT_TYPE;
    }
    return result;
}

// detects DOUBLE type of the field
// if NOT returns DSV_TEXT_TYPE
int DSV_Analyser::getMantissa() {
    int result = DSV_DOUBLE_TYPE;
    while(true) {
        if (buffer == EOF) throw DSV_DOUBLE_TYPE;
        buffer = fgetc(dsv_file);
        if (buffer==delimiter) break;
        if (buffer < ASCII_LOW_NUMBER || buffer > ASCII_HIGH_NUMBER) result = DSV_TEXT_TYPE;
    }
    return result;
}
