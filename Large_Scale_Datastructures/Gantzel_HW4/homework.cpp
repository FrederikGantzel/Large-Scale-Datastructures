#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cmath>
#include <array>
using namespace std;

//cd /mnt/c/Users/Gantzel/Desktop/Datastructures/Gantzel_HW4

long int maxQuaternaryValue = pow(4.0, 16);

char * Appendix_A();
char ** Appendix_B();
char randomLetter();
int genSeq_to_quater_convert (char * sequence);
char * quater_to_genSeq_convert (int quaterValue);

class Alignment {
	public:
		int length;
		int bestScore;
		char * seq1;
		char * connect;
		char * seq2;
		Alignment(){}
		Alignment(int Size, int maxi){
			length = Size;
			seq1 = new char[length+1];
			connect = new char[length+1];
			seq2 = new char[length+1];
			bestScore = maxi;
		}

		void setArrays(char * seqnr1, char * align, char * seqnr2) {
			for (int i=0; i<=length; i++) {
				seq1[i] = seqnr1[length-i];
				connect[i] = align[length-i];
				seq2[i] = seqnr2[length-i];
			}
		}

		~Alignment(){
			delete[] seq1;
			delete[] connect;
			delete[] seq2;
		}
}
;

class directAccess_hashTable {
	private:
		int elements;
		int collisions;

	public:
		bool * hashTable = new bool[maxQuaternaryValue];
		directAccess_hashTable(){
			elements = 0;
			collisions = 0;
		}

		directAccess_hashTable(char* location) {
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

		~directAccess_hashTable(){
			delete[] hashTable;
		}
}
;

//Quaternary converters from Assignment 3
int genSeq_to_quater_convert (char * sequence) { //converts the sequence to a decimal value
	int quaterValue = 0;

	for (int i=0; i<11; i++) {

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


char *  quater_to_genSeq_convert (int quaterValue) { //converts the decimal value to a sequence
	char * sequence = new char[11];
	int i = 11;

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

char * cov_snippet_finder(char * COVID_Sequence, int COVID_Sequence_length, char ** covid_seeds, int covid_seeds_length, char * read, int read_length) {

	int read_kmers_length = read_length-10;

	char ** read_kmers = new char*[read_kmers_length];
	for (int i=0; i<read_kmers_length; i++) {
		read_kmers[i] = new char[11];
		for(int j=0; j<11; j++) {
			read_kmers[i][j] = read[i+j];
		}
	}

	directAccess_hashTable kmerTable;

	for (int i=0; i<read_kmers_length; i++) {
		kmerTable.insert(read_kmers[i]);
	}

	int cut_location = 0;
	bool found = false;

	//use the first matching segment found. This might not be the best one.
	for(int i=0; i<covid_seeds_length; i++) {
		found = kmerTable.search(covid_seeds[i]);
		if (found == true) {
			cut_location = i;
			break;
		}
	}

	if (found == false) {
		char * res = new char[1];
		res[0] = '\0';
		return res;
	}

	int snippet_length = 100;
	char * cov_snippet = new char[snippet_length];

	if (cut_location < 50) {
		for (int i=0; i<snippet_length; i++) {
			cov_snippet[i] = COVID_Sequence[i];
		}
	}
	else if ((cut_location + 50) > COVID_Sequence_length) {
		for (int i=0; i<snippet_length; i++) {
			cov_snippet[i] = COVID_Sequence[(COVID_Sequence_length - snippet_length) + i];
		}
	}
	else {
		for (int i=0; i<snippet_length; i++) {
			cov_snippet[i] = COVID_Sequence[(cut_location - (snippet_length/2)) + i];
		}
	}
	return cov_snippet;

}

Alignment SW_algorithm(char * sequence1, char * sequence2, int sequence1Length, int sequence2Length, int * simScoreMatrix, int penalty) {

	int C_ij[4];
	C_ij[3] = 0;

	//create the matrix
	int matrix[sequence1Length+1][sequence2Length+1];
	for (int i=0; i<=sequence1Length; i++) {
		matrix[i][0] = 0;
	}
	for (int i=0; i<=sequence2Length; i++) {
		matrix[0][i] = 0;
	}

	for (int i=1; i<=sequence1Length; i++) {
		for (int j=1; j<=sequence2Length; j++) {
			int match = 0;
			if (sequence1[i-1] == sequence2[j-1]) {
				match = 1;
			}
			C_ij[0] = matrix[i-1][j-1]+simScoreMatrix[match];
			C_ij[1] = matrix[i-1][j]+penalty;
			C_ij[2] = matrix[i][j-1]+penalty;

			int maxVal = C_ij[0];
			maxVal = max(maxVal, C_ij[1]);
			maxVal = max(maxVal, C_ij[2]);
			maxVal = max(maxVal, C_ij[3]);

			matrix[i][j] = maxVal;

		}
	}

	//find maximum score in matrix
	//there could be multiple maximum scores, but this program only finds one
	int matrix_maxVal = 0;
	int max_i = 0;
	int max_j = 0;

	for (int i=1; i<=sequence1Length; i++) {
		for (int j=1; j<=sequence2Length; j++) {
			if (matrix[i][j] > matrix_maxVal) {
				matrix_maxVal = matrix[i][j];
				max_i = i;
				max_j = j;
			}
		}
	}

	char * seqnr1 = new char[sequence1Length+sequence2Length+2]; //largest possible path through the matrix
	char * align = new char[sequence1Length+sequence2Length+2];
	char * seqnr2 = new char[sequence1Length+sequence2Length+2];
	int current_i = max_i;
	int current_j = max_j;
	int counter = 0;
	int up, left, diag = 0;

	while (current_i > 0 && current_j > 0) {

		int match = 0;
		if (sequence1[current_i-1] == sequence2[current_j-1]) {
			match = 1;
		}
		up = matrix[current_i][current_j-1]+penalty;
		left = matrix[current_i-1][current_j]+penalty;
		diag = matrix[current_i-1][current_j-1]+simScoreMatrix[match];

		if (diag == 0 && up == 0 && left == 0) {
			seqnr1[counter] = sequence1[current_i-1];
			seqnr2[counter] = sequence2[current_j-1];
			align[counter] = '|';
			break;
		}

		else if (diag >= up && diag >= left) { //go diagonal. If both diag and up, or diag and left, or all of them are equal, go diagonal
			seqnr1[counter] = sequence1[current_i-1];
			seqnr2[counter] = sequence2[current_j-1];
			if (match == 1) {
				align[counter] = '|';
			}
			else {
				align[counter] = 'x';
			}
			current_i--;
			current_j--;
		}

		else if (up >= left && up > diag) { //go up. If up and left are equal, go up
			seqnr1[counter] = '_';
			align[counter] = ' ';
			seqnr2[counter] = sequence2[current_j-1];
			current_j--;
		}

		else if (left > up && left > diag) { //go left
			seqnr1[counter] = sequence1[current_i-1];
			align[counter] = ' ';
			seqnr2[counter] = '_';
			current_i--;
		}

		else {
			cout << "something went wrong, my dude" << endl;
			cout << "i: " << current_i << " j: " << current_j << endl;
			cout << "up: " << up << " left: " << left << " diagonal: " << diag << " matrix value: " << matrix[current_i][current_j] << endl;
			exit(-1);
		}

		if (current_i == 0 || current_j == 0) {
			break;
		}
		counter++;

	}


	Alignment alignment = Alignment(counter, matrix_maxVal);
	alignment.setArrays(seqnr1, align, seqnr2);

	return alignment;

}


int main(int argc, char ** argv){

	if (argc != 3){  // unexpected program call
		cout << endl << endl << "=========================="<< endl;
		cout << "Error: 4 input parameters expected" <<endl;
		cout << "Proper usage is:" <<endl;
		cout << "./homework <problem-flag> <number of random sequences>" << endl;
		cout << "Example:" << endl;
		cout << "./homework Problem1A 1000"  << endl;
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
		char * COVIDGen = new char[29903];
		COVIDGen = Appendix_A();

		char ** reads = new char*[15];
		for (int i=0; i<15; i++) {
			reads[i] = new char[50];
		}
		reads = Appendix_B();
		int seq1size = 29903;
		int seq2size = 50;
		int simScore[2] = {-3, 3}; //{mismatch, match}

		int penalty = -2; //gap penalty

		for (int i=0; i<15; i++) {
			cout << "Alignment bewteen read " << i+1 << " and COVID genome sequence:" << endl;
			Alignment alignment = SW_algorithm(COVIDGen, reads[i], seq1size, seq2size, simScore, penalty);

			for (int j=0; j<=alignment.length; j++) {
				cout << alignment.seq1[j] << " ";
			}
			cout << endl;
			for (int j=0; j<=alignment.length; j++) {
				cout << alignment.connect[j] << " ";
			}
			cout << endl;
			for (int j=0; j<=alignment.length; j++) {
				cout << alignment.seq2[j] << " ";
			}
			cout << endl;
			cout << "alignment score: " << alignment.bestScore << endl << endl;

		}

	}

	else if(strcmp(argv[1], "Problem1B") == 0) {

		int randomSeqAmount = atoi(argv[2]);

		char * COVIDGen = new char[29903];
		COVIDGen = Appendix_A();

		int seq1size = 29903;
		int seq2size = 50;
		int simScore[2] = {-3, 3}; //{mismatch, match}

		int penalty = -2; //gap penalty

		char randomSeqs[randomSeqAmount][50];

		time_t createRandomStart = time(nullptr);
		for (int i=0; i<randomSeqAmount; i++) { //create random sequences
			for (int j=0; j<50; j++) {
				randomSeqs[i][j] = randomLetter();
			}
		}
		time_t createRandomEnd = time(nullptr);

		time_t alignmentStart = time(nullptr);
		for (int i=0; i<randomSeqAmount; i++) {
			Alignment alignment = SW_algorithm(COVIDGen, randomSeqs[i], seq1size, seq2size, simScore, penalty);
		}
		time_t alignmentEnd = time(nullptr);

		cout << "Total time to create " << randomSeqAmount << " random sequences: " << createRandomEnd-createRandomStart << " (s)" << endl;
		cout << "Total time to perform alignment on all " << randomSeqAmount << " random sequences: " << alignmentEnd-alignmentStart << " (s)" << endl;

	}

	else if(strcmp(argv[1], "Problem2A") == 0) {
		char * COVIDGen = new char[29903];
		COVIDGen = Appendix_A();

		int covid_seeds_length = 29903 - 10;
		char ** covid_seeds = new char*[covid_seeds_length];
		for (int i=0; i<covid_seeds_length; i++) {
			covid_seeds[i] = new char[11];
			for(int j=0; j<11; j++) {
				covid_seeds[i][j] = COVIDGen[i+j];
			}
		}

		char ** reads = new char*[15];
		for (int i=0; i<15; i++) {
			reads[i] = new char[50];
		}

		reads = Appendix_B();
		int seq1size = 29903;
		int seq2size = 50;
		int simScore[2] = {-3, 3}; //{mismatch, match}

		int penalty = -2; //gap penalty

		char * cov_snippet = new char[100];
		for (int i=0; i<15; i++) {
			cout << "Alignment bewteen read " << i+1 << " and COVID genome sequence:" << endl;
			cov_snippet = cov_snippet_finder(COVIDGen, seq1size, covid_seeds, covid_seeds_length, reads[i], seq2size);

			if (cov_snippet[0] != '\0') {
				Alignment alignment = SW_algorithm(cov_snippet, reads[i], 100, seq2size, simScore, penalty);

				for (int j=0; j<=alignment.length; j++) {
					cout << alignment.seq1[j] << " ";
				}
				cout << endl;
				for (int j=0; j<=alignment.length; j++) {
					cout << alignment.connect[j] << " ";
				}
				cout << endl;
				for (int j=0; j<=alignment.length; j++) {
					cout << alignment.seq2[j] << " ";
				}
				cout << endl;
				cout << "alignment score: " << alignment.bestScore << endl << endl;
			}

			else {
				cout << "could not find an 11 character long match between read " << i+1 << " and the genome sequence" << endl << endl;
			}

		}
	}

	else if(strcmp(argv[1], "Problem2B") == 0) {

		int randomSeqAmount = atoi(argv[2]);

		char * COVIDGen = new char[29903];
		COVIDGen = Appendix_A();

		int covid_seeds_length = 29903 - 10;
		char ** covid_seeds = new char*[covid_seeds_length];
		for (int i=0; i<covid_seeds_length; i++) {
			covid_seeds[i] = new char[11];
			for(int j=0; j<11; j++) {
				covid_seeds[i][j] = COVIDGen[i+j];
			}
		}

		int seq1size = 29903;
		int seq2size = 50;
		int simScore[2] = {-3, 3}; //{mismatch, match}

		int penalty = -2; //gap penalty

		char randomSeqs[randomSeqAmount][50];

		time_t createRandomStart = time(nullptr);
		for (int i=0; i<randomSeqAmount; i++) { //create random sequences
			for (int j=0; j<50; j++) {
				randomSeqs[i][j] = randomLetter();
			}
		}
		time_t createRandomEnd = time(nullptr);

		time_t alignmentStart = time(nullptr);
		char * cov_snippet = new char[100];
		for (int i=0; i<randomSeqAmount; i++) {
			cov_snippet = cov_snippet_finder(COVIDGen, seq1size, covid_seeds, covid_seeds_length, randomSeqs[i], seq2size);
			if (cov_snippet[0] != '\0') {
				Alignment alignment = SW_algorithm(cov_snippet, randomSeqs[i], 100, seq2size, simScore, penalty);
			}
		}
		time_t alignmentEnd = time(nullptr);

		cout << "Total time to create " << randomSeqAmount << " random sequences: " << createRandomEnd-createRandomStart << " (s)" << endl;
		cout << "Total time to perform alignment on all " << randomSeqAmount << " random sequences: " << alignmentEnd-alignmentStart << " (s)" << endl;

	}

	else if(strcmp(argv[1], "Problem2C") == 0) {

		int randomSeqAmount = atoi(argv[2]);

		char * COVIDGen = new char[29903];
		COVIDGen = Appendix_A();

		int covid_seeds_length = 29903 - 10;
		char ** covid_seeds = new char*[covid_seeds_length];
		for (int i=0; i<covid_seeds_length; i++) {
			covid_seeds[i] = new char[11];
			for(int j=0; j<11; j++) {
				covid_seeds[i][j] = COVIDGen[i+j];
			}
		}

		int seq1size = 29903;
		int seq2size = 50;
		int simScore[2] = {-3, 3}; //{mismatch, match}

		int penalty = -2; //gap penalty

		char randomSeqs[randomSeqAmount][50];

		for (int i=0; i<randomSeqAmount; i++) { //create random sequences
			int randomStart = rand() % (29903 - 10);
			for (int j=0; j<50; j++) {
				randomSeqs[i][j] = COVIDGen[randomStart+j];
			}
		}

		//i just look for matching fragments here, i don't do any alignments
		time_t alignmentStart = time(nullptr);
		int finds = 0;
		char * cov_snippet = new char[100];
		for (int i=0; i<randomSeqAmount; i++) {
			cov_snippet = cov_snippet_finder(COVIDGen, seq1size, covid_seeds, covid_seeds_length, randomSeqs[i], seq2size);
			if (cov_snippet[0] != '\0') {
				finds++;
			}
		}
		time_t alignmentEnd = time(nullptr);

		cout << "Total time to search all " << randomSeqAmount << " random fragments: " << alignmentEnd-alignmentStart << " (s)" << endl;
		cout << finds << " fragments were found" << endl << endl;

		for (int i=0; i<randomSeqAmount; i++) { //put errors in
			for (int j=0; j<50; j++) {
				int randomChance = rand() % 20; //5% Chance

				if (randomChance == 0) {
				randomSeqs[i][j] = randomLetter(); //new random letter. New letter could be the same as the old one
				}
			}
		}

		time_t alignment2Start = time(nullptr);
		finds = 0;
		for (int i=0; i<randomSeqAmount; i++) {
			cov_snippet = cov_snippet_finder(COVIDGen, seq1size, covid_seeds, covid_seeds_length, randomSeqs[i], seq2size);
			if (cov_snippet[0] != '\0') {
				finds++;
			}
		}
		time_t alignmen2tEnd = time(nullptr);

		cout << "After adding random errors:" << endl;
		cout << "Total time to search all " << randomSeqAmount << " random fragments: " << alignmentEnd-alignmentStart << " (s)" << endl;
		cout << finds << " fragments were found" << endl;

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
