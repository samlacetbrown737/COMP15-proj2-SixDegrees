//use proj1 to get input/output from files and limit params

//should there be an array for each artist that holds the songs
//they've done or an array for each song that says if another artist
//has done it before

//BFS makes sense for not since you want the shortest path

//Incl will just run bfs from a->c and then  c->b [not a]

/*
 * SixDegrees.h
 * 
 * Comp 15 Proj 2
 * Sam Lacet-Brown
 * Nov 2020
 *
 *
 */

#ifndef __SIXDEGREES_H__
#define __SIXDEGREES_H__

#include "Artist.h"
#include "CollabGraph.h"
#include <string>
#include <iostream>
#include <stack>

using namespace std;
class SixDegrees {
    public:
        SixDegrees(istream &artistsFile);
        SixDegrees(istream &artistsFile, istream &commandFile);
        SixDegrees(istream &artistsFile, istream &commandFile, 
            ostream &outputFile);
        ~SixDegrees();
    private:
        void addArtists(istream &filename, ostream &pout);
        void parseCommands(istream &input, ostream &pout);
        void bfs(istream &input, ostream &pout);
        void bfs(Artist *a, Artist *b, ostream &pout);
        void dfs(istream &input, ostream &pout);
        void dfs(Artist *a, Artist *b, ostream &pout);
        void notBFS(istream &input, ostream &pout);
        void notBFS(Artist *a, Artist *b, 
           vector<Artist*> exclusions, ostream &pout);
        Artist* findArtist(string name, ostream &pout, bool silent);
        void printOut(stack<Artist> path, Artist *a, Artist *b, ostream &pout);
        void reachedGoal(Artist neighbor, Artist current, 
          Artist *a, Artist *b, ostream &pout);
        void noPath(Artist *a, Artist *b, ostream &pout);
        void startFrom(Artist *a);
        CollabGraph graph;
        vector<Artist*> singers;
        int run = 1;

        //testing functions
        //Artist getTop(stack<Artist> v);
        //void displayExplore(stack<Artist> v);
};

#endif