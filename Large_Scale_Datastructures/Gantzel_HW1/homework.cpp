
#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;

class FASTA_readset {
	public:
		void readFASTA(char* location);

		char genSequence[10000][51];
		char headers[10000][51];
		int unique_seq;
		int dataset_reads[14];
		int A_counter;
		int C_counter;
		int G_counter;
		int T_counter;
		int other_counter;

		void statistics_gen(); //generates statistics
		void genomic_sort(); // sorts all sequences in alphabetic order

		FASTA_readset(){};
		FASTA_readset(char* location, int readLimit) {
			ifstream input;
			input.open(location);

			for (int i = 0; i < readLimit; i++){
				input >> headers[i];
				input >> genSequence[i];
			}
			input.close();
		}

		~FASTA_readset(){}; //destructor
};

void FASTA_readset::readFASTA(char* location) {

	ifstream input;
	input.open(location);
	int i = 0;

	while(!input.eof()){
		input >> headers[i];
		input >> genSequence[i];
		i++;
	}

	input.close();
}

void FASTA_readset::statistics_gen() {
	int i = 0;
	int header_length;
	int a, b, c;

	unique_seq = 0;
	for (int k=0; k<14; k++) {
		dataset_reads[k] = 0;
	}
	A_counter = 0;
	C_counter = 0;
	G_counter = 0;
	T_counter = 0;
	other_counter = 0;

	while(headers[i][0] != '\0') {

		unique_seq = unique_seq + 1;

		header_length = 0;
		while(headers[i][header_length] != '\0') { // '\0' is a special end-of-string character
			header_length++;
		}

		a = 0;

		for(int x=0; x<14; x++) {

			while(headers[i][a] != '_' && headers[i][a] != '\0') {
				a++;
			}

			b = 1;
			while(headers[i][a+b] != '_' && headers[i][a+b] != '\0') {
				b++;
			}
			if (b > 2) {
				dataset_reads[x] = dataset_reads[x] + ((headers[i][a+1] - '0') * 10) + (headers[i][a+2] - '0');
			}
			else {
				dataset_reads[x] = dataset_reads[x] + (headers[i][a+1] - '0');
			}
			a++;
			//assuming the maximum number of reads for each unique sequence fragment for each dataset is 99
		}

		c = 0;
		while (genSequence[i][c] != '\0') {
			if (genSequence[i][c] == 'A') {
				A_counter++;
			}
			else if (genSequence[i][c] == 'C') {
				C_counter++;
			}
			else if (genSequence[i][c] == 'G') {
				G_counter++;
			}
			else if (genSequence[i][c] == 'T') {
				T_counter++;
			}
			else {
				other_counter++;
			}
			c++;
		}

		i++;
	}
}

void FASTA_readset::genomic_sort() {

	int x, y;
	int genlength=0;
	char tempSequence[51];

	while (genSequence[genlength][0] != '\0') {
		genlength++;
	}

	for (int i=0; i<genlength; i++) {

		for (int j=0; j<genlength; j++) {
			x=0;
			while (genSequence[j][x] != '\0') {

				if (genSequence[j][x] == 'A') {
					if (genSequence[j+1][x] == 'A') {
						x++;
					}
					else {
						break;
					}
				}

				else if (genSequence[j][x] == 'C') {

					if (genSequence[j+1][x] == 'A') {
						y=0;
						while (genSequence[j+1][y] != '\0') {
							tempSequence[y] = genSequence[j][y];
							genSequence[j][y] = genSequence[j+1][y];
							genSequence[j+1][y] = tempSequence[y];
							y++;
						}
						break;
					}

					else if (genSequence[j+1][x] == 'C') {
						x++;
					}

					else {
						break;
					}

				}

				else if (genSequence[j][x] == 'G') {

					if (genSequence[j+1][x] == 'A' || genSequence[j+1][x] == 'C') {
						y=0;
						while (genSequence[j+1][y] != '\0') {
							tempSequence[y] = genSequence[j][y];
							genSequence[j][y] = genSequence[j+1][y];
							genSequence[j+1][y] = tempSequence[y];
							y++;
						}
						break;
					}

					else if (genSequence[j+1][x] == 'G') {
						x++;
					}

					else {
						break;
					}
				}

				else if (genSequence[j][x] == 'N') {

					if (genSequence[j+1][x] == 'A' || genSequence[j+1][x] == 'C' || genSequence[j+1][x] == 'G') {
						y=0;
						while (genSequence[j+1][y] != '\0') {
							tempSequence[y] = genSequence[j][y];
							genSequence[j][y] = genSequence[j+1][y];
							genSequence[j+1][y] = tempSequence[y];
							y++;
						}
						break;
					}

					else if (genSequence[j+1][x] == 'N') {
						x++;
					}

					else {
						break;
					}
				}

				else if (genSequence[j][x] == 'T') {

					if (genSequence[j+1][x] == 'A' || genSequence[j+1][x] == 'C' || genSequence[j+1][x] == 'G' || genSequence[j+1][x] == 'N') {
						y=0;
						while (genSequence[j+1][y] != '\0') {
							tempSequence[y] = genSequence[j][y];
							genSequence[j][y] = genSequence[j+1][y];
							genSequence[j+1][y] = tempSequence[y];
							y++;
						}
						break;
					}

					else if (genSequence[j+1][x] == 'T') {
						x++;
					}

					else {
						break;
					}
				}

				else {
					x++;
				}

			}

		}

	}

}


int main(int argc, char ** argv){

	if (argc != 3){  // unexpected program call
		cout << endl << endl << "=========================="<< endl;
		cout << "Error: 2 input parameters expected" <<endl;
		cout << "Proper usage is:" <<endl;
		cout << "./homework <problem-flag> <filepath>" << endl;
		cout << "Example:" << endl;
		cout << "./homework problem1A sample_hw_dataset.fa"  << endl;
		cout << "=========================="<< endl << endl;
		cout << "exiting..." << endl;
		exit(-1);
	}else{

		cout << "The number of arguments passed: " << argc << endl;
		cout << "The first argument is: " << argv[0] << endl;
		cout << "The second argument is: " << argv[1] << endl;
		cout << "The third argument is: " << argv[2] << endl;
	}


	FASTA_readset gendata(argv[2], 1000);
	gendata.statistics_gen();
	gendata.genomic_sort();



	cout << "number of unique sequences: " << gendata.unique_seq << endl;
	for (int x=0; x<14; x++) {
		cout << "number of reads in dataset " << x+1 << ": " << gendata.dataset_reads[x] << endl;
	}
	cout << "number of A's: " << gendata.A_counter << endl;
	cout << "number of C's: " << gendata.C_counter << endl;
	cout << "number of G's: " << gendata.G_counter << endl;
	cout << "number of T's: " << gendata.T_counter << endl;
	cout << "number of other charaters (like N's): " << gendata.other_counter << endl;

	int i = 0;

	while(gendata.headers[i][0] != '\0') {
		cout << "sequence nr: " << i << endl;
		cout << "sequence is " << gendata.genSequence[i] << endl;
		i++;
	}

	return (0);
}
