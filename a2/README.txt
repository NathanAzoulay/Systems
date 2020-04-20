Members: 
	Nathan Azoulay, cssc1915 
	Angelo Cabading, cssc1952 
	Destyni Ta, cssc1953 
	Michael Hoang, cssc1954 
Class Information: 
	CS530, Spring 2020
Assignment Information: 
	Assignment #2, XE Disassembler 
Filename: 
	README.txt
File manifest:
	-dxe.cpp
	-dxe.h
	-makefile
	-sample.sym
	-sample.obj
	-sample.lis.txt
	-sample.sic
	-dxeTest.cpp (used to output data structures and test functions)
Compile Instructions: 
	within ~/a2: 
	make dxe
	dxe <filename>
To Clean: 
	make clean
Operating instructions: 
	./dxe sample.obj
Design Decisions: 
	
	Check for file input errors
    	Display error message if incorrect -> break out
	Open file
	Initialize our data to read in symbol file
	Read symbol file, store as vector (might change)
	Read object file, create output files with code
	Check over symbol file addresses
    	Write RESW or RESB if a symbol address is not listed

Extra Features: 
	1. Error checked to see if the files were correct or existent within the directory
Known Deficiencies/Bugs: 
	1. We had troubles implementing flag bits, LTORG, BASE, and instructional overload - thus the program may not break in all required parts
	2. Did not complete modification or end record since LTORG, base, and overload was not implemented
Lessons Learned: 
	1. We all gained a better understanding of how the XE disassembler functions and disassembles the object file using the symbol table 
	2. We had to learn how to create algorithms that manipulated the object file and the symbol file to create the source file
	3. We were able to tamper with various files in order to create the desired output in a different language, other than Java
	4. It was difficult to work with multiple coders because we all have different styles, but we were able to understand each other and create something to the best of our abilities
	5. Learned to store symbol table in vectors using C++ functions that I never previously used along with output formats using cout (fully tested readSym function)
	