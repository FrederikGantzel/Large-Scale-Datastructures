
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;

long int maxQuaternaryValue = pow(4.0, 16);


unsigned int genSeq_to_quater_convert (char * sequence) { //converts the sequence to a decimal value
	unsigned int quaterValue = 0;

	for (int i=0; i<16; i++) {

		if (sequence[i] == 'A') { //doesn't do anything, is just here for concistency
			quaterValue = quaterValue + 0;
		}
		if (sequence[i] == 'C') {
			quaterValue = quaterValue + pow(4.0, 15-i);
		}
		if (sequence[i] == 'G') {
			quaterValue = quaterValue + (2 * pow(4.0, 15-i));
		}
		if (sequence[i] == 'T') {
			quaterValue = quaterValue + (3 * pow(4.0, 15-i));
		}
	}

	return quaterValue;
}


char *  quater_to_genSeq_convert (unsigned int quaterValue) { //converts the decimal value to a sequence
	char * sequence = new char[16];
	int i = 16;

	while (quaterValue != 0) {
		int remainder = quaterValue % 4;
		if (remainder == 0) {
			sequence[i] = 'A';
			i--;
		}
		if (remainder == 1) {
			sequence[i] = 'C';
			i--;
		}
		if (remainder == 2) {
			sequence[i] = 'G';
			i--;
		}
		if (remainder == 3) {
			sequence[i] = 'T';
			i--;
		}
		quaterValue = quaterValue - remainder;
		quaterValue = quaterValue / 4;
	}
	while (i > 0) {
		sequence[i] = 'A';
		i--;
	}

	return sequence;
}


class Node {
	public:
		Node(){}
		char * sequence = new char[16];
		Node * next;
		~Node(){
			delete[] sequence;
			delete next;
		}
}
;


class linkedList { //Node and linkedList classes from Homework #2
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

		bool remove_node(char * remSequence) {
			Node * currentNode = new Node;
			currentNode = head;
			bool found = false;

			if(strcmp(currentNode->sequence, remSequence) == 0) {
				delete[] currentNode->sequence;
				head = currentNode->next;
				found = true;
			}

			while(currentNode->next != NULL) {
				if(strcmp(currentNode->next->sequence, remSequence) == 0) {
					delete[] currentNode->next->sequence;
					currentNode->next = currentNode->next->next;
					found = true;
					break;
				}
				else {
					currentNode = currentNode->next;
				}
			}
			return found;

		}

		~linkedList() {
			delete head;
		}

}
;


class FASTAreadset_DA {
	private:
		int elements;
		int collisions;

	public:
		bool * hashTable = new bool[maxQuaternaryValue];
		FASTAreadset_DA(){
			elements = 0;
			collisions = 0;
		}

		FASTAreadset_DA(char* location) {
			elements = 0;
			collisions = 0;
			ifstream input;
			input.open(location);
			char * skip = new char[16];
			char * nextSequence = new char[16];

			while(!input.eof()){
				input >> skip;
				input >> nextSequence;
				insert(nextSequence);
			}

			input.close();
		}

		int getElements() {
			return elements;
		}

		int getCollisions() {
			return collisions;
		}

		void insert (char * sequence) {
			unsigned int key = genSeq_to_quater_convert(sequence);

			if (hashTable[key] == false) {
				hashTable[key] = true;
				elements++;
			}
			else {
				elements++;
				collisions++;
			}
		}

		void remove (char * sequence) {
			unsigned int key = genSeq_to_quater_convert(sequence);

			if (hashTable[key] == true) {
				hashTable[key] = false;
				elements--;
			}
			else {
				cout << "Attempting to remove sequence: " << endl;
				cout << sequence << endl;
				cout << "But there is no such sequence in the table" << endl;
			}
		}

		bool search (char * sequence) {
			unsigned int key = genSeq_to_quater_convert(sequence);

			return hashTable[key];
		}

		~FASTAreadset_DA(){
			delete[] hashTable;
		}
}
;


class FASTAreadset_Chain {
	private:
		int size;
		int elements;
		int collisions;

	public:
		linkedList * hashTable;
		FASTAreadset_Chain(int S){
			elements = 0;
			collisions = 0;
			size = S;
			hashTable = new linkedList[size];
		}

		FASTAreadset_Chain(char* location, int S) {
			elements = 0;
			collisions = 0;
			size = S;
			hashTable = new linkedList[size];
			ifstream input;
			input.open(location);
			char * skip = new char[16];

			while(!input.eof()){
				char * nextSequence = new char[16];
				input >> skip;
				input >> nextSequence;
				insert(nextSequence);
			}

			input.close();
		}

		int getSize() {
			return size;
		}

		int getElements() {
			return elements;
		}

		int getCollisions() {
			return collisions;
		}

		int hashFunction(unsigned int key) {
			int result = key % size;
			return (result);
		}

		void insert (char * sequence) {
			unsigned int key = genSeq_to_quater_convert(sequence);
			int hashIndex = hashFunction(key);

			if(hashTable[hashIndex].head == NULL) {
				hashTable[hashIndex].add_node(sequence);
				elements++;
			}
			else {
				hashTable[hashIndex].add_node(sequence);
				elements++;
				collisions++;
			}
		}

		void remove (char * sequence) {
			unsigned int key = genSeq_to_quater_convert(sequence);
			int hashIndex = hashFunction(key);
			bool removed = hashTable[hashIndex].remove_node(sequence);
			if (removed == true) {
				elements--;
				if(hashTable[hashIndex].head != NULL && hashTable[hashIndex].head->next == NULL) {
					collisions--;
				}
			}
			else {
				cout << "Attempting to remove sequence: " << endl;
				cout << sequence << endl;
				cout << "But there is no such sequence in the table" << endl;
			}
		}

		bool search (char * sequence) {
			unsigned int key = genSeq_to_quater_convert(sequence);
			int hashIndex = hashFunction(key);
			bool found = false;
			Node * current = new Node;
			current = hashTable[hashIndex].head;

			while(current != NULL) {
				if(strcmp(current->sequence, sequence) == 0) {
					found = true;
					break;
				}
				current = current->next;
			}

			return found;
		}

		~FASTAreadset_Chain(){
			delete[] hashTable;
		}
}
;


int main(int argc, char ** argv){

	if (argc != 4){  // unexpected program call
		cout << endl << endl << "=========================="<< endl;
		cout << "Error: 3 input parameters expected" <<endl;
		cout << "Proper usage is:" <<endl;
		cout << "./homework <problem-flag> <filepath for hw_dataset.fa> <filepath for test_genome.fasta> " << endl;
		cout << "Example:" << endl;
		cout << "./homework Problem1A hw3_dataset.fa test_genome.fasta"  << endl;
		cout << "=========================="<< endl << endl;
		cout << "exiting..." << endl;
		exit(-1);
	}else{

		cout << "The number of arguments passed: " << argc << endl;
		cout << "The first argument is: " << argv[0] << endl;
		cout << "The second argument is: " << argv[1] << endl;
		cout << "The third argument is: " << argv[2] << endl;
		cout << "the fourth argument is: " << argv[3] << endl << endl;
	}


	if(strcmp(argv[1], "Problem1A") == 0) {

		FASTAreadset_DA data = FASTAreadset_DA(argv[2]);

		int elements = data.getElements();
		int collisions = data.getCollisions();
		int unique_seqs = elements - collisions;
		double loadFactor = (double)elements / maxQuaternaryValue;

		cout << "Number of elements: " << elements << endl;
		cout << "Number of collisions: " << collisions << endl;
		cout << "Number of unique elements: " << unique_seqs << endl;
		cout << "Load factor is: " << loadFactor << endl;

	}

	else if(strcmp(argv[1], "Problem1B") == 0) {
		FASTAreadset_DA data = FASTAreadset_DA(argv[2]);

		//load in the genome sequence using code from Homework #2
		linkedList baciGen;

		ifstream input;
		input.open(argv[3]);
		char skip;
		char * headSequence = new char[16];

		for (int i=0; i<71; i++) { //skip the header
			input.get(skip);
		}

		for (int i=0; i<16; i++) { //first sequence is first 16 chars
			char symb;
			input.get(symb);
			headSequence[i] = symb;
		}

		baciGen.add_node(headSequence);

		while(!input.eof()){
			char * nextSequence = new char[16];
			char symb;

			input.get(symb);

			if(symb == 'A' || symb == 'C' || symb == 'G' || symb == 'T') {

				for(int i=0; i<15; i++) {
					nextSequence[i] = baciGen.tail->sequence[i+1]; //next sequence is the last 15 characters of previous sequence

				}
				nextSequence[15] = symb; //plus the next character

				baciGen.add_node(nextSequence);

			}

		}

		input.close();

		Node * currentNode = new Node;
		currentNode = baciGen.head;
		int matches = 0;

		time_t start = time(nullptr);
		while(currentNode != NULL) { //search through genome
			bool searchFrag = data.search(currentNode->sequence);
			if (searchFrag == true) {
				matches++;
			}
			currentNode = currentNode->next;
		}
		time_t end = time(nullptr);
		time_t duration = end-start;

		cout << "Number of matching fragments found in read set: " << matches << endl;
		cout << "The entire search process took: " << duration << " seconds" << endl;
	}

	else if(strcmp(argv[1], "Problem2A") == 0) {

		time_t start = time(nullptr);
		FASTAreadset_Chain data1 = FASTAreadset_Chain(argv[2], 10000);
		time_t end = time(nullptr);
		time_t duration = end-start;

		int hashSize = data1.getSize();
		int elements = data1.getElements();
		int collisions = data1.getCollisions();
		double loadFactor = (double)elements / hashSize;

		cout << "Size of hash table: " << hashSize << endl;
		cout << "Number of elements: " << elements << endl;
		cout << "Number of collisions: " << collisions << endl;
		cout << "Load factor is: " << loadFactor << endl;
		cout << "Time to read in the sequence fragment file: " << duration << endl << endl;

		start = time(nullptr);
		FASTAreadset_Chain data2 = FASTAreadset_Chain(argv[2], 100000);
		end = time(nullptr);
		duration = end-start;

		hashSize = data2.getSize();
		elements = data2.getElements();
		collisions = data2.getCollisions();
		loadFactor = (double)elements / hashSize;

		cout << "Size of hash table: " << hashSize << endl;
		cout << "Number of elements: " << elements << endl;
		cout << "Number of collisions: " << collisions << endl;
		cout << "Load factor is: " << loadFactor << endl;
		cout << "Time to read in the sequence fragment file: " << duration << endl << endl;

		start = time(nullptr);
		FASTAreadset_Chain data3 = FASTAreadset_Chain(argv[2], 1000000);
		end = time(nullptr);
		duration = end-start;

		hashSize = data3.getSize();
		elements = data3.getElements();
		collisions = data3.getCollisions();
		loadFactor = (double)elements / hashSize;

		cout << "Size of hash table: " << hashSize << endl;
		cout << "Number of elements: " << elements << endl;
		cout << "Number of collisions: " << collisions << endl;
		cout << "Load factor is: " << loadFactor << endl;
		cout << "Time to read in the sequence fragment file: " << duration << endl << endl;

		start = time(nullptr);
		FASTAreadset_Chain data4 = FASTAreadset_Chain(argv[2], 10000000);
		end = time(nullptr);
		duration = end-start;

		hashSize = data4.getSize();
		elements = data4.getElements();
		collisions = data4.getCollisions();
		loadFactor = (double)elements / hashSize;

		cout << "Size of hash table: " << hashSize << endl;
		cout << "Number of elements: " << elements << endl;
		cout << "Number of collisions: " << collisions << endl;
		cout << "Load factor is: " << loadFactor << endl;
		cout << "Time to read in the sequence fragment file: " << duration << endl << endl;

	}

	else if(strcmp(argv[1], "Problem2B") == 0) {
		FASTAreadset_Chain data = FASTAreadset_Chain(argv[2], 10000000);

		//load in the genome sequence using code from Homework #2
		linkedList baciGen;

		ifstream input;
		input.open(argv[3]);
		char skip;
		char * headSequence = new char[16];

		for (int i=0; i<71; i++) { //skip the header
			input.get(skip);
		}

		for (int i=0; i<16; i++) { //first sequence is first 16 chars
			char symb;
			input.get(symb);
			headSequence[i] = symb;
		}

		baciGen.add_node(headSequence);

		while(!input.eof()){
			char * nextSequence = new char[16];
			char symb;

			input.get(symb);

			if(symb == 'A' || symb == 'C' || symb == 'G' || symb == 'T') {

				for(int i=0; i<15; i++) {
					nextSequence[i] = baciGen.tail->sequence[i+1]; //next sequence is the last 15 characters of previous sequence

				}
				nextSequence[15] = symb; //plus the next character

				baciGen.add_node(nextSequence);

			}

		}

		input.close();

		Node * currentNode = new Node;
		currentNode = baciGen.head;
		int matches = 0;

		time_t start = time(nullptr);
		while(currentNode != NULL) { //search through genome
			bool searchFrag = data.search(currentNode->sequence);
			if (searchFrag == true) {
				matches++;
			}
			currentNode = currentNode->next;
		}
		time_t end = time(nullptr);
		time_t duration = end-start;

		cout << "Number of matching fragments found in read set: " << matches << endl;
		cout << "The entire search process took: " << duration << " seconds" << endl;
	}

	else {
		cout << "Wrong input" << endl;
	}


	return (0);
}
