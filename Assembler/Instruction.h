class Instruction{
public:
	Instruction( string opn, string opr, short t, short f, unsigned int l){
		opname = opn; operand = opr; type = t; format = f; lineNum = l;
	}
	string getOpname(){ return opname; }
	string getOperand(){ return operand; }
	short getFormat(){ return format; }
	short getType(){ return type; }
	unsigned int getLineNumber(){ return lineNum; }
private:
	string opname;
	string operand;
	short type; // 0 is opname only <> 1 is opname, operand <> 2 is compiler direcctive <> 3 is data
	short format;
	unsigned int lineNum;
};