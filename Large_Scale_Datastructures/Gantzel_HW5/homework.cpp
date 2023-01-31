#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cmath>
#include <array>
#include <stack>
#include "covidgenome.h"
using namespace std;

//cd /mnt/c/Users/Gantzel/Desktop/Datastructures/Gantzel_HW5

char randomLetter();

class Trie {
	public:
		Trie * children[4];
    bool endOfTrie;

    Trie() {
      bool endOfTrie = false;
      for (int i=0; i<4; i++) {
        children[i] = nullptr;
      }
    }

		Trie(char ** sequences, int sequenceNumber, int sequenceLength) {
			bool endOfTrie = false;
      for (int i=0; i<4; i++) {
        children[i] = nullptr;
      }

			for (int i=0; i<sequenceNumber; i++) {
				insert(sequences[i], sequenceLength);
			}
		}

		Trie(const Trie &templ) {
			Trie * current = this;

			for (int i=0; i<4; i++) {
        current->children[i] = templ.children[i];
      }

		}

    int key(char input) {
      if (input == 'A') {
        return 0;
      }
      else if (input == 'C') {
        return 1;
      }
      else if (input == 'G') {
        return 2;
      }
      else if (input == 'T') {
        return 3;
      }
      else {
        cout << "invalid character" << endl;
        exit(-1);
      }
    }

    void insert(char * seq, int length) {
      Trie * current = this;

      for (int i=0; i<length; i++) {

        if (current->children[key(seq[i])] == nullptr) {
          current->children[key(seq[i])] = new Trie();
        }

        current = current->children[key(seq[i])];
      }

      current->endOfTrie = true;
    }

    bool regSearch(char * seq, int length) {
      Trie * current = this;

      if (current == nullptr) {
        return false;
      }

      for (int i=0; i<length; i++){
        current = current->children[key(seq[i])];

        if (current == nullptr) {
          return false;
        }
      }

      return current->endOfTrie;
    }

		bool fuzzySearch(char * seq, int length, int substitutions) { //the fuzzy search. It works recursively, so it takes number of substitutions so far as input
			Trie * current = this;
			bool found = false;

      if (current == nullptr) { //if empty, return false
        return false;
      }
			if (substitutions > 1) { //if too many substitutions have exceeded the maximum allowed, return false
				return false;
			}
			if (current->endOfTrie == true) { //if we have reached the end of the sequence, return true
				return true;
			}

			char remaining[length-1]; //create sting of the remaining letters (all other letters in the sequence than the first one)
			for (int i=0; i<(length-1); i++) {
				remaining[i] = seq[i+1];
			}

			for (int i=0; i<4; i++) { //if the trie has children, run the fuzzySearch function on them. If there is a mismatch, add 1 to substitutions
				if (i == key(seq[0])) {
					if (current->children[i] != nullptr) {
						bool next = current->children[i]->fuzzySearch(remaining, length-1, substitutions);
						if (next == true) {
							found = true;
						}
					}
				}
				else {
					if (current->children[i] != nullptr) {
						int newSubs = substitutions + 1;
						bool next = current->children[i]->fuzzySearch(remaining, length-1, newSubs);
						if (next == true) {
							found = true;
						}
					}
				}
			}

			return found;

		}

		int countNodes() {
			Trie * current = this;
			int nr = 0;

			if (current == nullptr) {
				return nr;
			}

			for (int i=0; i<4; i++) {
				nr = nr + current->children[i]->countNodes();
			}

			return nr+1;
		}

		~Trie(){
			for (int i=0; i<4; i++) {
				delete children[i];
			}
		}
}
;


int main(int argc, char ** argv){

	if (argc != 3){  // unexpected program call
		cout << endl << endl << "=========================="<< endl;
		cout << "Error: 3 input parameters expected" <<endl;
		cout << "Proper usage is:" <<endl;
		cout << "./homework <problem-flag> <number of random 36-mers>" << endl;
		cout << "Example:" << endl;
		cout << "./homework Problem1A 50000"  << endl;
		cout << "=========================="<< endl << endl;
		cout << "exiting..." << endl;
		exit(-1);
	}else{

		cout << "The number of arguments passed: " << argc << endl;
		cout << "The first argument is: " << argv[0] << endl;
		cout << "The second argument is: " << argv[1] << endl;
		cout << "The third argument is: " << argv[2] << endl << endl;
	}


	if(strcmp(argv[1], "Problem1A") == 0) {

		int numberOfRandoms = atoi(argv[2]);

		char * COVIDGen = new char[29903];
		COVIDGen = COVID_Genome();

		Trie mainTrie;

		char randomSeq[36];
		for (int i=0; i<numberOfRandoms; i++) {
			int randomStart = rand() % (29903 - 35);
			for (int j=0; j<36; j++) {
				randomSeq[j] = COVIDGen[randomStart+j];
			}
			mainTrie.insert(randomSeq, 36); //i insert the sequences as they are created
		}

		cout << "the trie contains " << mainTrie.countNodes() << " nodes" << endl;

		int covid_mers_length = 29903 - 35;
		char ** covid_mers = new char*[covid_mers_length];
		for (int i=0; i<covid_mers_length; i++) {
			covid_mers[i] = new char[36];
			for(int j=0; j<36; j++) {
				covid_mers[i][j] = COVIDGen[i+j];
			}
		}

		int fuzzyMatches = 0;
		int regMatches = 0;

		for (int i=0; i<(29903-35); i++) {
			bool fuzzyMatch = mainTrie.fuzzySearch(covid_mers[i], 36, 0);
			if (fuzzyMatch == true) {
				fuzzyMatches++;
			}
			bool regMatch = mainTrie.regSearch(covid_mers[i], 36);
			if (regMatch == true) {
				regMatches++;
			}
		}

		cout << "for " << numberOfRandoms << " random 36-mers, and all " << (29903-35) << " possible 36-mers:" << endl;
		cout << regMatches << " matches were found with the regular search, and " << fuzzyMatches << " were found with the fuzzy search" << endl << endl;

	}

	else if (strcmp(argv[1], "Problem1B") == 0) {
		int numberOfRandoms = atoi(argv[2]);

		char * COVIDGen = new char[29903];
		COVIDGen = COVID_Genome();

		Trie mainTrie;

		char randomSeq[36];
		for (int i=0; i<numberOfRandoms; i++) {
			int randomStart = rand() % (29903 - 35);
			for (int j=0; j<36; j++) {
				int randomChance = rand() % 20;
				if (randomChance == 0) {
					randomSeq[j] = randomLetter();
				}
				else {
					randomSeq[j] = COVIDGen[randomStart+j];
				}
			}
			mainTrie.insert(randomSeq, 36); //i insert the sequences as they are created
		}

		cout << "the trie contains " << mainTrie.countNodes() << " nodes" << endl;

		int covid_mers_length = 29903 - 35;
		char ** covid_mers = new char*[covid_mers_length];
		for (int i=0; i<covid_mers_length; i++) {
			covid_mers[i] = new char[36];
			for(int j=0; j<36; j++) {
				covid_mers[i][j] = COVIDGen[i+j];
			}
		}

		int fuzzyMatches = 0;
		int regMatches = 0;

		for (int i=0; i<(29903-35); i++) {
			bool fuzzyMatch = mainTrie.fuzzySearch(covid_mers[i], 36, 0);
			if (fuzzyMatch == true) {
				fuzzyMatches++;
			}
			bool regMatch = mainTrie.regSearch(covid_mers[i], 36);
			if (regMatch == true) {
				regMatches++;
			}
		}

		cout << "for " << numberOfRandoms << " random 36-mers, and all " << (29903-35) << " possible 36-mers:" << endl;
		cout << regMatches << " matches were found with the regular search, and " << fuzzyMatches << " were found with the fuzzy search" << endl << endl;

	}

	else {
		cout << "Wrong input" << endl;
	}


	return (0);
}

char randomLetter() {
	int randomValue = rand()%4;
	char letter;

	if(randomValue == 0) {
		letter = 'A';
	}

	else if(randomValue == 1) {
		letter = 'C';
	}

	else if(randomValue == 2) {
		letter = 'G';
	}

	else {
		letter = 'T';
	}

	return letter;
}
