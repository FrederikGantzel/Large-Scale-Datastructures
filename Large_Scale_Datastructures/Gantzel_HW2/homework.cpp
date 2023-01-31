
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;

class Node {
	public:
		Node(){}
		char * sequence = new char[50];
		Node * next;
		~Node(){
			delete[] sequence;
			delete next;
		}
}
;

class linkedList {
	public:
		Node * head;
		Node * tail;

		linkedList() {
			head = NULL;
			tail = NULL;
		}

		void add_node(char * nextSequence) {
			Node * newNode = new Node;
			newNode->sequence = nextSequence;
			newNode->next = NULL;

			if(head == NULL) {
				head = newNode;
				tail = newNode;
			}
			else {
				tail->next = newNode;
				tail = tail->next;
			}
		}

		~linkedList() {
			delete head;
		}

}
;

class FASTAreadset_LL {
	public:
		linkedList genSequence;

		FASTAreadset_LL(){}
		FASTAreadset_LL(char* location, int readLimit) {
			ifstream input;
			input.open(location);
			char * skip = new char[50];

			for(int i = 0; i < readLimit; i++){
				char * nextSequence = new char[50];
				input >> skip;
				input >> nextSequence;
				genSequence.add_node(nextSequence);
			}

			input.close();
		}
		FASTAreadset_LL(const FASTAreadset_LL &templ) {
			Node * templNode = templ.genSequence.head;

			while (templNode != NULL) {
				genSequence.add_node(templNode->sequence);
				templNode = templNode->next;
			}

		}

		void readFASTA(char* location) {
			ifstream input;
			input.open(location);
			char * skip = new char[50];

			while(!input.eof()){
				char * nextSequence = new char[50];
				input >> skip;
				input >> nextSequence;
				genSequence.add_node(nextSequence);
			}

			input.close();
		}

		Node * seqSearch(char * targetSequence) {
				Node * output = NULL;
				Node * current = new Node;
				current = genSequence.head;


				while(current != NULL) {
					if(strcmp(current->sequence, targetSequence) == 0) {
						output = current;
						return output;
					}
					current = current->next;
				}

				return output;
		}

		~FASTAreadset_LL(){
		} //destructor
}
;


int main(int argc, char ** argv){

	if (argc != 4){  // unexpected program call
		cout << endl << endl << "=========================="<< endl;
		cout << "Error: 3 input parameters expected" <<endl;
		cout << "Proper usage is:" <<endl;
		cout << "./homework <problem-flag> <filepath for hw_dataset.fa> <filepath for test_genome.fasta> " << endl;
		cout << "Example:" << endl;
		cout << "./homework problem1A hw_dataset.fa test_genome.fasta"  << endl;
		cout << "=========================="<< endl << endl;
		cout << "exiting..." << endl;
		exit(-1);
	}else{

		cout << "The number of arguments passed: " << argc << endl;
		cout << "The first argument is: " << argv[0] << endl;
		cout << "The second argument is: " << argv[1] << endl;
		cout << "The third argument is: " << argv[2] << endl;
		cout << "the fourth argument is: " << argv[3] << endl;
	}


	if(strcmp(argv[1], "Problem1A") == 0) {
		FASTAreadset_LL data;

		time_t start = time(nullptr);
		data.readFASTA(argv[2]);
		time_t end = time(nullptr);
		time_t duration = end-start;

		cout << "Reading the entire 36 million read set took: " << duration << " seconds" << endl;
	}


	else if(strcmp(argv[1], "Problem1B") == 0) {
		time_t start;
		for(int i=0; i<1; i++) {
			FASTAreadset_LL data; //i couldn't figure out how to delete the class manually
			data.readFASTA(argv[2]); //so i just let it go out of bounds and time how long it takes for it to delete itself
			start = time(nullptr);
		}
		time_t end = time(nullptr);
		time_t duration = end-start;

		cout << "Deleting the datastructure took: " << duration << " seconds" << endl;
	}


	else if(strcmp(argv[1], "Problem1C") == 0) {
		FASTAreadset_LL data;
		data.readFASTA(argv[2]);

		time_t start = time(nullptr);
		FASTAreadset_LL data2 = data;
		time_t end = time(nullptr);
		time_t duration = end-start;

		cout << "Copying the entire object took: " << duration << " seconds" << endl;

	}


	else if(strcmp(argv[1], "Problem1D") == 0) {
		FASTAreadset_LL data;
		data.readFASTA(argv[2]);

		time_t start = time(nullptr);

		char * target = new char[50];
		string tarstring = "CTAGGTACATCCACACACAGCAGCGCATTATGTATTTATTGGATTTATTT";
		strcpy(target, tarstring.c_str());
		Node * targetAddress = data.seqSearch(target);
		cout << "The adress for the sequence \"CTAGGTACATCCACACACAGCAGCGCATTATGTATTTATTGGATTTATTT\" is: " << targetAddress << endl;

		tarstring = "GCGCGATCAGCTTCGCGCGCACCGCGAGCGCCGATTGCACGAAATGGCGC";
		strcpy(target, tarstring.c_str());
		targetAddress = data.seqSearch(target);
		cout << "The adress for the sequence \"GCGCGATCAGCTTCGCGCGCACCGCGAGCGCCGATTGCACGAAATGGCGC\" is: " << targetAddress << endl;

		tarstring = "CGATGATCAGGGGCGTTGCGTAATAGAAACTGCGAAGCCGCTCTATCGCC";
		strcpy(target, tarstring.c_str());
		targetAddress = data.seqSearch(target);
		cout << "The adress for the sequence \"CGATGATCAGGGGCGTTGCGTAATAGAAACTGCGAAGCCGCTCTATCGCC\" is: " << targetAddress << endl;

		tarstring = "CGTTGGGAGTGCTTGGTTTAGCGCAAATGAGTTTTCGAGGCTATCAAAAA";
		strcpy(target, tarstring.c_str());
		targetAddress = data.seqSearch(target);
		cout << "The adress for the sequence \"CGTTGGGAGTGCTTGGTTTAGCGCAAATGAGTTTTCGAGGCTATCAAAAA\" is: " << targetAddress << endl;

		tarstring = "ACTGTAGAAGAAAAAAGTGAGGCTGCTCTTTTACAAGAAAAAGTNNNNNN";
		strcpy(target, tarstring.c_str());
		targetAddress = data.seqSearch(target);
		cout << "The adress for the sequence \"ACTGTAGAAGAAAAAAGTGAGGCTGCTCTTTTACAAGAAAAAGTNNNNNN\" is: " << targetAddress << endl;

		time_t end = time(nullptr);
		time_t duration = end-start;

		cout << "Completing the search for all five sequences took: " << duration << " seconds" << endl;

	}


	else if(strcmp(argv[1], "Problem2A") == 0) {
		linkedList baciGen;
		int fragCount = 0;

		ifstream input;
		input.open(argv[3]);
		char skip;
		char * headSequence = new char[50];

		time_t start = time(nullptr);
		for (int i=0; i<71; i++) { //skip the header
			input.get(skip);
		}

		for (int i=0; i<50; i++) { //first sequence is first 50 chars
			char symb;
			input.get(symb);
			headSequence[i] = symb;
		}

		baciGen.add_node(headSequence);

		while(!input.eof()){
			char * nextSequence = new char[50];
			char symb;

			input.get(symb);

			if(symb == 'A' || symb == 'C' || symb == 'G' || symb == 'T') {

				for(int i=0; i<49; i++) {
					nextSequence[i] = baciGen.tail->sequence[i+1]; //next sequence is the last 49 characters of previous sequence

				}
				nextSequence[49] = symb; //plus the next character

				baciGen.add_node(nextSequence);
				fragCount++;

			}

		}
		time_t end = time(nullptr);
		time_t duration = end-start;

		input.close();

		cout << "Number of 50-character fragments in the genome sequence: " << fragCount << endl;
		cout << "Breaking up the genome sequence took: " << duration << " seconds" << endl;

		/*
		Node * currentNode = new Node; //used for printing all fragments
		currentNode = baciGen.head;

		while(currentNode != NULL) {
			cout << currentNode->sequence << endl;
			currentNode = currentNode->next;
		}
		*/

	}


	else if(strcmp(argv[1], "Problem2B") == 0) {
		linkedList baciGen;

		ifstream input;
		input.open(argv[3]);
		char skip;
		char * headSequence = new char[50];

		for (int i=0; i<71; i++) { //skip the header
			input.get(skip);
		}

		for (int i=0; i<50; i++) { //first sequence is first 50 chars
			char symb;
			input.get(symb);
			headSequence[i] = symb;
		}

		baciGen.add_node(headSequence);

		while(!input.eof()){
			char * nextSequence = new char[50];
			char symb;

			input.get(symb);

			if(symb == 'A' || symb == 'C' || symb == 'G' || symb == 'T') {

				for(int i=0; i<49; i++) {
					nextSequence[i] = baciGen.tail->sequence[i+1]; //next sequence is the last 49 characters of last sequence

				}
				nextSequence[49] = symb; //plus the next character

				baciGen.add_node(nextSequence);

			}

		}

		input.close();


		FASTAreadset_LL data;
		data.readFASTA(argv[2]);

		Node * currentNode = new Node;
		currentNode = baciGen.head;
		int matches = 0;
		//int count = 0;

		time_t start = time(nullptr);
		//while(count < 100) {
		//while(count < 1000) {
		//while(count < 10000) {
		while(currentNode != NULL) {
			Node * targetAddress = data.seqSearch(currentNode->sequence);
			if (targetAddress != NULL) {
				matches++;
			}
			currentNode = currentNode->next;
			//count++;
		}
		time_t end = time(nullptr);
		time_t duration = end-start;

		//cout << "searching the first " << count << " sequence fragments:" << endl;
		cout << "Number of matching fragments found in read set: " << matches << endl;
		cout << "The entire search process took: " << duration << " seconds" << endl;

	}


	else {
		cout << "Wrong input" << endl;
	}


	return (0);
}
