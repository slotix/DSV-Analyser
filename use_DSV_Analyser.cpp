//
// Created by Sn0w1eo on 07.12.2017.
//
#include "DSV_Analyser.h"
#include <iomanip>
#include <iostream>
#include <thread>
#include <mutex>


using namespace std;

void printProgressBar(const DSV_Analyser& obj) {
    int percent = 0;
    long long int currentPosition = 0;
    long long int fSize = obj.GetFileSize();
    while(true) {
        currentPosition = obj.GetCurrentPosition();
        if (currentPosition == EOF) {
            cout << '\r' << '[' << left << setw(100) << string(100, '#') << "] " << fSize << '/' << fSize << std::flush;
            break;
        }
        percent=static_cast<int>(currentPosition/(double)fSize * 100);
        cout << '\r' << '[' << left << setw(100) << string(percent, '#') << "] " << currentPosition << '/' << fSize << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

}


int main() {
    string filename = R"(D:\Trash\csv_files\10Million.csv)";
//    string filename = R"(D:\Trash\csv_files\sample.csv)";
//      string filename = R"(D:\Trash\my.csv)";

    cout << "ANALYSE STARTED..." << endl;
    clock_t start = clock();
    DSV_Analyser obj(filename.c_str() , '\t', '.' );
    long long int fileSize = obj.GetFileSize();
    cout << "FILESIZE: " << fileSize << endl;
//    obj.Analyse(true);
    std::thread analyser(&DSV_Analyser::Analyse, &obj, true);
    std::thread printProgress(&printProgressBar, std::cref(obj));
    printProgress.join();
    analyser.join();

    cout << endl;
    clock_t finish = clock();
    double elapsed_time = double(finish-start)/1000;
    cout << "TIME ELAPSED IN SEC: " << elapsed_time << endl;
    cout << "STATISTICS:" << endl;
    cout << "Total Columns:" << obj.Columns.size() << endl << endl;
    cout << setw(10) << right << "#" << "  " << setw(60) << left << "TITLE"
         << setw(10) << right << "LENGTH" << setw(10) << "TYPE" << endl;
    cout << setw(92) << setfill('-') << '-' << setfill(' ') << endl;
    for (int i=0; i < obj.Columns.size(); i++) {
        cout << setw(10) <<  right << i << "  " << setw(60) << left << setfill('.') << obj.Columns[i].Title
             << setw(10) << right << obj.Columns[i].Length << setw(10) << setfill(' ');
        if (obj.Columns[i].Type & DSV_TYPES::TEXT_TYPE) {
            cout << "Text";
        } else if(obj.Columns[i].Type & DSV_TYPES::DOUBLE_TYPE) {
            cout << "Double";
        } else if(obj.Columns[i].Type & DSV_TYPES::INTEGER_TYPE) {
            cout << "Number";
        } else {
            cout << "Undefined";
        }
        cout << endl;
    }
    cout << endl;
}
