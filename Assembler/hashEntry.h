class InstructionSet{
public:
	string the_key;
	InstructionSet(string o, short f, int h){
		opname = o; format = f; hexcode = h; the_key = o;
	}

	InstructionSet(){
		opname = "FAIL";
	}
	string getOpname(){ return opname; }
	short getFormat(){ return format; }
	unsigned int getHexcode(){ return hexcode; }

private:
	string opname;
	short format;
	unsigned int	hexcode;
};

bool operator ==(const InstructionSet &x, const InstructionSet &y){
	if( x.the_key.compare(y.the_key) == 0 ){ return 1; }
	else { return 0; }
}

class Symbol{
public:
	string label;
	int location;
	string the_key;
	Symbol(string lab, int loc){
		label = lab; location = loc; the_key = lab;
	}

	Symbol (){
		the_key = "FAIL"; label = "FAIL";
	}
	string getLabel(){ return label; }
	int getLocation(){ return location; }

};

bool operator ==(const Symbol &x, const Symbol &y){
	if( x.the_key.compare(y.the_key) == 0 ){ return 1; }
	else { return 0; }
}

class Directive{
public:
	string directive;
	string the_key;
	Directive(string k){
		the_key = k; directive = k;
	}
	Directive(){
		the_key = "FAIL"; directive = "FAIL";
	}
	string getDirective(){
		return directive;
	}

private:

};

bool operator ==(const Directive &x, const Directive &y){
	if( x.the_key.compare(y.the_key) == 0 ){ return 1; }
	else { return 0; }
}

class Key{
public:
	string the_key;
	Key(string k){
		the_key = k;
	}
private:

};

bool operator ==(const Key &x, const Key &y){
	if( x.the_key.compare(y.the_key) == 0 ){ return 1; }
	else { return 0; }
}