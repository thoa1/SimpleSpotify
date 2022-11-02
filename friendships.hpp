#ifndef _FRIENDSHIPS_HPP_
#define _FRIENDSHIPS_HPP_

#include <map>
#include <string>
#include <queue>

#include "song.hpp"
#include "user.hpp"
#include "bstree.hpp"
using namespace std;

/**
 * Initially, users have no friendships, so a graph cannot be the only
 * data structure used because there will not be edges connecting the users.
 * 
 * We use a map<string, User*> to map the every user's name to his instance.
 * std::map has a O(log(n)) worst-case time complexity.
 * 
 */

class Friendships
{
private:
    // No need for map, BST has a search method we can use
    //map<string, User*> list;
    BSTree<User *> users;

public:
    
    /**
     * This is our implementation of BFS.
     * Calling this on a User* will return how far he is from the primary user
     * i.e: Immediate friends? 2nd, 3rd, 4th, friends?
     */
    int FindUserRadius(vector<User *> friends, User *user)
    {
        struct user_depth
        {
            User *usr;
            int depth = 0;
        };

        queue<struct user_depth> search_queue;
        vector<User *> explored;

        // Enqueue all the primary's users initial friends
        for (int i = 0; i < (int)friends.size(); i++)
            search_queue.push((struct user_depth){friends[i], 0});

        while (!search_queue.empty())
        {
            // pop the current item off the queue
            struct user_depth sud = search_queue.front();
            User *current = sud.usr;
            int depth = sud.depth;
            search_queue.pop();
            
            //cout << *current << " " << depth << endl;

            // check if he is the user we are looking for
            if (*(sud.usr) == *(user)) return sud.depth;

            // mark him as explored
            explored.push_back(current);

            // add all his friends to the queue
            vector<User *> *subfriends = current->GetFriends();
            for (int i = 0; i < (int)subfriends->size(); i++)
            {
                User *next = (*subfriends)[i];

                // Add if the user was not already explored
                if (find(explored.begin(), explored.end(), next) == explored.end())
                    search_queue.push((struct user_depth){next, depth + 1});
            }
        }
        
        // Returns -1 if user was never found
        return -1;
    }

    /* End BFS graph traversal */

    BSTree<User *> *GetUsers() { return &users; }

    bool Exists(string usr)
    {
        bool found = false;
        users.search(usr, &found);
        return found;
    }

    bool CreateFriendship(string a, string b)
    {
        bool found = false;
        User *userA = users.search(a, &found);
        User *userB = users.search(b, &found);

        if (!found) return 1;
        else
        {
            // Make sure they are not already friends, we want to prevent duplicates.
            if (userA->IsFriendsWith(userB) || userB->IsFriendsWith(userA)) return 1;

            userA->AddFriend(userB);
            userB->AddFriend(userA);
            return 0;
        }
    }

    // Removed the friendship between two users
    void RemoveFriendship(string a, string b)
    {
        bool found = false;
        User *userA = users.search(a, &found);
        User *userB = users.search(b, &found);

        if (found) RemoveFriendship(userA, userB);
    }

    void RemoveFriendship(User *a, User *b)
    {
        a->RemoveFriend(b);
        b->RemoveFriend(a);
    }

    void ShowFriends(string un)
    {
        bool found = false;
        User *usr = users.search(un, &found);

        if (!found) return;

        auto *friends = usr->GetFriends();
        for (int i = 0; i < (int)friends->size(); i++)
            cout << *(*friends)[i] << endl;
    }
    
    /**
     * It is unsafe to directly call remove on the BinarySearchTree
     * because we will then have dangling pointers
     */
    bool RemoveUser(string username, BSTree<User *> *primary_user_friends)
    {
        bool found = false;
        User *usr = users.search(username, &found);
        if (!found) return 1;
        else {
            // remove all his friendships among simulated users
            vector<User *> *friends = usr->GetFriends();
            for (int i = 0; i < (int)friends->size(); i++)
                ((*friends)[i])->RemoveFriend(usr);

            // remove friendship with primary user
            primary_user_friends->remove(username);

            // remove him from the system's user collection
            users.remove(username);
        }

        return 0;
    }
};

#endif // !_FRIENDSHIPS_HPP_