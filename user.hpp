#ifndef _USER_HPP_
#define _USER_HPP_

#include <string>
#include <vector>

#include "song.hpp"
#include "user.hpp"

using namespace std;

class User
{
private:
    string username;
    vector<User *> friends;

    string to_lower(string tmp)
    {
        transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
        return tmp;
    }
public:

    /**
     * User class constructor.
     */
    User(string _username)
    {
        username = _username;
    }

    /**
     * Returns true if this User is friends with other User.
     */
    bool IsFriendsWith(User *other)
    {
        for (int i = 0; i < (int)friends.size(); i++)
            if (friends[i] == other) return true;
        
        return false;
    }

    void AddFriend(User *other)
    {
        //cout << this << " pushing back " << *other << endl;
        friends.push_back(other);
    }

    void RemoveFriend(User *other)
    {
        for (int i = 0; i < (int)friends.size(); i++)
            if (*(friends[i]) == *other)
                friends.erase(friends.begin() + i);
    }

    string GetUsername() { return username; }

    vector<User *> *GetFriends()
    {
        return &friends;
    }

    /* Comparison Operators for Sorting in Alphabetical Order */
    bool operator<(const User &left)
    {
        return to_lower(left.username).compare(to_lower(username)) < 0 ? false : true;
    }

    bool operator>(const User &left)
    {
        return to_lower(left.username).compare(to_lower(username)) > 0 ? false : true;
    }

    bool operator<(const string &str)
    {
        return to_lower(str).compare(to_lower(username)) < 0 ? false : true;
    }

    bool operator>(const string &str)
    {
        return to_lower(str).compare(to_lower(username)) > 0 ? false : true;
    }

    bool operator==(const string &str) { if (str == username) return true; else return false; }
    bool operator==(const User &user) { if (user.username == username) return true; else return false; }

    friend ostream &operator<<(ostream &os, const User &user)
    {
        if (user.username == "")
            os << "User does not exist.";
        else
            os << user.username;
        return os;
    }
};

#endif // !_USER_HPP_