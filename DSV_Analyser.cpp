//
// Created by Sn0w1eo on 07.12.2017.
//
#include "DSV_Analyser.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>


void moveToNewLine(FILE* file) {
    int buffer;
    while(true) {
        buffer=fgetc(file);
        if (buffer == '\n') return;
        if (buffer == EOF) return;
    }
}


// CONSTRUCTORS
DSV_Analyser::DSV_Analyser(const char* _filepath, const char _delimiter, const char _decimalMark) :
        eof(false), buffer(0), delimiter(_delimiter), filepath(strdup(_filepath)),
        dsv_file(nullptr), decimalMark(_decimalMark), Columns(0), totalColumns(0), hasHeader(true), currentColumn(0), currentFieldLength(0) {}
DSV_Analyser::~DSV_Analyser() {
    if (dsv_file != nullptr) fclose(dsv_file);
    std::vector<DSV_FieldInfo>().swap(Columns);
    delete[] filepath;
};


// PUBLIC METHODS
void DSV_Analyser::Analyse(bool _hasHeader = true) {
    std::vector<DSV_FieldInfo>().swap(Columns);
    hasHeader = _hasHeader;
    int result;
    ReadHeader();
    if (dsv_file != nullptr) fclose(dsv_file);
    dsv_file = fopen(filepath, "r");
    if (hasHeader) moveToNewLine(dsv_file);
    currentColumn = 0;
    while(!eof) {
        result = Next();
        Columns[currentColumn].Type |= result;
        if (Columns[currentColumn].Length <= currentFieldLength)
            Columns[currentColumn].Length = currentFieldLength;
        currentFieldLength=0;
        currentColumn++;
        if (currentColumn==totalColumns) {currentColumn=0; }
    }
    fclose(dsv_file);
}

void DSV_Analyser::ReadHeader() {
    std::ifstream file(filepath);
    std::string headers_line;
    std::string str_buffer;
    getline(file, headers_line);
    std::istringstream dl(headers_line);
    while (getline(dl, str_buffer,  delimiter)){
        DSV_FieldInfo info = { hasHeader ? str_buffer : "", 0, DSV_TYPES::UNDEFINED_TYPE};
        Columns.push_back(info);
    }
    totalColumns = Columns.size();
    file.close();
}



// PRIVATE METHODS
int DSV_Analyser::Next() {
    currentFieldLength = 0;
    try {
        return getNextFieldType();
    } catch(int e) {
        eof = true;
        return e;
    }
}

// returns DSV_TYPES::TEXT_TYPE
int DSV_Analyser::getNextFieldType() {
    bool first_byte = true;
    while( true ) {
        if (buffer==EOF) throw int(DSV_TYPES::TEXT_TYPE);
        buffer = fgetc(dsv_file);
        //if (buffer=='\r') buffer=' ';
        if (buffer=='\n') return DSV_TYPES::TEXT_TYPE;
        if (buffer==delimiter) break;
        if (buffer >= ASCII_LOW_NUMBER && buffer <= ASCII_HIGH_NUMBER && first_byte) {
            return getNumberType();
        }
        first_byte = false;
        ++currentFieldLength;
    }
    return DSV_TYPES::TEXT_TYPE;
}

// detects INTEGER type the field
// if NOT returns DSV_TYPES::TEXT_TYPE
int DSV_Analyser::getNumberType() {
    ++currentFieldLength;
    int result = DSV_TYPES::INTEGER_TYPE;
    while(true) {
        if (buffer==EOF) throw int(DSV_TYPES::INTEGER_TYPE);
        buffer = fgetc(dsv_file);
        if (buffer=='\r') buffer=' ';
        if (buffer=='\n') return DSV_TYPES::INTEGER_TYPE;
        if (buffer==delimiter) break;
        if (buffer==decimalMark) {
            return getMantissa();
        }
        if (buffer < ASCII_LOW_NUMBER || buffer > ASCII_HIGH_NUMBER) result = DSV_TYPES::TEXT_TYPE;
        ++currentFieldLength;
    }
    return result;
}

// detects DOUBLE type of the field
// if NOT returns DSV_TYPES::TEXT_TYPE
int DSV_Analyser::getMantissa() {
    ++currentFieldLength;
    int result = DSV_TYPES::DOUBLE_TYPE;
    while(true) {
        if (buffer == EOF) throw int(DSV_TYPES::DOUBLE_TYPE);
        buffer = fgetc(dsv_file);
        if (buffer=='\r') buffer=' ';
        if (buffer=='\n') return DSV_TYPES::DOUBLE_TYPE;
        if (buffer==delimiter) break;
        if (buffer < ASCII_LOW_NUMBER || buffer > ASCII_HIGH_NUMBER) result = DSV_TYPES::TEXT_TYPE;
        ++currentFieldLength;
    }
    return result;
}




// USEFUL FUNCTIONS

