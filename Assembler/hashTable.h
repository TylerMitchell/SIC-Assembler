template <class K>
class HashTable {
public:
	HashTable (int size); // constructor: size is the size of your hash table
	//Post: Your hash table is ready to have data inserted into it
	~HashTable();
	void hashInsert( K data);
	//Takes data and inserts it into the hash table if the data is unique
	int hashFunction( int value);
	//Takes the value of a piece of data and returns the index where the data should be found or stored
	int hashFunction(string value);
	K hashSearch(K & data);
	//Takes data and searches the hashtable for that data. If the data is found, A copy of that data will be returned.
	K hashSearch(string data);
private:
	list<K> * tableptr; // pointer to hashtable
	int tablesize; // table size
};

template <class T>
HashTable<T>::HashTable(int size){ //allocate an array of lists of size indexes
	tablesize = size; 
	list<T> * temp = new list<T>[size];
	tableptr = temp; // tableptr is a pointer to the head of an array of type list<T>
}

template <class T>
void HashTable<T>::hashInsert( T data){//insert a data item into the hashtable if it is not already present
	int index = hashFunction(data.the_key); // find the index on the hashtable at which the data should be stored-------------------------
	list<T> * findIndex = tableptr; // get a temporary pointer to the head of the hash table
	for(int i = 0;i<index;i++){ // make findIndex a pointer to the list where the data item should be inserted
		findIndex++;
	}
	bool duplicate = false; // a switch to be flipped if the value is already present in the list
	for( list<T>::iterator it = findIndex->begin(); it != findIndex->end(); it++){ //search the list findIndex for the value you want to insert. If it already exists, set duplicate = true.
		if (*it == data ){duplicate = true;}
	}
	if(!duplicate){findIndex->push_front(data);} // if the data item is not already present then, insert it into the list.
}

template <class T>
int HashTable<T>::hashFunction(int value){//takes the value of the data to be hashed and returns an index to the hash table where the data should be placed/found
	stringstream ss, stream; 
    ss << value;
    string number = "";
	int cnt = ss.str().length();
	while ( cnt < 4){ // if the string has less than 4 characters, insert 0's at the front of it untill it has 4
		number.insert(0, "0");
		cnt++;
	}
	number.append( ss.str() );
	string flip = number.substr(2,2); // swap the last 2 characters with the first 2 characters
	flip.append(number.substr(0,2));
	stream << flip; //convert string to number
	int hashVal;
	stream >> hashVal;
	int index = hashVal % tablesize; // calculate hash table index
	return index;
}

template <class T>
int HashTable<T>::hashFunction(string value){//takes the value of the data to be hashed and returns an index to the hash table where the data should be placed/found
	unsigned int index = 0;
	for(unsigned int i = 0; i < value.size(); i++){
		index += (i+1)*value.at(i);
	}
	return index%tablesize;
}

template <class T>
T HashTable<T>::hashSearch(T & data){
	int index = hashFunction(data.the_key);// find the index where data should be located annd return it
	list<T> * findIndex = tableptr;
	for(int i = 0;i<index;i++){ //find the list corresponding to the target index, findIndex is a pointer to that list
		findIndex++;
	}
	for( list<T>::iterator it = findIndex->begin(); it != findIndex->end(); it++){ //search the list findIndex for the target value. If the value is found, return that data item.
		if (*it == data){return *it;}
	}
	T * stupid = new T;
	return *stupid;
}

template <class T>
T HashTable<T>::hashSearch(string data){
	int index = hashFunction(data);// find the index where data should be located annd return it
	list<T> * findIndex = tableptr;
	for(int i = 0;i<index;i++){ //find the list corresponding to the target index, findIndex is a pointer to that list
		findIndex++;
	}
	for( list<T>::iterator it = findIndex->begin(); it != findIndex->end(); it++){ //search the list findIndex for the target value. If the value is found, return that data item.
		if (it->the_key == data){return *it;}
	}
	T * stupid = new T;
	return *stupid;
}

template <class T>
HashTable<T>::~HashTable(){ //free dynamic array memory
	delete [] tableptr;
}