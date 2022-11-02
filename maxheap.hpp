#ifndef _MAXHEAP_HPP_
#define _MAXHEAP_HPP_

/**
 * References:
 * Binary Heap - Insert, Sift Up, Delete, Sift Down, Heapify
 * https://www.youtube.com/watch?v=Dvq-YKeuO9Y
 */

#include <string>
#include <vector>

#include "song.hpp"

#define CAP_M 5

using namespace std;

class MaxHeap
{
private:

    /**
     * The song_info struct holds:
     * - A reference to a song.
     * - An int counting how many times that song has been listened to.
     */
    struct song_info {
        Song *pSong = nullptr;
        int cPlays = 0;
    };

    /**
     * Dynamically allocated array holding song_info nodes
     */
    struct song_info *arrNodes = nullptr;

    /* Number of elements supported by heap's current size */
    int capacity = 0;
    
    /* Number of current elements */
    int count = 0;

    inline int parent(int pos)      { return (pos - 1) / 2; }
    inline int left_child(int pos)  { return (2 * pos) + 1; }
    inline int right_child(int pos) { return (2 * pos) + 2; }

    inline void swap(int a, int b)
    {
        struct song_info tmp = arrNodes[a];
        arrNodes[a] = arrNodes[b];
        arrNodes[b] = tmp;
    }

    void max_heapify(int i){
    
        int l = left_child(i); // left = 2*i + 1
        int r = right_child(i); // right = 2*i + 2
        int largest = i;
            
        if (l <= capacity && arrNodes[l].cPlays > arrNodes[largest].cPlays)
            largest = l;
             
        if (r <= capacity && arrNodes[r].cPlays > arrNodes[largest].cPlays)
            largest = r;
        
            // If largest is not root
        if (largest != i) {
            swap(i, largest);
        
            // Recursively heapify the affected sub-tree
            max_heapify(largest);
        }


    }

    void sift_up(int index)
    {
        if (arrNodes[parent(index)].cPlays < arrNodes[index].cPlays)
            swap(index, parent(index)), parent(index) >= 0 ? sift_up(parent(index)) : sift_up(0);
    }

    
public:
    inline void insert(Song *song, int plays)
    {
        arrNodes[count].pSong = song;
        arrNodes[count].cPlays = plays;
        sift_up(count);
        count++;
        capacity++; // TODO: Temporary fix, should really change extractMax() to dercement count exclusively.
    }

    void insertnode(Song *song){
        
        count++;
        int i = count++;
        arrNodes[i].pSong = song;
        arrNodes[i].pSong = 0;
        while(i != 0 && arrNodes[parent(i)].cPlays < arrNodes[i].cPlays){
            swap(i, parent(i));
            i = parent(i);
        } 
    }

    ~MaxHeap()
    {
        delete[] arrNodes;
    }

    /**
     * Max Heap class constructor.
     * capacity: Number of songs to track plays for
     */
    MaxHeap(const vector<Song *>& v)
    {
        capacity = v.size();
        arrNodes = new struct song_info[CAP_M * v.size()];

        // Import all the songs in the binary search tree into the heap.
        for(unsigned index = 0; index < (unsigned)v.size(); index++)
            insert(v[index], 0);
    }

    /**
     * Increments the Plays counter for the song.
     * Does nothing if song_title is not found.
     */
    void CountPlay(string song_title, int n)
    {
       for (int i = 0; i < count; i++)
            if(song_title == arrNodes[i].pSong->GetTitle()){
                IncreaseKey(i, n);
            }
    }

    void IncreaseKey(int i, int key)
    {
        arrNodes[i].cPlays += key;
        while(i > 0 && arrNodes[i/2].cPlays < arrNodes[i].cPlays){   
            swap(i, i/2);
            i = i/2;
        }
    }

    void GetPlays(string song_title);

    Song *HighestPlayedSong() { return arrNodes[0].pSong; }

    void debug_print()
    {
        for (int i = 0; i < count; i++)
            cout << "Node #" << i << " Has song " << *(arrNodes[i].pSong) << " Number of Plays " << (arrNodes[i].cPlays) << endl;

        int j = 0;
        while (j < count && arrNodes[right_child(j)].pSong != nullptr)
        {
            cout << "Right child of Node #" << j << " is #" << right_child(j) << "(" << *(arrNodes[right_child(j)].pSong) << ")." << endl;
            j = right_child(j);
        }
    }
    void print()
    {
        int power = 0;
        int value = 1;
        for(int i = 0; i < count; i++){
            if(i == value){
                cout << endl;
                power += 1;
                value += (1 << power);
            }
            cout << arrNodes[i].cPlays << "\t" << arrNodes[i].pSong->GetTitle() << endl;
        }
        
    }
    
    Song* heap_extract_max()
    {
        if(capacity < 1) return nullptr;

        Song* max = arrNodes[0].pSong;
        arrNodes[0] = arrNodes[count-1];
        capacity --;
        count--;
        max_heapify(0);
        return max;
    }

    int getCapacity() { return capacity;}


};

#endif // !_MAXHEAP_HPP_