#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <cmath>
#include <cstdlib>
#include "Conversions.h"
#include "hashEntry.h"
#include "hashTable.h"
#include "Instruction.h"
#include "globalTables.h"
#include "PassTwo.h"
#include "Parser.h"


void readInInstructionSet(string inst){
	ifstream instructionSet;
	instructionSet.open( inst.c_str() );
	vector<InstructionSet> instructions;
	string op;
	short fmt;
	string hex;
	while( instructionSet >> op >> fmt >> hex){
		int h = hexToInt(hex);
		InstructionSet temp(op,fmt,h);
		OPTAB.hashInsert(temp);
	}
	instructionSet.close();

}

void loadAssemblerDirectives(){
	Directive d1("BASE");
	Directive d2("EQU");
	Directive d4("START");
	Directive d3("END");
	DTAB.hashInsert(d1);
	DTAB.hashInsert(d2);
	DTAB.hashInsert(d3);
	DTAB.hashInsert(d4);
}

void loadSymbols(){
	int size = 8;
	string symbol[8] = {"A", "X", "L", "B", "S", "T", "F"};
	int value[8] = { 0, 1, 2, 3, 4, 5, 6, };
	for(int i=0; i < size; i++){
		Symbol temp( symbol[i], value[i] );
		SYMTAB.hashInsert(temp);
	}
}

int main(){
	readInInstructionSet("InstructionSet.txt");
	loadAssemblerDirectives();
	loadSymbols();
	ParseAssembly test1( "test.asm" );
	while( !INSTRUCTIONLIST.empty() ){
		Instruction t = INSTRUCTIONLIST.front();
		INSTRUCTIONLIST.pop();
		cout << t.getOpname() << " " << t.getOperand() << endl;
	}
	cin.get();
	return 0;
}