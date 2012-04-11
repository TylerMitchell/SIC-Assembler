class ParseAssembly : public BasePass{
public:
	ParseAssembly(string file){
		fileName = file;
		stillReading = true;
		lineNumber = 0;
		locationCounter = 0;
		openFile();
		while ( stillReading ) { readLine(); } //this runs pass 1
		while( !INSTRUCTIONLIST.empty() ){
			Instruction t = INSTRUCTIONLIST.front();
			INSTRUCTIONLIST.pop();
			cout << t.getOpname() << " " << t.getOperand() << " " << t.getFormat() << " " << t.getType() << " " << t.getLineNumber() << endl;
		}
		cin.get();
		closeFile();
		PassTwo WOOT(firstInstruction, locationCounter, fileName, baseRegister); // this runs pass 2
	}
	void readLine(){
		char asmLine[256];
		if ( fileHandle.getline(asmLine, 256) ) {
			lineNumber += 1;
			parseLine(asmLine);
		}
		else{ closeFile(); stillReading = false; }
	}

	void parseLine( char line[] ){// the heart of pass 1 --- debug the location counter
		string temp(line);
		stringstream conv(temp);
		istream_iterator<string> it(conv);
		istream_iterator<string> end;
		vector<string> tokens(it, end);
		//ostream_iterator<string> oit(cout);
		//copy(tokens.begin(), tokens.end(), oit);
		short fmt = 0;
		int size = tokens.size();
		if ( size ){
			fmt = searchOPTAB(tokens[0]);
			short type = 0; // type 0 is opname only <> type 1 is label, opname <> type 2 is opname, operand <> type 3 is label, opname, operand
			if( locationCounter == 0 && lineNumber == 1){// if this iss the first line read
				if( tokens[1].compare("START") == 0){
					int l = atoi( tokens[2].c_str() );
					locationCounter = l;
					firstInstruction = l;
				}
				else{ cout << "first line MUST be a START directive"; }//----
			}
			Instruction poop( tokens[0], "FAIL", 0, fmt, lineNumber  );
			string opname = "";
			switch( size ){
				case 0: // empty line
					break;
				case 1: // this is an instruction with only an opcode feild
					INSTRUCTIONLIST.push(poop);
					break;
				case 2: // this is an instruction with no label feild or a label and an opcode
					if( isExtended(tokens[0]) ){ fmt = 4; type = 1;}
					if( fmt ){ //opname operand order
						opname = tokens[0];
						if( isExtended( tokens[0] ) ){ opname = circumsizeString( tokens[0], 1 ); }
						Instruction poop( opname, tokens[1], 1, fmt, lineNumber);
						INSTRUCTIONLIST.push(poop);
					}
					else{ // not found in OPTAB
						fmt = labelOpnameOrder(tokens[0], tokens[1], "FAIL");
					}
					break;
				case 3: // this is an instruction with all three feilds
					fmt = labelOpnameOrder(tokens[0], tokens[1], tokens[2]);
					break;
				default:
					cout << "Error in parseAssembly.parseLine";
			}	
		}
		locationCounter += fmt;
	}

	short labelOpnameOrder(string label, string opname, string operand){
		short fmt = 0;
		bool notDirective = true; bool notData = true;
		if( searchDTAB( opname ) ){// if it is an assembler directive
			if( opname.compare( "BASE" ) == 0 ){ Instruction poop( opname, operand, 2, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); }
			if( opname.compare( "END" ) == 0 ){ stillReading = false; Instruction poop( opname, operand, 2, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); notDirective = false; }
			if( opname.compare( "START" ) == 0 ){ Instruction poop( opname, label, 2, fmt, 0  ); INSTRUCTIONLIST.push(poop); notDirective = false; }
		}
		else {
			if( searchSYMTAB(label) ){ cout << "Duplicate symbol"; }
			else{
				if( searchDTAB(label) ){
					if( label.compare( "BASE" ) == 0 ){ Instruction poop( label, opname, 2, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); notDirective = false; }
					if( label.compare( "END" ) == 0 ){ stillReading = false; Instruction poop( label, opname, 2, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); notDirective = false; }
					if( label.compare( "START" ) == 0 ){ Instruction poop( label, "", 2, fmt, 0  ); INSTRUCTIONLIST.push(poop); notDirective = false; }
				}
				else {
					Symbol temp( label, locationCounter );
					SYMTAB.hashInsert(temp);
				}
				fmt = searchOPTAB(opname);
				if( isExtended( opname ) ){ fmt = 4; }
				if(!fmt){ 
					if( opname.compare( "WORD" ) == 0 ){ Instruction poop( opname, operand, 3, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); notData = false; }
					if( opname.compare( "RESW" ) == 0 ){ Instruction poop( opname, operand, 3, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); fmt += 3 * atoi( operand.c_str() ); notData = false; }
					if( opname.compare( "RESB" ) == 0 ){ Instruction poop( opname, operand, 3, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); fmt = atoi( operand.c_str() ); notData = false; }
					if( opname.compare( "BYTE" ) == 0 ){ Instruction poop( opname, operand, 3, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); notData = false; }
					if( opname.compare( "END" ) == 0 ){ stillReading = false; Instruction poop( opname, operand, 2, fmt, lineNumber  ); INSTRUCTIONLIST.push(poop); notData = false; }
					if( opname.compare( "START" ) == 0 ){ Instruction poop( opname, label, 2, fmt, 0  ); INSTRUCTIONLIST.push(poop); notData = false; }
				}
				if( notDirective && notData){
					if( operand.compare( "FAIL" ) == 0 ){// label, opname order
						if( fmt == 4 ){ opname = circumsizeString(opname, 1); }
						Instruction poop( opname, operand, 0, fmt, lineNumber  );
						INSTRUCTIONLIST.push(poop);
					}
					else{ 
						if( fmt == 0 ){ cout << "Invalid input line " << lineNumber << endl;}
						if( fmt == 4 ){ opname = circumsizeString(opname, 1); }
						Instruction poop( opname, operand, 1, fmt, lineNumber  );
						INSTRUCTIONLIST.push(poop);
					}
				}
			}
			return fmt;
		}
	}

	bool isExtended( string e){
		if( e.at(0) == '+' ){ return true; }
		else{ return false; }
	}
	
private:
	int locationCounter;
	int firstInstruction;
	int lineNumber;
	bool stillReading;
};
