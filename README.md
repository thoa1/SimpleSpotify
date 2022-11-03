# SimpleSpotify
 


## A. Making and Breaking Friendships 
 Add users, make friendships, break friendships, and show friendships
 Friendships of a user should be stored with his instance
## B. Adding and Moving Songs `35 points`
 Add songs to system library and show them
 Add songs from system library to primary user library (via recommend), show the primary user's library
 Show main system library after some songs have been moved to primary user's library
 Remove song from primary user's library, show that they go back to system, with count of 0
 Song containers should be indexed by a BST (BinarySearchTree) to look up faster than O(N)
## C. Tracking Listens 
 Show that you can build a network of friends, set EFN to 2
 Have users within the network listen to songs, causing the counts on the heap to go up (and show the songs w/ their plays)
 When users outside the network (EFN) listen to the song, the count should not go up
  Implement a BFS (BreadthFirstSearch)
## D. Making Recommendations `35 points`
  Build up a heap using add song and listen, show the heap (displaying the songs and counts)
  Get recommendations for many songs at a time. Show that they come off the top of the heap and are added to user library
  Listen to a song after is has been moved within the heap to show that the BST can still find it and increase listen count
  Show that code for this functionality is implemented in a heap. Show the spot in the code where heap updates the BST index.
