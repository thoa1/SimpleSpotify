#ifndef _SONG_HPP_
#define _SONG_HPP_

#include <iostream>
#include <cstdbool>
#include <algorithm>
#include <string>
#include <ctime>

using namespace std;

class Song
{
private:
    /**
     * Only title needed for this project
     */
    string title;

    string to_lower(string tmp)
    {
        transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
        return tmp;
    }


public:
    Song() = default;
    
    /**
     * Creates a Song with title
     */
    Song(string _title)
    {
        title = _title;
    }
    

    const string GetTitle() { return title; }

    // Decides equality based on whether the titles match.
    bool operator==(const string &o) { if (o == title) return true; else return false; }
    bool operator==(const Song &o) { if (o.title == title) return true; else return false; }

    /* Comparison Operators for Sorting in Alphabetical Order */
    bool operator<(const Song &left)
    {
        return to_lower(left.title).compare(to_lower(title)) < 0 ? false : true;
    }

    bool operator>(const Song &left)
    {
        return to_lower(left.title).compare(to_lower(title)) > 0 ? false : true;
    }

    bool operator<(const string &left)
    {
        return to_lower(left).compare(to_lower(title)) < 0 ? false : true;
    }

    bool operator>(const string &left)
    {
        return to_lower(left).compare(to_lower(title)) > 0 ? false : true;
    }

    friend ostream &operator<<(ostream &os, const Song &song)
    {
        if (song.title == "")
        {
            os << "Song does not exist.";
        }else
        {
            os << song.title << ".";
        }
        return os;
    }
};

#endif // !_SONG_HPP_