// dxe.cpp
// CS530, Spring 2020
// Assignment #2, SIC/XE Disassembler
// Nathan Azoulay, cssc1915


#include <map>
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



// Method readInput opens a file given the file name and its 
// appropriate extension. This method returns the file pointer
// to the open file.
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

void createInstructionMap(map<int,string> &imap){
    imap.insert(pair<int,string>(0x18,"ADD3"));
    imap.insert(pair<int,string>(0x90,"ADDR2"));
    imap.insert(pair<int,string>(0x58,"ADDF3"));
    imap.insert(pair<int,string>(0x40,"AND3"));
    imap.insert(pair<int,string>(0xB4,"CLEAR2"));
    imap.insert(pair<int,string>(0x28,"COMP3"));
    imap.insert(pair<int,string>(0x88,"COMPF3"));
    imap.insert(pair<int,string>(0xA0,"COMPR2"));
    imap.insert(pair<int,string>(0x24,"DIV3"));
    imap.insert(pair<int,string>(0x9C,"DIVR2"));
    imap.insert(pair<int,string>(0x64,"DIVF3"));
    imap.insert(pair<int,string>(0xC4,"FIX1"));
    imap.insert(pair<int,string>(0xC0,"FLOAT1"));
    imap.insert(pair<int,string>(0xF4,"HIO1"));
    imap.insert(pair<int,string>(0x3C,"J3"));
    imap.insert(pair<int,string>(0x30,"JEQ3"));
    imap.insert(pair<int,string>(0x34,"JGT3"));
    imap.insert(pair<int,string>(0x38,"JLT3"));
    imap.insert(pair<int,string>(0x48,"JSUB3"));
    imap.insert(pair<int,string>(0x00,"LDA3"));
    imap.insert(pair<int,string>(0x68,"LDB3"));
    imap.insert(pair<int,string>(0x50,"LDCH3"));
    imap.insert(pair<int,string>(0x08,"LDL3"));
    imap.insert(pair<int,string>(0x70,"LDF3"));
    imap.insert(pair<int,string>(0x6C,"LDS3"));
    imap.insert(pair<int,string>(0x74,"LDT3"));
    imap.insert(pair<int,string>(0x04,"LDX3"));
    imap.insert(pair<int,string>(0xD0,"LPS3"));
    imap.insert(pair<int,string>(0x20,"MUL3"));
    imap.insert(pair<int,string>(0x60,"MULF3"));
    imap.insert(pair<int,string>(0x98,"MULR2"));
    imap.insert(pair<int,string>(0xC8,"NORM1"));
    imap.insert(pair<int,string>(0x44,"OR3"));
    imap.insert(pair<int,string>(0xD8,"RD3"));
    imap.insert(pair<int,string>(0xAC,"RMO2"));
    imap.insert(pair<int,string>(0x4C,"RSUB3"));
    imap.insert(pair<int,string>(0xA4,"SHIFTL2"));
    imap.insert(pair<int,string>(0xA8,"SHIFTR2"));
    imap.insert(pair<int,string>(0xF0,"SIO1"));
    imap.insert(pair<int,string>(0xEC,"SSK3"));
    imap.insert(pair<int,string>(0x0C,"STA3"));
    imap.insert(pair<int,string>(0x78,"STB3"));
    imap.insert(pair<int,string>(0x54,"STCH3"));
    imap.insert(pair<int,string>(0x80,"STF3"));
    imap.insert(pair<int,string>(0xD4,"STI3"));
    imap.insert(pair<int,string>(0x14,"STL3"));
    imap.insert(pair<int,string>(0x7C,"STS3"));
    imap.insert(pair<int,string>(0xE8,"STSW3"));
    imap.insert(pair<int,string>(0x84,"STT3"));
    imap.insert(pair<int,string>(0x10,"STX3"));
    imap.insert(pair<int,string>(0x1C,"SUB3"));
    imap.insert(pair<int,string>(0x5C,"SUBF3"));
    imap.insert(pair<int,string>(0x94,"SUBR2"));
    imap.insert(pair<int,string>(0xB0,"SVC2"));
    imap.insert(pair<int,string>(0xE0,"TD3"));
    imap.insert(pair<int,string>(0xF8,"TIO1"));
    imap.insert(pair<int,string>(0x2C,"TIX3"));
    imap.insert(pair<int,string>(0xB8,"TIXR2"));
    imap.insert(pair<int,string>(0xDC,"WD3"));


}


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
		if(strncmp(&res[0],"=",1)==0){
			names.push_back(""); // no name literal
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
	
} // end readSym()


// Method takes in a file pointer and returns the ending address of the file passed
int getEndAddress(FILE **fptr){
    char buff[120];
    rewind(fptr[1]);
    fgets(buff,120,fptr[1]);
    string res = buff;
    int result = strtol(res.substr(13,6).c_str(),0,16);
    return result;
} // end method getEndAddress()


// This method reads in the object code from the .obj file
void readObj(FILE **fptr, vector<string> &symbols, vector<int> &addresses, vector<char> &flags,
                vector<string> &literals, vector<int> &lens, vector<string> &names, vector<int> &litAddr, ofstream &sic, ofstream &lis){
    map<int,string> opTab;
    createInstructionMap(opTab);
    int address = 000000; // address that we will print for each line
    int maxSize = 150; // max length of a record
    string opFormat;
    string match;
    bool n,i,x,b,p,e = false; // flags set to false by default
    char line[maxSize]; // memory storage for each line that we will break down
    bool immediate = false;
    int pos=0; // gives us position or index of the line in the record
    bool base = false; // false until BASE directive is printed
    bool litOp = false; // tells us if literal operand is present
    string objCode;
   
	// loop through each line of obj file
	while(fgets(line, maxSize, fptr[1])){
		string curr = line; // store current record into string curr
		pos = 0; // position reset to 0 when loops over again
		
		// check if line is a header record
		if(line[0] == 'H'){
            string progName = curr.substr(1,6);
            pos = pos + 7; // move position past the program name
            address = strtol(curr.substr(pos,6).c_str(),0,16);

            // write to lis and sic file the first line of program
            lis << setw(4) << setfill('0') <<  uppercase << hex << address << " ";
            lis << setw(8) << setfill(' ') <<  left << progName;
            lis << " START   " << address << endl;
            sic << setw(8) << setfill(' ') <<  left << progName;
            sic << " START   " << address << endl;

		} // end if for H record
		
		// check if line is a text record
		if(line[0] == 'T'){
            address = strtol(curr.substr(1,6).c_str(),0,12);
            pos = pos + 9; // (T + 6 nibbles address + 2 nibbles record length)

            // loop until end of line reached
            while(line[pos] != '\n'){
                n,i,x,b,p,e = false; // reset flag bits to 0
                
                // loop to see if current address has a symbol
                for(int i=0; i < addresses.size(); i++){
                    if(addresses[i] == address){
			 match = symbols[i];
			break;
		    }
		    else{
			match = "";
		    } 
			
                } // end loop
		
                for(int b=0; b<literals.size();b++){
		    
                    string literalT = literals[b].substr(1,1);
                    string lit = literals[b].substr(3,lens[b]);
                    string strLit;
		    if(address == litAddr[b]){
			sic << " " <<  setw(8) << setfill(' ');
			sic << " " << setw(8) << setfill(' ') << left << "LDA";
			sic << setw(9) << setfill(' ') << left << literals[b] << endl;
			lis << setw(4) << setfill('0') << hex << right << address << " ";
			lis << setw(8) << setfill(' ') << left << match;
			lis << " " <<  setw(8) << setfill(' ') << left << "LDA" << endl;
			
			break;
		    }
                    if(strcmp(&literalT[0],"X")==0){
                        
                    }
		    else if(strcmp(&literalT[0],"D")==0){

		    }
		    else{

		    }
			
                }
                // get opcode and perform logic to get the actual opcode in order to compare to our instruction set
                int opcode = strtol(curr.substr(pos,2).c_str(),0,12);
                int rem = opcode % 4; // remainder
                opcode -= rem;
		
		
		
                string opInstructionFormat = opTab[opcode]; // pull instruction with matching opcode
		        
                string opInstruction = opInstructionFormat.substr(0,opInstructionFormat.size()-1);
		        string opF = opInstructionFormat.substr(opInstructionFormat.size()-1,1);
		        int opFormat = stoi(opF); 
		        	
		
                // set n & i bits according to the remainder of operation
                if(rem == 1){
                    n = false;
                    i = true;
		            immediate = true;
                }
                else if(rem == 2){
                    n = true;
                    i = false;
                }
                else if(rem == 3){
                    n,i = true;
                }
                else{
                    n,i = false;
                }

                // cases for different instruction formats
                if(opFormat == 1){
                    // prints address
                    lis << hex << address << " ";
                    
                }
                else if(opFormat == 2){
                    lis << setw(4) <<  setfill('0') <<  hex << address;
                    lis << setw(8) << setfill(' ') << match;
                    sic << setw(8) << setfill(' ') << match;

                    lis << " " << setw(8) << setfill(' ') << left << opInstruction;
                    sic << " " << setw(8) << setfill(' ') << left << opInstruction;

                    objCode = curr.substr(pos,6); // 050000
		    cout << objCode <<endl;

                    if(opInstruction == "CLEAR2"){
                        lis << setw(9) << setfill(' ') << left << "X";
                        sic << setw(9) << setfill(' ') << left << "X" << endl;
                    } // CLEAR instruction

                    address += 2;
                    pos += 6;
                    lis << objCode << endl;
		   
                    
                }
                else if(opFormat == 3){
		            
	    	    lis << setw(4) <<  setfill('0') <<  hex << right << address << " ";
                    lis << setw(8) << setfill(' ') << left << match;
                    sic << setw(8) << setfill(' ') << match;
		    if(opcode == 4){
                    	lis << " " << setw(8) << setfill(' ') << left << "CLEAR";
			lis << setw(9) << setfill(' ') << left << "X";
			objCode = curr.substr(pos,6); // get object code to print
			lis << objCode <<endl;
		    }
                    sic << " " << setw(8) << setfill(' ') << left << opInstruction;
		    
                    if(immediate){
			sic << setw(9) << setfill(' ') << left << "#0" <<endl;
		    }
		    address +=3;
  		    pos+=6;
		    
                    
                  
                }
			
            } // end while loop
            
		
		} // end if for T record
		
		// check if line is a modification record
		if(line[0] == 'M'){
		
		} // end if for M record
		
		// else assume we are at the end record
		if(line[0] == 'E'){
		
		} // end else for E record
	} // end while loop
} // end method readObj()


void createOutFiles(char *argv[],ofstream &sic,ofstream &lis){
	// create a stream for our output files
    // create a .sic and .lis.txt file with our argv[1] \
    
    char buffer[20];
    strcpy(buffer,argv[1]);
    strcat(buffer, ".sic");
    sic.open(buffer);
	
    // "lis.txt" file
    strcpy(buffer,argv[1]);
    strcat(buffer,".lis.txt");
    lis.open(buffer);
} // end method createOutFiles()


// MAIN METHOD
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

