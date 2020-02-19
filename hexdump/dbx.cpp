// Nathan Azoulay
// cssc1915
// February 17, 2020
// Hexdump program: Open and read every byte in file and write ASCII
// hex value or binary and its printable character to standard output.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <fstream>

using namespace std;
unsigned char * readInput(const char *);
void outBin(unsigned char *,long);
string binConv(int);
void outHex(unsigned char *,long);

long maxSize;

// main method calls readInput() and
// outBin() or outHex()
int main(int argc, char * argv[]){

	// use file location to call valid method
	int fileLocation;
	if((argc==3)==true){
		fileLocation = 2;
		unsigned char * buffer = readInput(argv[fileLocation]);
		outBin(buffer,maxSize); 
	}
	else{
		fileLocation = 1;
		unsigned char * buffer = readInput(argv[fileLocation]);
		outHex(buffer,maxSize);
	}

	unsigned char * buffer = readInput(argv[fileLocation]);
	cout<<endl;

	// clear buffer
	free(buffer);
	return 0;
}	

// readInput() takes in a file, counts #
// of bytes in the file and reads it into
// an unsigned char array (buffer)
unsigned char * readInput(const char * fileName){
	FILE * fp; // file pointer
	
	// open file, check if exist
	fp = fopen(fileName,"rb");

	if(fp == NULL){
		cout<< "Error with file" << endl;
		exit(1);
	}
	
	// set file position of stream
	fseek(fp,0, SEEK_END);
	// get number of bytes
	maxSize = ftell(fp);
	// go back beginning of file
	rewind(fp);
	
	// character array size of file
	unsigned char *buffer = (unsigned char*) malloc(sizeof(char)*maxSize);
	
	// store in char array # of bytes	
	fread(buffer,1,maxSize,fp);
	fclose(fp);	
	return buffer;

} // end method readInput()

// outBin() takes in buffer and # of bytes
// in the file, and calls binConv() to get
// binary out of the hex digits
void outBin(unsigned char * buffer, long maxSize){

	int rowBytes = 6; // max bin bytes per row
	int addr = 0000000; // starting address

	// loop through each byte in file
	for(int i=0; i<maxSize; i++){

		// print ASCII if multiple of 6 and not at starting byte
		if(i%rowBytes==0 && i != 0){
			cout<< " ";
			for(int j = rowBytes; j>0; j--){
				// check if readable character
				if(buffer[i-j]<' ' || buffer[i-j]>'~')
					cout<< '.';
				// print rest of human readable characters
				else {cout << buffer[i-j];}

			}
			cout<< endl;
		}

		// print address if at starting byte or multiple of 6
		if(i%rowBytes == 0){
			cout<< setfill('0')<< setw(7) << hex << addr << ": ";
			addr += rowBytes;
		}
		
		// convert hex to binary and print result
		cout << binConv(int(buffer[i]));
		
		// printing of last row's ASCII and spaces to fill
		if(i==maxSize-1){
			int lastRow = maxSize % rowBytes; // bytes in last row
			int remaining = rowBytes - lastRow; // get # bytes to fill with spaces
			int numSpaces = (remaining*9) + 3; // add 2 for spaces before
			cout << setfill(' ') << setw(numSpaces);

			for(int k = lastRow-1; k>=0;k--){
				// check if human readable character
				if(buffer[i-k]<' ' || buffer[i-k]>'~')
					cout<< '.';
				// print rest of readable character
				else {cout << buffer[i-k];}
			}
		}
		cout<< " "; // space between each byte
	}
} // end method outBin()


// binConv() method to convert hex to binary
string binConv(int num){
	// empty result string
	string binResult = "";
	
	// get first digit(d) of hex byte
	int d = num/16;
	for(int i = 0; i<2; i++){
			
		switch(d){
			case (0):
				binResult.append("0000");
				break;
			case (1):
				binResult.append("0001");
				break;
			case (2):
				binResult.append("0010");
				break;
			case (3):
				binResult.append("0011");
				break;
			case (4):
				binResult.append("0100");
				break;
			case (5):
				binResult.append("0101");
				break;
			case (6):
				binResult.append("0110");
				break;
			case (7):
				binResult.append("0111");
				break;
			case (8):
				binResult.append("1000");
				break;
			case (9):
				binResult.append("1001");
				break;
			case (10):
			case ('A'):
			case ('a'):
				binResult.append("1010");
				break;
			case (11):
			case ('B'):
			case ('b'):
				binResult.append("1011");
				break;
			case (12):
			case ('C'):
			case ('c'):
				binResult.append("1100");
				break;
			case (13):
			case ('D'):
			case ('d'): 
				binResult.append("1101");
				break;
			case (14):
			case ('E'):
			case ('e'):
				binResult.append("1110");
				break;
			case (15):
			case ('F'):
			case ('f'):
				binResult.append("1111");
				break;
		}	// end of switch
		// set digit to second hex digit in byte
		d = num%16;
	}
	return binResult;
} // end method binConv()

// outHex() takes the character and prints it in its
// hex format, by converting the int(buffer) using cout
// hex function
void outHex(unsigned char * buffer, long maxSize){
	
	int rowBytes = 16; // max hex bytes per row
	int startAddr = 0000000; // starting address 
	
	// loop through each byte in array
	for(int i=0; i<maxSize; i++){
		// print ASCII as long as not at byte 0 and is multiple of 16
		if(i%rowBytes == 0 && i !=0){
			cout<< " ";
			for(int j=rowBytes; j>0; j--){
				// print a period if not a human readable character
				if(buffer[i-j]<' ' || buffer[i-j]>'~'){
					cout << '.';
				}
				// prints readable character
				else{ cout<< buffer[i-j];}
			}
			cout<<endl;
		}
		
		// print and keep track of address in file if multiple of 16 or at byte 0
		if(i%rowBytes==0){
			cout<< setfill('0') << setw(7) << hex << startAddr << ": ";
			startAddr+= rowBytes;
		}
		
		// converts int to hex and prints using hex function
		cout << setfill('0') << setw(2) << hex << int(buffer[i]);
		
		
		// find remaining spaces needed to print rest of ASCII 
		if(i==maxSize-1){

			int lastRow = maxSize%rowBytes; // bytes in last row
			int leftinRow = rowBytes-lastRow; // get # bytes to fill with spaces
			int between = (leftinRow/2); // # spaces to display between bytes remaining
			int odd = (leftinRow%2); // if file has odd # bytes, need to account 1 more
			int numSpaces = ((leftinRow*2) + odd + between + 2); // add 2 for spaces before
			
			cout << setfill(' ') << setw(numSpaces);
			
			// print rest of ASCII bytes
			for(int k = lastRow-1; k >=0; k--){
				if(buffer[i-k]<' ' || buffer[i-k]>'~')
					cout<< '.';
				// print rest of  readable character
				else {cout<< buffer[i-k];}
			}
		}

		// space between two bytes
		if((i%2)-1 == 0) { cout << " "; }
	}
} // end method outHex()
