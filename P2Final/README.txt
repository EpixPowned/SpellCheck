Project 2 | Spell Checker | Paul Cantave(pdc78) and Omar Waseem(omw11) | CS214

Hello, and welcome to our spell checker for CS214. Our spell checker uses a trie data structure to check for a word from a provided dictionary, and read if it is within the dictionary as a valid word or not. 
We chose a trie data structure because we believed that it would be the most efficient way to read through the dictionary and compare it with the words within the txt file given to the program.

Our main test files are: 
hi.txt : tests of different words to see if our read_file function correctly checks and reports errors in our text file
test.txt : Used mainly for line and column placement testing
test2.txt : used mainly for directory testing
test3.txt : another example of hi.txt (also used for directory testing)


We used many different directories inside (TestDirectory) and outside P2 to test if text files were being read. 

In our main function, we use traverse_directory() to read through any directories in the arguments, and read_file() if it is just a txt file which is in the argument. 
Using our trieinsert() function, we insert each word with the variations which are accepted into our data structure so that we can check each word for its correctness. 

To operate our function, please follow the instructions:

1. type and run tar -xf p2.tar
2. type and run cd P2
3. type and run make in the terminal
4. type ./spchk <dictionary> <file1> <file2>, etc. for however many files/directories you wish to spell check. 
5. Results will print in the terminal in the following format: <filepath> (line, column): incorrect word. 

Thank you for using our spell check!

