//
// Created by Sn0w1eo on 07.12.2017.
//
#include "DSV_Analyser.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>




// CONSTRUCTORS
DSV_Analyser::DSV_Analyser(const char* _filepath, const char _delimiter, const char _decimalMark) :
        eof(false), buffer(0), delimiter(_delimiter), filepath(strdup(_filepath)),
        dsv_file(nullptr), decimalMark(_decimalMark), Columns(0), totalColumns(0), hasHeader(true),
        currentColumn(0), currentFieldLength(0), fileSize(0), currentFilePosition(0)
{
    std::ifstream file_size_counter(filepath, std::ios::binary | std::ios::ate);
    fileSize = file_size_counter.tellg();
    file_size_counter.close();
}

DSV_Analyser::~DSV_Analyser() {
    if (dsv_file != nullptr) fclose(dsv_file);
    std::vector<DSV_FieldInfo>().swap(Columns);
    delete[] filepath;
};


// PUBLIC METHODS
void DSV_Analyser::moveToNewLine() {
    while(true) {
        buffer=fgetc(dsv_file);
        if (buffer == '\n') return;
        if (buffer == EOF) return;
    }
}

long long int DSV_Analyser::GetCurrentPosition() const {
    return dsv_file ? ftell(dsv_file) : 0;
}

long long int DSV_Analyser::GetFileSize() const {
    return fileSize;
}

void DSV_Analyser::Analyse(bool _hasHeader = true) {
    std::vector<DSV_FieldInfo>().swap(Columns);
    hasHeader = _hasHeader;
    int result;
    ReadHeader();
    if (dsv_file != nullptr) fclose(dsv_file);
    dsv_file = fopen(filepath, "r");
    if (hasHeader) moveToNewLine();
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
    totalColumns = static_cast<int>(Columns.size());
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
        if (buffer==34) return getEnclosed();
        if (buffer=='\n') return DSV_TYPES::TEXT_TYPE;
        if (buffer==delimiter) break;
        if (buffer >= ASCII_LOW_NUMBER && buffer <= ASCII_HIGH_NUMBER && first_byte) {
            return getNumber();
        }
        first_byte = false;
        ++currentFieldLength;
    }
    return DSV_TYPES::TEXT_TYPE;
}

// detects INTEGER type the field
// if NOT returns DSV_TYPES::TEXT_TYPE
int DSV_Analyser::getNumber() {
    ++currentFieldLength;
    int result = DSV_TYPES::INTEGER_TYPE;
    while(true) {
        if (buffer==EOF) throw int(DSV_TYPES::INTEGER_TYPE);
        buffer = fgetc(dsv_file);
        //if (buffer=='\r') buffer=' ';
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
        //if (buffer=='\r') buffer=' ';
        if (buffer=='\n') return DSV_TYPES::DOUBLE_TYPE;
        if (buffer==delimiter) break;
        if (buffer < ASCII_LOW_NUMBER || buffer > ASCII_HIGH_NUMBER) result = DSV_TYPES::TEXT_TYPE;
        ++currentFieldLength;
    }
    return result;
}

int DSV_Analyser::getEnclosed() {
    bool enclosed = true;
    ++currentFieldLength;
    while(true) {
        if (buffer==EOF) throw int(DSV_TYPES::TEXT_TYPE );
        buffer = fgetc(dsv_file);
        if (!enclosed) {
            //if (buffer=='\r') buffer=' ';
            if (buffer==delimiter || buffer=='\n') break;
        }
        if (buffer==34) enclosed = !enclosed;
        ++currentFieldLength;
    }
    return DSV_TYPES::TEXT_TYPE;
}



// USEFUL FUNCTIONS

