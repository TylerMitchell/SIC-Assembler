using namespace std;

unsigned int hexToInt(string hex){//32bit only
	int multiplier = hex.size();
	unsigned int value = 0;
	unsigned int powersOfSixteen[8] = { 268435456,16777216,1048576,65536,4096,256,16,1};
	short int i = 7;
	for(int c = 0; c < multiplier; c++){
		switch( hex[ (multiplier-1) - c]){
			case '0':
				break;
			case '1':
				value += powersOfSixteen[i-c];
				break;
			case '2':
				value += 2 * powersOfSixteen[i-c];
				break;
			case '3':
				value += 3 * powersOfSixteen[i-c];
				break;
			case '4':
				value += 4 * powersOfSixteen[i-c];
				break;
			case '5':
				value += 5 * powersOfSixteen[i-c];
				break;
			case '6':
				value += 6 * powersOfSixteen[i-c];
				break;
			case '7':
				value += 7 * powersOfSixteen[i-c];
				break;
			case '8':
				value += 8 * powersOfSixteen[i-c];
				break;
			case '9':
				value += 9 * powersOfSixteen[i-c];
				break;
			case 'A':
				value += 10 * powersOfSixteen[i-c];
				break;
			case 'B':
				value += 11 * powersOfSixteen[i-c];
				break;
			case 'C':
				value += 12 * powersOfSixteen[i-c];
				break;
			case 'D':
				value += 13 * powersOfSixteen[i-c];
				break;
			case 'E':
				value += 14 * powersOfSixteen[i-c];
				break;
			case 'F':
				value += 15 * powersOfSixteen[i-c];
				break;
			default:
				cout << "Error in hexToInt";
		}
	}
	return value;
}