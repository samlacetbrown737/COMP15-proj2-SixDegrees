/*
 * main.cpp
 * 
 * Comp 15 Proj 2
 * Sam Lacet-Brown
 * Nov 2020
 *
 *
 */

#include "SixDegrees.h"
#include "Artist.h"
#include "CollabGraph.h"
#include <iostream>
#include <fstream>

using namespace std;

void artistOnly(char *argv[]) {
    ifstream artistList;
    artistList.open(argv[1]);
    if(!artistList.is_open()) {
        cerr << argv[1] << " cannot be opened." << endl;
        return;
    }
    SixDegrees *six = new SixDegrees(artistList);
    artistList.close();
    delete six;
    return;
}

void artistAndCommands(char *argv[]) {
    ifstream artistList;
    artistList.open(argv[1]);
    if(!artistList.is_open()) {
        cerr << argv[1] << " cannot be opened." << endl;
        return;
    }
    ifstream commandsList;
    commandsList.open(argv[2]);
    if(!commandsList.is_open()) {
        cerr << argv[2] << " cannot be opened." << endl;
        return;
    }
    SixDegrees *six = new SixDegrees(artistList, commandsList);
    artistList.close();
    commandsList.close();
    delete six;
    return;
}

void allInputs(char *argv[]) {
    ofstream output;
    output.open(argv[3]);
    ifstream artistList;
    artistList.open(argv[1]);
    if(!artistList.is_open()) {
        cerr << argv[1] << " cannot be opened." << endl;
        return;
    }
    ifstream commandsList;
    commandsList.open(argv[2]);
    if(!commandsList.is_open()) {
        cerr << argv[2] << " cannot be opened." << endl;
        return;
    }
    SixDegrees *six = new SixDegrees(artistList, commandsList, output);
    artistList.close();
    commandsList.close();
    delete six;
    return;
}

int main(int argc, char *argv[]) {
    if(argc == 2) {
        artistOnly(argv);
    } else if (argc == 3) {
        artistAndCommands(argv);
    } else if (argc == 4) {
        allInputs(argv);
    } else {
        cerr << "Usage: ./SixDegrees dataFile [commandFile] [outputFile]";
    }
    return 0;
}