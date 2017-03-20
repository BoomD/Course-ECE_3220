#include <iostream>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <cstdlib>
#include <limits.h>
#include <vector>
#include <string>
#include <sstream>


using namespace std;

class Signal { 
	private:
		float max;
		float min;
		float average;
		char* file_name;
	public:
		float sum;
		int length;// change length to in public because non-member operator funtion
		vector<float>data;
		void read_data(char *);
		int find_Max();
		int find_Min();
		int find_Average();
		//void offseting(float); these four funtions are no longer need anymore because of operation function
		//void scaling(float);
		//void center();
		//void normalize();
		void statistics();
		void Sig_info();
		void Save_file(char *file_name);
		void display_menu();
		Signal();
		Signal(int file_num);
		Signal(char *file_name);
		~Signal();
		void operator+(float);
		void operator-(float);
		void operator*(float);
		void operator/(float);
};

int main(int argc, char **argv){
	Signal con ;
    printf("\nArgc here = %d\n",argc);
	if(argc == 3){//argc should be three, otherwise call default constructor
		if(!(strcmp(argv[1], "-f"))){
			con = Signal(argv[2]);	
		}
		else if(!(strcmp(argv[1], "-n"))){
			int num = atoi(argv[2]);
			con = Signal(num);	
		}
        else{
            cout << "\n\nCall default conconstructor\nLength, max, min, avgerage sets to zero\nFile sets to NULL" << endl;
			cout << "Option 1~4 and 7 will casuse Segmentation Fault Because of NULL" << endl;
			con = Signal();
        }
	}
	else {
		cout << "\n\nCall default conconstructor\nLength, max, min, avgerage sets to zero\nFile sets to NULL" << endl;
		cout << "Option 1~4 and 7 will casuse Segmentation Fault Because of NULL" << endl;
		con = Signal();
	}

    while(1){
        con.display_menu();
    }
	return 0;
}

Signal::Signal(int file_num) {// constructor, takes file #
	char name[30];
	sprintf(name,"Raw_data_%02d.txt", file_num);
	//printf("\n%d\n",strlen(name));
	read_data((char*)name);	
}

Signal::Signal(char *filename) {// constructor, takes filename
	this->file_name = filename;
	read_data(this->file_name);	
}

Signal::Signal() {//read default file
    length = 0;
	max = 0;
	min = 0;
	average = 0;
	file_name = NULL;	
}

Signal::~Signal() {
	
}

void Signal::display_menu(){
		cout << " \n*****Singals options:*****"<< "\n1) offseting signals" << "\n2) Scale signals" << "\n3) Center signals" << "\n4) Normalize signals" << "\n5) Show Statistics of signals" << "\n6) show information of signals" << "\n7) Save signals into output file" << "\n8) Test Strings " << "\n9) Exit" << endl;

		int choice;
		cin >> choice;

		switch(choice)
		{
			case 1: { 	
					cout << "\n Enter the offset value: ";
					float off;
					cin >> off;
					operator+(off);//change to operator function, offset = +
					break;
			}
			case 2: { 	cout << "\nEner the scale value: ";
				        float sca;
					cin >> sca;
					operator*(sca);//change to operator function, offset = *
					break; 
			}
			case 3: { 	
					operator-(average); //change to operator function, center = - average
					break;	
			}
			case 4: { 	
					operator/(max);//change to operator function, normalize = / max
					break;
			}
			case 5: {	
					statistics();
					break;
			}
			case 6: {	
					Sig_info();
					break;
			}
			case 7:	{	
					cout << "\nEnter the filename: ";
					char saved_file[100] = {'\0'};
					cin >> saved_file;
					Save_file(saved_file);
					break;
			}
			case 8: {
					string text;// show usage of string
					cout << "Enter some text, finish it with an '!':" << endl;
					getline(cin, text, '!');
					cout << "The text you enter is: " << text << endl;
			}
			case 9: {
					exit(0);
			}
			default: { 	 
					cout << "Wrong option, enter again: " << endl; 
					break;
				 }
		}
	}

void Signal::Save_file( char *filename) {// save file
	FILE *f;
	if((f = fopen(filename, "w")) == NULL){
		cout << "can't write tofile "<< endl;
	}
	fprintf(f, "%d %f\n", length, max);
	for(int i=0; i<length; i++){
		fprintf(f, "%f\n", data[i]);
	}
	fclose(f);
	
}

void Signal::statistics() {
	cout << "\nAverage value of signal: " << this->average << endl;
	cout << "Maximum value of signal:" << this->max << endl;
	cout << "Minimum value of signal:" << this->min << endl;
}

void Signal::Sig_info() {//shows max, min, and avg
	cout << "\nlength of signal:: " << this->length << endl;
	cout << "Maximum value of signal:" << this->max << endl;
	cout << "Minimum value of signal:" << this->min << endl;
	cout << "Average value of signal:" << this->average << endl;
}



void Signal::read_data( char *filename) {// read file
	FILE *f;
	if((f = fopen(filename, "r")) == NULL){
		cout << "can't write to file" <<endl;
	}
	fscanf(f, "%d %f", &length, &max);
	max = -1000000;
	int i;
	float trans_into_vector;
	for(i=0; i<length; i++){
		fscanf(f, "%f", &trans_into_vector);
		data.push_back(trans_into_vector);//save into vector
	}
    this->find_Average();
	this->find_Max();
    this->find_Min();
	fclose(f);

}

int Signal::find_Average() {//find avg
	int i;
    float total = 0;
	for(i=0; i<length; i++){
        total += data[i];
	}
    average = (total/ (float)length);
	if (average < 0.00001 && average > -0.00001) average = 0;
	return average;
}

int Signal::find_Max() {//find max
	int i;
	max = data[0];
	for(i=1; i<length; i++){
		if( data[i] > max ){
			max = data[i];
		}
	}
	return max;
}	

int Signal::find_Min(void) {//find min
	int i;
	min = data[0];
	for(i=1; i<length; i++){
		if( data[i] < min ){
			min = data[i];
		}
	}
	return min;
}	

void Signal::operator+(float offset) {// operator + is equal to offset
	int i;
	for(i=0; i<length; i++){
		data[i] += offset;
	}
	this->find_Average();
	this->find_Max();
    this->find_Min();
}

void Signal::operator-(float average) {// operator - is equal to center
	int i;
	for(i=0; i<length; i++){
		data[i] -= average;
	}
	this->find_Average();
	this->find_Max();
    this->find_Min();
}

void Signal::operator*(float scale) {// operator * is equal to scale
	int i;
	for(i=0; i<length; i++){
		data[i] *= scale;
	}
	this->find_Average();
	this->find_Max();
    this->find_Min();
}

void Signal::operator/(float max) {// operator / is equal to normalize
	int i;
	for(i=0; i<length; i++){
		data[i] /= max;
	}
	this->find_Average();
	this->find_Max();
    this->find_Min();
}

Signal operator+(const Signal &lhs, const Signal &rhs){// addition operator
	Signal sum;
	int i;
	for(i=0; i<sum.length; i++){
		sum.data[i] = lhs.data[i] + rhs.data[i];
	}
	sum.find_Average();
	sum.find_Max();
    sum.find_Min();
	
	return sum;
}