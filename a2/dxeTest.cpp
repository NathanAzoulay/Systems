// dxe.cpp
// CS530, Spring 2020
// Assignment #2, SIC/XE Disassembler
// Nathan Azoulay, cssc1915

#include <algorithm>
#include <string.h>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string> 
#include <iomanip> 
#include <fstream> 
#include "dxe.h"

using namespace std; 


/* 
 * Method readInput opens a file given the file name and its 
 * appropriate extension. This method returns the file pointer
 * to the open file.
 */
FILE *readInput(char *argv[], const char *ptr){
    char buffer[30];
    FILE *inFile;
    strcpy(buffer,argv[1]);
    strcat(buffer, ptr);
    inFile = fopen(buffer,"r");
    return inFile;
} // end method readInput()

// This method checks conditions:
// error if there isnt 2 arguments at command line
// or if files are non existent throw error
void inputErrorCheck(int argc, char *argv[]){

    // if not even arguments at command line
    if(argc != 2){
        cout << "Error, command requires two arguments" << endl
             << "Enter: dxe <filename>" <<endl;
        exit(1);
    }
    
    // get file pointers 
    FILE *fptr[2] = {readInput(argv, ".sym"), readInput(argv, ".obj")};

    // if files do not exist or cannot be opened, display error
    if((fptr[0] == NULL) ||( fptr[1] == NULL)){
        cout << "Error, files failed to open." << endl
             << "Check that they exist and are valid" <<endl
             << "Enter: dxe <filename>" <<endl;
        fclose(fptr[0]); // close file pointers
        fclose(fptr[1]);
        exit(1);
    }
    // close file pointers even if no errors since we don't need them since
    // we created them in our main method as fptr
    fclose(fptr[0]); 
    fclose(fptr[1]);
} // end method inputErrorCheck()

// SIC registers attributes (name and register number)
struct regSIC{
    string name;
    string num;
};

// Register names with associated numbers
const struct regSIC regSIC[]={
    {"A","0"},
    {"X","1"},
    {"L","2"},
    {"B","3"},
    {"S","4"},
    {"T","5"},
    {"F","6"},
    {"PC","8"},
    {"SW","9"}
};

// This method returns the register name given the number as input
string getRegister(string register_num){
    for(int i=0; i<=8;i++){
        if(regSIC[i].num == register_num){
            return regSIC[i].name;
        }
    }
    return "";
} // end method getRestister(string)

// opTab attributes are instruction name, opcode, and its format(s)
struct opTab{
    string name;
    string opcode;
    string format;
};

// opTable with stored instructions, opcodes, and formats
const struct opTab opTab[] = {
    { "ADD", "18", "3/4" },       { "ADDR", "90", "2" },      { "ADDF", "58", "3/4" },   { "AND", "40", "3/4" },
    { "CLEAR", "B4", "2" },     { "COMP", "28", "3/4" },      { "COMPF", "88", "3/4"},   { "COMPR", "A0", "2" },
    { "DIV", "24", "3/4" },     { "DIVR", "9C", "2" },        { "DIVF", "64", "3/4" },   { "FIX", "C4", "1" },
    { "FLOAT", "C0", "1" },       { "HIO", "F4", "1" },       { "J", "3C", "3/4" },      { "JEQ", "30", "3/4" },
    { "JGT", "34", "3/4" },     { "JLT", "38", "3/4" },       { "JSUB", "48", "3/4" },   { "LDA", "00", "3/4" },
    { "LDB", "68", "3/4" },     { "LDCH", "50", "3/4" },      { "LDL", "08", "3/4" },    { "LDF", "70", "3/4" },
    { "LDS", "6C", "3/4" },       { "LDT", "74", "3/4" },     { "LDX", "04", "3/4" },    { "LPS", "D0", "3/4" },
    { "MUL", "20", "3/4" },     { "MULR", "98", "2" },        { "MULF", "60", "3/4" },   { "NORM", "C8", "1" },
    { "OR", "44", "3/4" },     { "RD", "D8", "3/4" },         { "RMO", "AC", "2" },       { "RSUB", "4C", "3/4" },
    { "SHIFTL", "A4", "2" },      { "SHIFTR", "A8", "2" },    { "SIO", "F0", "1" },      { "SSK", "EC", "3/4" },
    { "STA", "0C", "3/4" },     { "STF", "80", "3/4" },       { "STB", "78", "3/4" },       { "STCH", "54", "3/4" },
    { "STI", "D4", "3/4" },     { "STL", "14", "3/4" },       { "STS", "7C", "3/4" },     { "STSW", "E8", "3/4" },
    { "STT", "84", "3/4" },       { "STX", "10", "3/4" },     { "SUB", "1C", "3/4" },    { "SUBR", "94", "2" },
    { "SUBF", "5C", "3/4" },    { "SVC", "B0", "2" },         { "TD", "E0", "3/4" },        { "TIO", "F8", "1" },
    { "TIX", "2C", "3/4" },     { "TIXR", "B8", "2" },        { "WD", "DC", "3/4" }
};

// return index of opcode if it is in opTab, else return -1
int indexOpCode(string op){
    for(int i=0; i<59;i++){
        if(opTab[i].opcode.compare(op) == 0){
            return i;
        }
    }
    return -1;
} // end indexOpCode()

// get instruction given index of op table
string getInstruction(int indexOpTab){
    string result = opTab[indexOpTab].name;
    return result;
} // end getInstruction()

// get the format of instruction at index of OpTable
string getFormatInstr(int indexOpTab){
    string result = opTab[indexOpTab].format;
    return result;
} // end getFormatInstr()
	

// Method takes in file pointers and all our vectors declared in main
// The result of our method is the populated vectors with the symbol and
// literal table correctly stored
void readSym(FILE **fptr,vector<string> &symbols, vector<int> &addresses, 
                vector<char> &flags, vector<string> &literals, vector<int> &lens,
                vector<string> &names, vector<int> &litAddr){
	
    
    // variable storage
    char lines[160];
    char res[10];
    int val;
    bool check = true;
   
    // file pointer move over symbol file header
    fgets(lines,50,fptr[0]);
    
    // move file pointer over symbol data
    fgets(lines,80,fptr[0]);

    // start reading symbol file
    while(check){

        // excludes whitespace, store result in res
        fscanf(fptr[0],"%s",res);

        // if res == "Name" then we break and begin reading literal table
        if(strcmp(res,"Name")==0){
	    check = false;
            break; 
        }
        
        // if we are not in literal table, assume res is a symbol name
        // push this into symbols
        symbols.push_back(res);

        // get hex address and push into addresses
        fscanf(fptr[0],"%x", &val);
        addresses.push_back(val);

        // exclude whitespace, store result in res
        // take first char that is our flag and store in flags 
        fscanf(fptr[0],"%s",res);
        flags.push_back(res[0]);
    }


	for(int i=0; i < flags.size(); i++){
		cout << flags[i] << endl;
	}

	
 	for(int i=0; i < symbols.size(); i++){
		cout << symbols[i] << endl;
	}	


	for(int i=0; i < addresses.size(); i++){
		cout << addresses[i] << endl;
	}
// TEST TO PRINT OUT FLAGS, ADDRESSES, and SYMBOLS

	// skip over header section of literal table	
	fgets(lines,70,fptr[0]);
	fscanf(fptr[0],"%s",res);
	
	// read rest of .sym file (literal table)
	int j=0;	
	while(fgets(lines,80,fptr[0])){
	    // exclude whitespace scan and store in res
	    fscanf(fptr[0],"%s",res);
            
	    // check if first character of res is an "="
	    // if true, indicates literal doesn't have a name 
	    // populate names with empty string and populate literals,lens,litAddr vectors
	   // if(strncmp(&res[0],"=",1)==0){
		if(&res[0] == "="){
			names.push_back(" "); // no name literal
			literals.push_back(res); // populate literal vector

		// duplicate bug fix
			literals.erase(unique(literals.begin(),literals.end()),literals.end());
		
		// scan length, store in val and push onto lens<int>
			fscanf(fptr[0],"%x",&val);
			lens.push_back(val);

		// scan for literal address, store in val, push onto litAddr<int>
			fscanf(fptr[0],"%x",&val);
			litAddr.push_back(val);
		
		// duplicate bug fix
			litAddr.erase(unique(litAddr.begin(),litAddr.end()),litAddr.end());
	    } // end if
	    // case where literal has a name, populate name with res
	    // continue to populate literals,lens,litAddr vectors
	    else {
		// literal has a name, so push res into names<string>
			names.push_back(res);

		//duplicate bug fix
			names.erase(unique(names.begin(),names.end()),names.end());

		// scan literal, store in res, push onto literals<string>
			fscanf(fptr[0], "%s",res);
			literals.push_back(res);
		// scan length, store in val, push onto lens<int>
			fscanf(fptr[0],"%x",&val);
			lens.push_back(val);
		// scan literal address, store in val, push onto litAddr<int>
			fscanf(fptr[0],"%x",&val);
			litAddr.push_back(val);
			litAddr.erase(unique(litAddr.begin(),litAddr.end()),litAddr.end());
	    }	// end else
	 j++;
	} // end while loop
	
  // TEST FOR {names,literals,lens,litAddr}
	for(int i=0; i < names.size(); i++){
		cout << names[i] << endl;
	}
	for(int i=0; i < literals.size(); i++){
		cout << literals[i] << endl;
	}
	for(int i=0; i < lens.size(); i++){
		cout << lens[i] << endl;
	}
	for(int i=0; i < litAddr.size()-1;i++){
		cout << litAddr[i] << endl;
	}
        
} // end readSym()


// Method takes in a file pointer and returns the ending address of the file passed
int getEndAddress(FILE **fptr){
    char buff[120];
    rewind(fptr[1]);
    fgets(buff,120,fptr[1]);
    string res = buff;
    int result = strtol(res.substr(13,6).c_str(),0,16);
    return result;
}


// This method reads in the object code from the .obj file
void readObj(FILE **fptr, vector<string> &symbols, vector<int> &addresses, vector<char> &flags,
                vector<string> &literals, vector<int> &lens, vector<string> &names, vector<int> &litAddr, ofstream &sic, ofstream &lis){

    int address = 000000; // address that we will print for each line
    int maxSize = 180; // max length of a record

    
    bool n,i,x,b,p,e = false; // flags set to false by default
    char line[maxSize]; // memory storage for each line that we will break down
    int pos; // gives us position or index of the line in the record
    bool base = false; // false until BASE directive is printed
    bool litOp = false; // tells us if literal operand is present
	
	// loop through each line of obj file
	while(fgets(line, maxSize, fptr[1])){
		string curr = line; // store current record into string curr
		pos = 0; // position reset to 0 when loops over again
	
// TEST WITHIN EACH IF TO MAKE SURE IT IS READING CORRECTLY
	
		// check if line is a header record
		if(line[0] == 'H'){
			cout << "Header Record" << endl;
		} // end if for H record
		
		// check if line is a text record
		if(line[0] == 'T'){
			cout << "Text Record" << endl;
		} // end if for T record
		
		// check if line is a modification record
		if(line[0] == 'M'){
			cout << "Modification Record" << endl;
		} // end if for M record
		
		// else assume we are at the end record
		if(line[0] == 'E'){
			cout << "End record" << endl;
		} // end else for E record
		
		
	} // end while loop
} // end method readObj()


void createOutFiles(char *argv[],ofstream &sic,ofstream &lis){
	// create a stream for our output files
    // create a .sic and .lis.txt file with our argv[1] 
    char buffer[20];
    strcpy(buffer,argv[1]);
    strcat(buffer, ".sic");
    sic.open(buffer);
	
    // "lis.txt" file
    strcpy(buffer,argv[1]);
    strcat(buffer,".lis.txt");
    lis.open(buffer);
} // end method createOutFiles()

int main(int argc, char *argv[]){

    // check that files can open and exist
    inputErrorCheck(argc,argv);

    // open files, get pointers
    FILE *fptr[2] = {readInput(argv, ".sym"), readInput(argv, ".obj")};

    // memory to store symbols and literals from sym file
    vector<string> symbols;
    vector<int> addresses;
    vector<char> flags;
    // literals
    vector<string> literals;
    vector<int> lens;
    vector<string> names;
    vector<int> litAddr;
    
    // read symbol file, populate our vectors
    readSym(fptr,symbols,addresses, flags, literals, lens, names, litAddr);
	
	// create output files
	ofstream sic,lis;
	createOutFiles(argv,sic,lis); // creates both output files(empty)
    
	
    // read obj file once method is working
    readObj(fptr, symbols, addresses, flags, literals, lens,names, litAddr, sic, lis);
	
    // get ending address of program
    int finalAddress = getEndAddress(fptr);
	
    // call a method to parse the data into the output files properly
    // parseData(fptr, symbols, addresses, flags, finalAddress, sic, lis);
	
    // close output files
    sic.close();
    lis.close();
	
    // close file pointers
    fclose(fptr[0]);
    fclose(fptr[1]); 
    return 0;
} // end main method

