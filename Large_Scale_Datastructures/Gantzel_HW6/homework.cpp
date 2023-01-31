#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cmath>
#include <array>
#include <stack>
#include "appendixes.h"
using namespace std;

//cd /mnt/c/Users/Gantzel/Desktop/Datastructures/Gantzel_HW6

class suffix_tree {
public:
	suffix_tree * children[5];
	int howLong;
	int offset;

	suffix_tree() {
		howLong = 1;
		offset = 0;
		for (int i=0; i<5; i++) {
			children[i] = nullptr;
		}
	}

	suffix_tree(char * seq, int seqLength) {
		howLong = 1;
		offset = 0;
		for (int i=0; i<5; i++) {
			children[i] = nullptr;
		}
		insert(seq, seqLength);
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
		else if (input == '$') {
			return 4;
		}
		else {
			cout << "invalid character" << endl;
			exit(-1);
		}
	}

	void insert(char * seq, int length) {

		for (int i=0; i<length; i++) {
			int insertLength = length-i;
			suffix_tree * current = this;

			if (current->children[key(seq[i])] == nullptr) {
				current->children[key(seq[i])] = new suffix_tree();
				current->children[key(seq[i])]->howLong = insertLength;
				current->children[key(seq[i])]->offset = i;
			}
			else {
				current = current->children[key(seq[i])];

				int howDeep = 1;
				int depthOfCurrent = 1;
				bool nextIsMatch;
				if (current->howLong == 1) {
					if (current->children[key(seq[i+howDeep])] != nullptr) {
						nextIsMatch = true;
					}
					else {
						nextIsMatch = false;
					}
				}
				else {
					if(key(seq[i+howDeep]) == key(seq[(current->offset)+depthOfCurrent])) {
						nextIsMatch = true;
					}
					else {
						nextIsMatch = false;
					}
				}

				while(nextIsMatch == true) {
					if (depthOfCurrent == current->howLong) {
						current = current->children[key(seq[i+howDeep])];

						depthOfCurrent = 1;
						howDeep++;
					}
					else {
						depthOfCurrent++;
						howDeep++;
					}

					if (current->howLong == depthOfCurrent) {
						if (current->children[key(seq[i+howDeep])] == nullptr) {
							nextIsMatch = false;
						}
					}
					else {
						if(key(seq[i+howDeep]) != key(seq[(current->offset)+depthOfCurrent])) {
							nextIsMatch = false;
						}
					}

				}

				int origLength = current->howLong;
				current->howLong = depthOfCurrent;

				if (origLength - current->howLong != 0) {
					suffix_tree * tempTree = new suffix_tree();
					for (int j=0; j<5; j++) {
						tempTree->children[j] = current->children[j];
						current->children[j] = nullptr;
					}
					tempTree->howLong = origLength - current->howLong;
					tempTree->offset = current->offset + current->howLong;

					current->children[key(seq[(current->offset)+depthOfCurrent])] = tempTree;
				}

				current->children[key(seq[i+howDeep])] = new suffix_tree();
				current->children[key(seq[i+howDeep])]->howLong = insertLength-howDeep;
				current->children[key(seq[i+howDeep])]->offset = i+howDeep;

			}

		}
	}

	bool search(char * seq, int length, char * origSeq) {
		suffix_tree * current = this;

		int locInString = 1;
		for (int i=0; i<length; i++){

			if (current == nullptr) {
				return false;
			}
			else if (current->howLong == 1) {
				current = current->children[key(seq[i])];
			}
			else if ((current->offset)+locInString != (current->offset)+(current->howLong)) {
				if (key(seq[i]) != key(origSeq[(current->offset)+locInString])) {
					return false;
				}
				locInString++;
			}
			else {
				current = current->children[key(seq[i])];
				locInString = 1;
			}

		}

		return true;
	}

	int countNodes() {
		suffix_tree * current = this;
		int nr = 0;

		if (current == nullptr) {
			return nr;
		}

		for (int i=0; i<5; i++) {
			nr = nr + current->children[i]->countNodes();
		}

		return nr+1;
	}

	~suffix_tree(){
		for (int i=0; i<5; i++) {
			delete children[i];
		}
	}
}
;

class suffix_trie {
public:
	suffix_trie * children[5];

	suffix_trie() {
		for (int i=0; i<5; i++) {
			children[i] = nullptr;
		}
	}

	suffix_trie(char * seq, int seqLength) {
		for (int i=0; i<5; i++) {
			children[i] = nullptr;
		}
		insert(seq, seqLength);
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
		else if (input == '$') {
			return 4;
		}
		else {
			cout << "invalid character" << endl;
			exit(-1);
		}
	}

	void insert(char * seq, int length) {

		for (int i=0; i<length; i++) {
			suffix_trie * current = this;

			for (int j=0; j<(length-i); j++) {

				if (current->children[key(seq[j+i])] == nullptr) {
					current->children[key(seq[j+i])] = new suffix_trie();
				}

				current = current->children[key(seq[j+i])];
			}

		}
	}

	bool search(char * seq, int length) {
		suffix_trie * current = this;

		if (current == nullptr) {
			return false;
		}

		for (int i=0; i<length; i++){
			current = current->children[key(seq[i])];

			if (current == nullptr) {
				return false;
			}
		}

		return true;
	}

	int countNodes() {
		suffix_trie * current = this;
		int nr = 0;

		if (current == nullptr) {
			return nr;
		}

		for (int i=0; i<5; i++) {
			nr = nr + current->children[i]->countNodes();
		}

		return nr+1;
	}

	~suffix_trie(){
		for (int i=0; i<5; i++) {
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

		cout << endl;
		cout << "The number of arguments passed: " << argc << endl;
		cout << "The first argument is: " << argv[0] << endl;
		cout << "The second argument is: " << argv[1] << endl;
		cout << "The third argument is: " << argv[2] << endl << endl;
	}


	if(strcmp(argv[1], "Problem1A") == 0) {

		char * COVIDGen = appendix_A();
		char ** apbFrag = appendix_B();

		suffix_trie mainSuffixTrie(COVIDGen, 29904);

		cout << "the trie contains " << mainSuffixTrie.countNodes() << " nodes" << endl;

		for (int i=0; i<5; i++) {
			bool found = mainSuffixTrie.search(apbFrag[i], 36);
			if (found == true) {
				cout << "Found fragment nr " << i+1 << " from appendix B" << endl;
			}
			else {
				cout << "Did not find fragment nr " << i+1 << " from appendix B" << endl;
			}
		}

		cout << endl;
	}

	else if(strcmp(argv[1], "Problem1B") == 0) {

		int numberOfRandoms = atoi(argv[2]);

		char * COVIDGen = appendix_A();

		time_t buildStart = time(nullptr);
		suffix_trie mainSuffixTrie(COVIDGen, 29904);
		time_t buildEnd = time(nullptr);

		cout << "the trie contains " << mainSuffixTrie.countNodes() << " nodes" << endl;

		int matches = 0;
		char covid_kmers[36];

		time_t searchStart = time(nullptr);
		for (int i=0; i<numberOfRandoms; i++) {
			int randomStart = rand() % (29903 - 35);
			for (int j=0; j<36; j++) { //creating the random sequence
				covid_kmers[j] = COVIDGen[randomStart+j];
			}

			bool found = mainSuffixTrie.search(covid_kmers, 36); //and searching for it
			if (found == true) {
				matches++;
			}
		}
		time_t searchEnd = time(nullptr);

		cout << "It took " << buildEnd - buildStart << " seconds to build the suffix trie" << endl;
		cout << "It took " << searchEnd - searchStart << " seconds to search for all " << numberOfRandoms << " random sequences" << endl;
		cout << "Out of " << numberOfRandoms << " random sequences from the genome, " << matches << " were found" << endl;
		cout << endl;

	}

	else if(strcmp(argv[1], "Problem2A") == 0) {

		char * COVIDGen = appendix_A();
		char ** apbFrag = appendix_B();

		suffix_tree mainSuffixTree(COVIDGen, 29904);
		cout << "the trie contains " << mainSuffixTree.countNodes() << " nodes" << endl;

		for (int i=0; i<5; i++) {
			bool found = mainSuffixTree.search(apbFrag[i], 36, COVIDGen);
			if (found == true) {
				cout << "Found fragment nr " << i+1 << " from appendix B" << endl;
			}
			else {
				cout << "Did not find fragment nr " << i+1 << " from appendix B" << endl;
			}
		}

		cout << endl;
	}

	else if(strcmp(argv[1], "Problem2B") == 0) {

		int numberOfRandoms = atoi(argv[2]);

		char * COVIDGen = appendix_A();

		time_t buildStart = time(nullptr);
		suffix_tree mainSuffixTree(COVIDGen, 29904);
		time_t buildEnd = time(nullptr);

		cout << "the trie contains " << mainSuffixTree.countNodes() << " nodes" << endl;

		int matches = 0;
		char covid_kmers[36];

		time_t searchStart = time(nullptr);
		for (int i=0; i<numberOfRandoms; i++) {
			int randomStart = rand() % (29903 - 35);
			for (int j=0; j<36; j++) { //creating the random sequence
				covid_kmers[j] = COVIDGen[randomStart+j];
			}

			bool found = mainSuffixTree.search(covid_kmers, 36, COVIDGen); //and searching for it
			if (found == true) {
				matches++;
			}
		}
		time_t searchEnd = time(nullptr);

		cout << "It took " << buildEnd - buildStart << " seconds to build the suffix tree" << endl;
		cout << "It took " << searchEnd - searchStart << " seconds to search for all " << numberOfRandoms << " random sequences" << endl;
		cout << "Out of " << numberOfRandoms << " random sequences from the genome, " << matches << " were found" << endl;
		cout << endl;

	}

	else {
		cout << "Wrong input" << endl;
	}


	return (0);
}
