#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <limits.h>



using namespace std;

class Signal { 
	private:
		int length;
		float max;
		float min;
		float average;
		float *data;
		char* file_name;
		
		
	public:

		void read_data(char *);
		int find_Max();
		int find_Min();
		int find_Average();
		void offseting(float);
		void scaling(float);
		void center();
		void normalize();
		void statistics();
		void Sig_info();
		void Save_file(char *file_name);
		void display_menu();
		Signal();
		Signal(int file_num);
		Signal(char *file_name);
		~Signal();
};

			 

int main(int argc, char **argv){
	Signal con ;
    printf("\nargc here = %d\n",argc);
	if(argc == 3){//argc should be three, otherwise call default constructor
		if(!(strcmp(argv[1], "-f")))
		{
			con = Signal(argv[2]);	
		}
		else if(!(strcmp(argv[1], "-n"))){
			int num = atoi(argv[2]);
			con = Signal(num);	
		}
        else{
            printf("wrong input stytle");
        }
	}
	else {
		cout << "call default conconstructor ,with the file Raw_data_01.txt" << endl;
		con = Signal("Raw_data_01.txt");
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
    read_data("Raw_data_01.txt");
}

Signal::~Signal() {
		data = NULL;
	
}


void Signal::display_menu(){
		cout << " \n*****Singals options:*****"<< "\n1) offseting signals" << "\n2) Scale signals" << "\n3) Center signals" << "\n4) Normalize signals" << "\n5) Show Statistics of signals" << "\n6) show information of signals" << "\n7) Save signals into output file" << "\n8) Exit" << endl;

		int choice;
		cin >> choice;

		switch(choice)
		{
			case 1: { 	
					cout << "\n offset value: ";
					float off;
					cin >> off;
					offseting(off);
					break;
				}
			case 2: { 	cout << "\nscale value: ";
				        float sca;
					cin >> sca;
					scaling(sca);
					break; 
				}
			case 3: { 	
					center(); 
					break;	
				}
			case 4: { 	
					normalize();
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
					cout << "Enter a filename: ";
					char saved_file[100] = {'\0'};
					cin >> saved_file;
					Save_file(saved_file);
					break;
				}
			case 8: {
					exit(EXIT_SUCCESS);
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
	
void Signal::offseting(float offset) {// offset signals
	int i=0;
	
	for(i=0; i<length; i++)
	{
		data[i] += offset;
	}
	
	find_Average();
	find_Max();
    this->find_Min();

}

void Signal::scaling(float scale) {// scale signals
	int i;

	for(i=0; i<length; i++)
	{
		data[i] *= scale;
	}
	
	find_Average();
	find_Max();
    this->find_Min();
}

void Signal::center() {//center signals
	int i=0;

	
	for(i=0; i<length; i++)
	{   
        this->data[i] = this->data[i] - average;
	}

    this->find_Average();
	this->find_Max();
    this->find_Min();
}

void Signal::normalize() {//center signals
	int i;
	
	this->find_Max();
	
	for(i=0; i<length; i++)
	{
		this->data[i] /= max;
	}
	
	this->find_Average();
	this->find_Max();
    this->find_Min();
}

void Signal::statistics() {
	cout << "\nAverage: " << this->average << endl;
	cout << "Maximum val:" << this->max << endl;
	cout << "Minimum val:" << this->min << endl;
}

void Signal::Sig_info() {//shows max, min, and avg

	cout << "\nSignal length: " << length << endl;
	cout << "Maximum val:" << max << endl;
	cout << "Minimum val:" << min << endl;
	cout << "Average val:" << average << endl;
}



void Signal::read_data( char *filename) {// read file

	FILE *f;
	if((f = fopen(filename, "r")) == NULL){
		cout << "can't write to file" <<endl;
	}
	fscanf(f, "%d %f", &length, &max);
	max = -1000000;
	data = new float[length];
	int i=0;
	for(i=0; i<length; i++){
		fscanf(f, "%f", data+i);
	}

    this->find_Average();
	this->find_Max();
    this->find_Min();
	fclose(f);

}

int Signal::find_Average(void) {//find avg
	int i;
    float total = 0;
	for(i=0; i<length; i++)
	{
        total += data[i];
	}
    average = (total/ (float)length);

	if (average < 0.00001 && average > -0.00001) average = 0;
	return average;
}

int Signal::find_Max(void) {//find max
	int i;
	max = data[0];
	for(i=1; i<length; i++)
	{
		if( data[i] > max ){
			max = data[i];
		}
	}
	return max;
}	

int Signal::find_Min(void) {//find min
	int i;
	min = data[0];
	for(i=1; i<length; i++)
	{
		if( data[i] < min ){
			min = data[i];
		}
	}
	return min;
}	




