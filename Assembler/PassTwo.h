class BasePass{
public:
	void openFile(){
		fileHandle.open(fileName.c_str());
		if( fileHandle.fail() ){ cout << "\nFile failed to open"; cin.get(); exit(0);} // error if file failed to open
	}

	void closeFile(){
		fileHandle.close();
	}

	string circumsizeString(string opname, int numcut){
		opname = opname.substr( numcut, (opname.size()-numcut) );
		return opname;
	}

	bool searchDTAB( string &lookfor){// returns true if found and false if not
		Directive temp = DTAB.hashSearch(lookfor);
		if( (temp.getDirective()).compare("FAIL") != 0 ){ return 1; }
		else { return 0; }
	}
	bool searchSYMTAB(string &lookfor){
		Symbol temp = SYMTAB.hashSearch(lookfor);
		if( (temp.getLabel()).compare("FAIL") != 0 ){ return 1; }
		else { return 0; }
	}

	int searchOPTAB(string &lookfor){
		InstructionSet temp = OPTAB.hashSearch(lookfor);
		if( (temp.getOpname()).compare("FAIL") != 0 ){ return temp.getFormat(); }
		else { return 0; }
	}
	fstream fileHandle;
	string fileName;
	unsigned int baseRegister;
	unsigned int firstInstruction;
};

class PassTwo : public BasePass{
public:
	PassTwo( unsigned int fi, unsigned int lc, string file, unsigned int br){
		firstInstruction = fi;
		baseRegister = br;
		programCounter = 0;
		locationCounter = lc;
		startOfTextRecord = 0;
		textRecordSize = 0;
		fileName = objectFileName(file);
		openFile();
		while( !INSTRUCTIONLIST.empty() ){
			Instruction temp = INSTRUCTIONLIST.front();
			INSTRUCTIONLIST.pop();
			string assembled;
			switch( temp.getType() ){
				case 0:// opname only
					programCounter += temp.getFormat();
					assembleInstruction( temp.getOpname(), temp.getFormat() );
					break;
				case 1://opname, operand
					programCounter += temp.getFormat();
					assembleInstruction( temp.getOpname(), temp.getOperand(), temp.getFormat() );
					break;
				case 2://assembler directive
					if( temp.getOpname().compare("END") == 0){
						writeEndRecord();
					}
					if( temp.getOpname().compare("START") == 0){
						writeHeaderRecord( temp );
					}
					if( temp.getOpname().compare("BASE") == 0){
						setBaseRegister( temp.getOperand() );
					}
					break;
				case 3:// data
					if( temp.getOpname().compare("RESW") == 0 ){ programCounter += 3 * atoi( temp.getOperand().c_str() );  writeTextRecord(); }
					if( temp.getOpname().compare("RESB") == 0 ){ programCounter += atoi( temp.getOperand().c_str() );  writeTextRecord(); }
					assembled = parseData( temp.getOperand() );
					checkTextRecord( assembled, assembled.size()/2 );
					break;
				default:
					cout << "OH NOES!!! Something went wrong... (During PassTwo)";
					break;
			}
		}
		closeFile();
	}
	
	string objectFileName( string fn ){// change extension to .o
		int l = fn.find(".");
		if( l != string::npos ){
			fn = fn.substr(0, l);
			fn.append(".o");
		}
		return fn;
	}
	
	void setBaseRegister(string &value){
		Symbol temp = SYMTAB.hashSearch(value);
		if( temp.getLabel().compare("FAIL") == 0 ){
			cout << "Programmer attempted to load an invalid base register...NOOB!";
		}
		baseRegister = temp.getLocation();
	}

	void writeEndRecord(){//6
		int i = firstInstruction;
		stringstream conv;
		string t;
		conv << hex << i;
		conv >>t;
		t = padString( t, 6, "0", FRONT);
		writeTextRecord();
		fileHandle << "E" << t << endl;
	}

	string padString( string t, int padToThis, string useThis, bool isBack ){
		if( isBack){
			for( int cnt = t.size(); cnt < padToThis; cnt++ ){
				t.append(useThis);
			}
			return t;
		}
		else{
			for( int cnt = t.size(); cnt < padToThis; cnt++ ){
				t.insert(0, useThis);
			}
			return t;
		}
	}

	void writeHeaderRecord( Instruction &woot){
		string prgName = woot.getOperand();
		prgName = padString( prgName, 6, " ", BACK);
		stringstream conv, conv2;
		string t;
		string l;
		conv << hex << firstInstruction;
		conv >>t;
		t = padString( t, 6, "0", FRONT);
		conv << hex << (locationCounter - firstInstruction);
		conv >> l;
		l = padString( l, 6, "0", FRONT);
		fileHandle << "H" << prgName << t << l << endl;
	}

	void writeTextRecord(){
		string temp;
		string play;
		stringstream conv;
		stringstream conv2;
		stringstream conv3;
		conv << "T";
		conv2 << hex << startOfTextRecord;
		conv2 >> play;
		play = padString( play, 6, "0", FRONT);
		conv << play;
		conv3 << hex << textRecordSize;
		play.erase();
		conv3 >> play;
		play = padString( play, 2, "0", FRONT);
		conv << play;
		conv >> temp;
		textRecordSize = 0;
		fileHandle << temp << textRecord << endl;
		textRecord.erase();
		startOfTextRecord = programCounter;
	}
	
	void assembleInstruction( string opname, string operand, short format ){ // the heart of instruction assembly
		//figure out nixbpe e is known, it is 1 if format = 4
		bool n,i,x,b,p,e;
		x = false; b = false; p = false;
		if( format == 1 ){ assembleInstruction( opname, format ); }
		if( format == 2 ){
			unsigned int hexc = OPTAB.hashSearch( opname ).getHexcode();
			if( operand.find(",") != string::npos ){ // there are 2 registers listed
				int pos = operand.find_first_not_of( " ,\t");
				string temp, temp2;
				temp.push_back( operand[pos] );
				operand.replace( pos, 1, " ");
				int loc = SYMTAB.hashSearch( temp ).getLocation();
				temp = intToHex(loc);
				pos = operand.find_first_not_of( " ,\t");
				temp2.push_back( operand[pos] );
				loc = SYMTAB.hashSearch( temp2 ).getLocation();
				temp2 = intToHex(loc);
				string final = intToHex(hexc);
				final.append(temp);
				final.append(temp2);
				checkTextRecord( final, format );
			}
			else{ // there is only 1 register listed
				int pos = operand.find_first_not_of( " \t");
				string temp;
				temp.push_back( operand[pos] );
				int loc = SYMTAB.hashSearch( temp ).getLocation();
				temp = intToHex(loc);
				temp.push_back('0');
				string final = intToHex(hexc);
				final.append(temp);
				checkTextRecord( final, format );
			}
		}
		if( format == 3 || format == 4 ){
			unsigned int hexc = OPTAB.hashSearch( opname ).getHexcode();
			if( format == 4 ){ e = true; }
			else{ e = false; }
			short test = immediateOrIndirect( operand );// returns 0 if immediate, 1 if indirect, 2 if simple
			switch( test ){ // set the n and i bits
				case 0: //immediate
					n = false; i = true;
					operand = circumsizeString( operand, 1 );
					break;
				case 1: //indirect
					n = true; i = false;
					operand = circumsizeString( operand, 1 );
					break;
				case 2:
					n = true; i = true;
					break;
				default:
					cout << "This should never happen... WHAT HAVE YOU DONE TO ME STUPID PROGRAMMER!!!";
					break;
			}
			x = isIndexed(operand); // figure out if x bit is set
			if( x ){ operand = operand.substr(0, (operand.size()-2) ); } // cut the (,X) off of the instruction
			unsigned int addr = SYMTAB.hashSearch(operand).getLocation();// time to decide if it will be pc or base relative
			int disp = addr - programCounter;
			if( !(!n && i) ){
				if( (disp >> 12) && !e){//if disp can't fit
					disp = addr - baseRegister;
					if( disp > 4095 || disp < -4096){// if disp can't fit
						cout << "You need to specify extended format with a '+' symbol" << endl;
					}
					else{//it will be BASE relative
						b = true;
					}
				}
				else{ //it will be PC relative
					if( !e ){ p = true; }
				}
			}
			else{ //immediate addressing
				disp = atoi( operand.c_str() ) ;
			}
			if( e ){ //the instruction is ready to be assembled
				int ni = (n << 1) + i;
				int xbpe = (x << 3) + (b << 2) + (p << 1) + e;
				hexc += ni;
				string final = intToHex(hexc);
				final.append( intToHex(xbpe) );
				final.append( padString( intToHex( SYMTAB.hashSearch( operand ).getLocation() ), 5, "0", FRONT ) );//hahahahahahahahaha
				checkTextRecord( final, format );
			}
			else{
				int ni = (n << 1) + i;
				int xbpe = (x << 3) + (b << 2) + (p << 1) + e;
				hexc += ni;
				string final = padString( intToHex(hexc), 2, "0", FRONT );
				final.append( intToHex(xbpe) );
				final.append( padString( intToHex(disp), 3, "0", FRONT ) );
				checkTextRecord( final, format );
			}
		}
	}
	
	string intToHex( int num){
		string temp;
		stringstream conv;
		conv << hex << num;
		conv >> temp;
		return temp;
	}

	bool isIndexed( string operand ){
		int loc = operand.rfind( ",X" );
		if( loc != string.npos ){ return true; }
		else{ return false; }
	}
	short immediateOrIndirect( string operand){
		short settings = 2;
		if( operand.at(0) == '#' ){ settings = 0; }
		if( operand.at(0) == '@' ){ settings = 1; }
		return settings;
	}

	void assembleInstruction( string opname, short format){
		unsigned int opcode = (OPTAB.hashSearch(opname)).getHexcode();
		// if there is no operand then we will always use simple addressing n=1, i=1
		opcode += 3;
		stringstream conv;
		conv << hex << opcode;
		string op;
		conv >> op;
		op = padString(op, (format*2), "0", BACK);
		checkTextRecord( op, format );
	}
	
	void checkTextRecord( string op, short fmt){
		if( (textRecordSize + fmt) < 31){
			textRecordSize += fmt;
			textRecord.append(op);
		}
		else{ // text record is full and needs to be written
			writeTextRecord();
			textRecord.append(op);
			textRecordSize += fmt;
		}
	}
	string parseData(string operand){
		if( operand.find("C'") != string::npos ){ // data needs to be converted from ascii
			string poop;
			stringstream conv;
			operand = circumsizeString(operand, 2);
			for( string::iterator c = operand.begin(); *c != '\''; c++){
				int i = *c;
				conv << hex << i;
			}
			conv >> poop;
			return poop;
		}
		else if( operand.find("X'") != string::npos ){ // data is already in hex format
			operand = circumsizeString(operand, 2);
			operand = operand.substr(0, operand.size()-1);
			return operand;
		}
		else{ //data is in decimal form and needs to be converted into hex -------------- yet to be tested
			int t = atoi( operand.c_str() );
			string poop;
			stringstream conv;
			conv << t;
			conv >> hex >> poop;
			cout << poop << " ";
			return poop;
		}
	}

private:
	unsigned int locationCounter;
	unsigned int programCounter;
	string textRecord; // contains the hex chars that will be appended to the string containing cols 1-9
	short textRecordSize;
	unsigned int startOfTextRecord;
};