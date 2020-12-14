/*
 * SixDegrees.cpp
 * 
 * Comp 15 Proj 2
 * Sam Lacet-Brown
 * Nov 2020
 *
 * This program finds a path between two artists, with optional exclusions.
 * This is similar to Six Degrees of Kevin Bacon, but with artists and songs
 * instead of actors and movies. It can be run to return either the first path
 * it finds or specfically the shortest one.
 */

#include "SixDegrees.h"
#include "Artist.h"
#include "CollabGraph.h"
#include <queue>
#include <stack>
#include <vector>
using namespace std;

/* SixDegrees
 *    Purpose: Initalizes a SixDegrees instance
 *    Parameters: An istream of artists and songs
 *    Returns: none
 */
SixDegrees::SixDegrees(istream &artistsFile) {
    addArtists(artistsFile, cout);
    parseCommands(cin, cout);
}

/* SixDegrees
 *    Purpose: Initalizes a SixDegrees instance with commands
 *    Parameters: An istream of artists and songs, and an istream of commands
 *    Returns: none
 */
SixDegrees::SixDegrees(istream &artistsFile, istream &commandFile) {
    addArtists(artistsFile, cout);
    parseCommands(commandFile, cout);
}

/* SixDegrees
 *    Purpose: Initalizes a redirected SixDegrees instance with commands
 *    Parameters: An istream of artists and songs, an istream of commands,
 *      and an ostream of where to redirect output to
 *    Returns: none
 */
SixDegrees::SixDegrees(istream &artistsFile, istream &commandFile, 
    ostream &outputFile) {
    addArtists(artistsFile, outputFile);
    parseCommands(commandFile, outputFile);
}

// deconstructor
SixDegrees::~SixDegrees() {
    auto i = singers.begin();
    for( ; i != singers.end(); i++) {
        delete (*i);
    }
}

/* addArtists
 *    Purpose: Populates the CollabGraph with artists and their songs/collabs
 *    Parameters: An istream of artists and songs, and an output stream
 *    Returns: none
 */
void SixDegrees::addArtists(istream &filename, ostream &pout) {
    string line;
    Artist *singer, *addTo; //addTo is used only if the artist is a duplicate
    bool newSinger = true;
    while(getline(filename, line)) {
        if(newSinger) {
            singer = new Artist(line);
            addTo = findArtist(line, pout, true); //silent search
            newSinger = false;
        } else {
            if (line == "*") {
                graph.insert_vertex(*singer);
                auto i = singers.begin();
                for( ; i != singers.end(); i++) {
                    if((*i)->get_name() != singer->get_name()) {
                        string collab = singer->get_collaboration(*(*i));
                        if(collab != "") {
                            graph.insert_edge(*singer, *(*i), collab);
                        }
                    }
                }
                singers.push_back(singer);  
                newSinger = true;
            } else {
                singer->add_song(line);
                if(addTo != nullptr) {
                    addTo->add_song(line);
                }
            }
        }
    }
    graph.print_graph(pout);
}

/* parseCommands
 *    Purpose: Redirect to the correct function for the input
 *    Parameters: An istream of commands, and an output stream
 *    Returns: none
 */
void SixDegrees::parseCommands(istream &input, ostream &pout) {
    string command;
    input >> command;
    if(input.eof()) {
        run = -1;
    }
    while (run != -1 and command != "quit") {
        if (command == "bfs") {
            bfs(input, pout);
        } else if (command == "dfs") {
            dfs(input, pout);
        } else if (command == "not") {
            notBFS(input, pout);
        } else {
            pout << command << " is not a command. Please try again." << endl;
        }
        input >> command;
        if(input.eof()) {
            run = -1;
        }
    } 
}

/* bfs
 *    Purpose: Gathers values for overloaded bfs function
 *    Parameters: An istream of artists, and an output stream
 *    Returns: none
 */
void SixDegrees::bfs(istream &input, ostream &pout) {
    string temp, a, b;
    getline(input, temp);
    getline(input, a);
    getline(input, b);
    Artist *artA, *artB;
    artA = findArtist(a, pout, false);
    artB = findArtist(b, pout, false);
    if(artA and artB) {
        bfs(artA, artB, pout);
    }
}

/* bfs
 *    Purpose: Searches for the shortest path between two Artists
 *    Parameters: Two Artist points and an output stream
 *    Returns: none
 */
void SixDegrees::bfs(Artist *a, Artist *b, ostream &pout) {
    queue<Artist> explore;
    Artist current;
    vector<Artist> neighbors;
    startFrom(a);
    explore.push(*a);
    while(not explore.empty()) {
        current = explore.front();
        explore.pop();
        neighbors = graph.get_vertex_neighbors(current);
        auto size = neighbors.size()-1;
        for(int i = 0; i <= (int)size; i++) {
            if(not graph.is_marked(neighbors[i])) {
                graph.mark_vertex(neighbors[i]);
                if(neighbors[i].get_name() == b->get_name()) {
                    reachedGoal(neighbors[i], current, a, b, pout);
                    return;
                }
                graph.set_predecessor(neighbors[i], current);
                explore.push(neighbors[i]);
            }
        }
    }
    noPath(a, b, pout);
}

/* dfs
 *    Purpose: Gathers values for overloaded dfs function
 *    Parameters: An istream of artists, and an output stream
 *    Returns: none
 */
void SixDegrees::dfs(istream &input, ostream &pout) {
    string temp;
    string a, b;
    getline(input, temp);
    getline(input, a);
    getline(input, b);
    Artist *artA, *artB;
    artA = findArtist(a, pout, false);
    artB = findArtist(b, pout, false);
    if(artA and artB) {
        dfs(artA, artB, pout);
    }
}

/* dfs
 *    Purpose: Searches for a path between two Artists
 *    Parameters: Two Artist points and an output stream
 *    Returns: none
 */
void SixDegrees::dfs(Artist *a, Artist *b, ostream &pout) {
    stack<Artist> explore;
    Artist current;
    vector<Artist> neighbors;
    startFrom(a);
    explore.push(*a);
    while(not explore.empty()) {
        current = explore.top();
        explore.pop();
        neighbors = graph.get_vertex_neighbors(current);
        auto size = neighbors.size()-1;
        for(int i = 0; i <= (int)size; i++) {
            if(not graph.is_marked(neighbors[i])) {
                graph.mark_vertex(neighbors[i]);
                if(neighbors[i].get_name() == b->get_name()) {
                    reachedGoal(neighbors[i], current, a, b, pout);
                    return;
                }
                graph.set_predecessor(neighbors[i], current);
                explore.push(neighbors[i]);
            }
        }
    }
    noPath(a, b, pout);
}

/* notBFS
 *    Purpose: Gathers values for overloaded notBFS function
 *    Parameters: An istream of artists, and an output stream
 *    Returns: none
 */
void SixDegrees::notBFS(istream &input, ostream &pout) {
    string temp, a, b, c;
    vector<Artist*> exclude;
    getline(input, temp);
    getline(input, a);
    getline(input, b);
    getline(input, c);
    Artist *artA, *artB, *artC;
    artA = findArtist(a, pout, false);
    artB = findArtist(b, pout, false);
    while(c != "*" and c != "quit") {
        artC = findArtist(c, pout, false);
        if(artC) {
            exclude.push_back(artC);
        }
        getline(input, c);
    }
    if(c == "quit") {
        run = -1;
        return;
    }
    if(artA and artB) {
        notBFS(artA, artB, exclude, pout);
    }
}

/* notBFS
 *    Purpose: Searches for the shortest path between two Artists that excludes
 *       a list of Artists
 *    Parameters: Two Artists, a vector of excluded Artists and a output stream
 *    Returns: none
 */
void SixDegrees::notBFS(Artist *a, Artist *b, 
vector<Artist*> exclusions, ostream &pout) {
    queue<Artist> explore;
    Artist current;
    vector<Artist> neighbors;
    startFrom(a);
    explore.push(*a);
    while(not explore.empty()) {
        current = explore.front();
        explore.pop();
        neighbors = graph.get_vertex_neighbors(current);
        for(int i = 0; i <= (int)neighbors.size()-1; i++) {
            bool outOfList = true;
            for(size_t j = 0; j < exclusions.size(); j++) {
                string bannedArtist = exclusions[j]->get_name();
                 if(neighbors[i].get_name() == bannedArtist) {
                     outOfList = false;
                 }
            }
            if(not graph.is_marked(neighbors[i]) and outOfList) {
                graph.mark_vertex(neighbors[i]);
                if(neighbors[i].get_name() == b->get_name()) {
                    reachedGoal(neighbors[i], current, a, b, pout);
                    return;
                }
                graph.set_predecessor(neighbors[i], current);
                explore.push(neighbors[i]);
            }
        }
    }
    noPath(a, b, pout);
}

/* noPath
 *    Purpose: Prints result if there is no path between two Artists
 *    Parameters: A starting and ending Artis and a output stream
 *    Returns: none
 */
void SixDegrees::noPath(Artist *a, Artist *b, ostream &pout) {
    pout << "A path does not exist between \"" << a->get_name();
    pout << "\" and \"" << b->get_name() << "\"." << endl;
}

/* printOut
 *    Purpose: Displays the path found between two artists
 *    Parameters: A stack of Artists with the starting artist at the top
 *      of the stack and the second to last artist at the bottom, and a
 *      pointer to the first and last artists and an output stream
 *    Returns: none
 */
void SixDegrees::printOut(stack<Artist> path, Artist *a,
    Artist *b, ostream &pout) {
    if(path.empty()) {
        pout << "A path does not exist between \"" << a->get_name();
        pout << "\" and \"" << b->get_name() << "\"." << endl;
        return;
    }
    path.pop();
    Artist curr, prev;
    while(not path.empty()) {
        curr = path.top();
        prev = graph.get_predecessor(curr);
        pout << "\"" << prev.get_name() << "\"";
        pout << " collaborated with \"" << curr.get_name() << "\"";
        pout << " in \"";
        pout << curr.get_collaboration(prev);
        pout << "\"." <<  endl;
        path.pop();
    }
    pout << "\"" << curr.get_name() << "\"";
    pout << " collaborated with \"" << b->get_name() << "\"";
    pout << " in \"";
    pout << b->get_collaboration(curr);
    pout << "\"." <<  endl;
    pout << "***" << endl;
}


/* startFrom
 *    Purpose: Resets the graph
 *    Parameters: An Artist to start from
 *    Returns: none
 */
void SixDegrees::startFrom(Artist *a) {
    graph.clear_metadata();
    graph.mark_vertex(*a);
}

/* reachedGoal
 *    Purpose: Prints the path between two Artists, either directly if the two
 *      worked together or via the printOut function if there is a path
 *    Parameters: The last visited Artist, the goal Artist, a pointer to the
 *      starting Artist, a pointer to the ending Artist, and an output stream
 *    Returns: none
 */
void SixDegrees::reachedGoal(Artist neighbor, Artist current, 
    Artist *a, Artist *b, ostream &pout) {
    if(graph.report_path(*a, current).empty()) {
        pout << "\"" << current.get_name() << "\"";
        pout << " collaborated with \"";
        pout << neighbor.get_name() << "\"";
        pout << " in \"";
        pout << neighbor.get_collaboration(current);
        pout << "\"." <<  endl;
        pout << "***" << endl;

        cout << "neigh" << neighbor.get_name();
        cout << "B" << b->get_name();
    }  else {
        printOut(graph.report_path(*a, current), a, b, pout);
    }
}

/* findArtist
 *    Purpose: Find an Artist given their name as a string
 *    Parameters: A string with the name of an artist to find
 *      and an output stream, and a boolean to determine if there
 *      should be output printed
 *    Returns: A pointer to the Artist vertex
 */
Artist* SixDegrees::findArtist(string name, ostream &pout, bool silent) {
    if(name == "quit") {
        run = -1;
        return nullptr;
    }
    if(not singers.empty()) {
        auto i = singers.begin();
        for( ; i != singers.end(); i++) {
            if((*i)->get_name() == name) {
                return (*i);
            }
        }
        if(not silent) {
          pout << "\"" << name << "\" was not found in the dataset :(" << endl;
        }
    }
    return nullptr; 
}