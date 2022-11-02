#include <iostream>
#include <vector>
#include <assert.h>
#include <sstream>
#include "friendships.hpp"
#include "maxheap.hpp"
#include "bstree.hpp"
#include "song.hpp"
#include "user.hpp"
#include "parser.hpp"
#include "WordTokenizer.hpp"

#define OP(operation)   parser.getOperation() == operation
#define ARG1(arg1)      parser.getArg1() == arg1
#define ARG2(arg2)      parser.getArg2() == arg2

using namespace std;

string          vtos(vector<string>);
bool            file_exists(string path);
vector<Song *>  fileread();

vector<Song *> fileread()
{
    assert(file_exists("./songs/songs.txt"));

    WordTokenizer info_reader = WordTokenizer("./songs", "songs.txt");
    vector<Song *> songs;
    vector<string> nextline;
    do {
        nextline = info_reader.nextLine();

        if (nextline.size() == 0) break;

        songs.push_back(new Song(vtos(nextline)));
    } while(nextline.size() > 0);

    return songs;
}

bool file_exists(string path)
{
    ifstream ifs;
    ifs.open(path);
    return (ifs) ? true : false;
}

// Converts a vector of strings, into one string
string vtos(vector<string> vector)
{
	string str = "";
	for (int i = 0; i < (int)(vector.size()) - 1; i++) str += vector[i] + " ";
	str += vector[vector.size() - 1];
    str = str.substr(0, str.size()-2);
	return str;
}

int main()
{
    cout << "Welcome to CA5" << endl;

    Friendships network;
    vector<Song *> songs = fileread();
    BSTree<Song *> song_tree;
    MaxHeap song_plays(songs);
    string input_str;
    BSTree<Song *> p_song_tree;
    BSTree<User *> p_friends_tree;
    int efn_radius = 2;
    
    // Populate song_tree from the song vector
    for (int index = 0; index < (int)songs.size(); index++)
        song_tree.insert(songs[index]);

    for (;;)
    {
        cout << "\n: " << flush;
        getline(cin, input_str);
        Parser parser(input_str);
        const string arg1 = parser.getArg1(), arg2 = parser.getArg2();

        if (OP("user") && ARG1("add"))
        {
            string username = parser.getArg2();
            if (username != "")
            {
                cout << "Added user " << username << endl;
                network.GetUsers()->insert(new User(username));
            }
            else cout << "Syntax: user add <username>" << endl;
        }
        else if (OP("user") && ARG1("remove"))
        {
            string username = parser.getArg2();
            if (username != "")
            {
                cout << "Removed user " << username << endl;
                network.RemoveUser(username, &p_friends_tree);
            }else cout << "Syntax: user remove <username>" << endl;
        }
        else if (OP("friend"))
        {
            string a = parser.getArg1(), b = parser.getArg2();
            
            // 2 Usernames provided
            if (a != "" && b != "")
            {

                bool error = network.CreateFriendship(a, b);
                if (error)
                    cout << "Failed to create friendship." << endl;
                else
                    cout << "Created friendship between " << a << " and " << b << "." << endl;
                
            }
            // 1 username provided
            else if (a != "" && b == "")
            {
                bool found = false;
                User *usr = network.GetUsers()->search(a, &found);

                if (found) p_friends_tree.insert(usr);
                else cout << a << " does not exist." << endl;

            }else cout << "Syntax: friend <username> <username>\n"
                          "Creates friendship between two users. OR\n"
                          "Syntax: friend <username>\n"
                          "Creates a friendship between the primary user and others.\n";
        }
        // Check if user exists
        else if (OP("search") && ARG1("user"))
        {
            bool found = false;
            User *usr = network.GetUsers()->search(parser.getArg2(), &found);
            if (found) cout << "User " << *usr << " exists." << endl;
            else cout << "User not found" << endl;
        }
        // Check if song exists
        else if (OP("search") && ARG1("song"))
        {
            bool found = false;
            Song *s = song_tree.search(parser.getArg2(), &found);
            if (found) cout << "Song " << *s << " exists." << endl;
            else cout << "Song not found" << endl;
        }
        // Add song to system library
        else if (OP("addsong"))
        {
            if (parser.getArg1() != "\0"){
                song_tree.insert(new Song(parser.getArg1()));
                bool found = false;
                Song *s = song_tree.search(parser.getArg1(), &found);
                // TODO: Also add the song to the MaxHeap
                song_plays.insert(s, 0);
                cout << parser.getArg1() << " added to system" << endl;
            }
            else {
                cout << "Error song not added, specify title" << endl;
            }
        }
        else if (OP("unfriend")) {
            string a = parser.getArg1(), b = parser.getArg2();
        
            // User provided two usernames
            if (a != "" && b != "")
            {
                bool found = false;
                User *usr1 = network.GetUsers()->search(a, &found);
                User *usr2 = network.GetUsers()->search(b, &found);
                if(!found){
                    cout << "A user was not found" << endl;
                    continue;
                }
                usr1->RemoveFriend(usr2);
                usr2->RemoveFriend(usr1);
                cout << "Friendship of " << a << " and " << b << " removed " << endl;
            }
            // One user name provided; remove friend from primary user
            else if (a != "" && b == "")
            {
                p_friends_tree.remove(a);
            }else cout << "Syntax: unfriend <username> <username>\n"
                          "Removes friendship between two users. OR\n"
                          "Syntax: unfriend <username>\n"
                          "Removes a friendship between the primary user and others.\n";
        }
        // Makes a user listen to a song and increments it's count
        else if (OP("listen")){
            
            if(parser.getArg1() != "" && parser.getArg2() != "")
            {
                string usrname = parser.getArg1(), song = parser.getArg2();
                
                if (!network.Exists(usrname))
                {
                    cout << usrname << " not found." << endl;
                    continue;
                }

                // Get the user's instance
                bool found = false;
                User *usr = network.GetUsers()->search(usrname, &found);

                // Check if the user is within radius
                auto p_friends_vector = p_friends_tree.as_vector();
                int depth = network.FindUserRadius(p_friends_vector, usr);

                if (depth >= 0)
                    cout << usrname << " is " << depth << " friends away from primary user.\n";

                // If the user is within the EFN radius, count his listen
                if (depth < efn_radius && depth >= 0) song_plays.CountPlay(song, 1);
            
            }else cout << "Syntax: listen <username> <song>\n";

        }
        // Grab reccomended songs from heap
        else if(OP("recommend")){
            if(parser.getArg1() != "" && arg2 == ""){
                int num = stoi(arg1);
                
                // Ensure there are enough songs to recommend
                if(num >= song_plays.getCapacity()) continue;

                cout << "Songs that are recommended: " << endl;
                for ( int i = 0; i < num; i++){
                    cout << song_plays.HighestPlayedSong()->GetTitle() << endl;
                    p_song_tree.insert(song_plays.HighestPlayedSong());
                    song_plays.heap_extract_max();
                }
            } else cout << "Syntax: recommend <N> " << endl;
        }
        // Sets the efn radius
        else if (OP("efn") && arg1 != "") {
            efn_radius = stoi(arg1);
            cout << "EFN Radius is " << efn_radius << endl;
        }
        else if (OP("efn") && arg1 == "") cout << "EFN Radius is " << efn_radius << endl;
        else if (OP("removesong") && arg1 != "" && arg2 == ""){
            cout << "Removing " << arg1 << " from primary song list" << endl;
            bool found = false;
            Song *s = p_song_tree.search(arg1, &found);
            if (found) cout << "Song " << *s << " exists." << endl;
            else cout << "Song not found" << endl;
            p_song_tree.remove(arg1);
            song_plays.insert(s, 0);
        }
        else if (OP("show") && ARG1("users")) network.GetUsers()->print();
        else if (OP("show") && ARG1("friends"))
        {
            if (!p_friends_tree.empty())
                cout << "Primary User's Friends: " << endl, p_friends_tree.print();

            if (arg2 != "")
            {
                cout << endl << arg2 << "'s Friends:\n";
                network.ShowFriends(arg2);
            } 
        }
        else if (OP("show") && ARG1("psongs") && arg2 == "") cout << "Primary User's Songs: " << endl, p_song_tree.print();
        else if (OP("show") && ARG1("songs")) song_plays.print();
        else if (OP("exit") || OP("quit")) break;
        else if (OP("debug_heap")) song_plays.debug_print();
        else if (parser.getOperation() != "") cout << "Input not recognized.\n";
    }


    return 0;
}
