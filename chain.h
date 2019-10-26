// Name: Liulan Zheng

/* Class Chain - creates chains of any size, any type
			   - reads a text file that contains chains that starts with "[" and ends with a "]",  
				  for example [4: 10 30 -1 2] number before " : " is the size of the chain
			   - concatenates the chains
			   - adds an element to the end of the chain 
*/


#ifndef CSCI335_HOMEWORK1_CHAIN_
#define CSCI335_HOMEWORK1_CHAIN_

#include <iostream>
#include <cstddef>
#include <fstream>
#include <sstream>


namespace teaching_project {
/* Reads chains from a text file, concatenates chains,and modifies elements in the chain 
	using copy assignment operator, move constructor, move assignment operator, copy constructor, and destructor
   sample usage:
   Chain<string> a_chain;
   a_chain.ReadChain();
   Chain<string> e{a_chain};
   Chain<string> d = a_chain + e;
*/
template<typename Object>
class Chain {
	public:
	
/**********************big-five***************************/
	
	// Zero-parameter constructor.
	Chain():size_{0}, array_{nullptr}{}
 

	// Copy-constructor.
	Chain(const Chain &rhs):size_{rhs.size_} {
		array_ = new Object[rhs.size_];
		size_ = rhs.size_;              //copying the size_ from rhs.size_
		for(size_t x = 0; x < rhs.size_; x++) { ///copy every item in rhs arrary_ to this array_
			array_[x] = rhs.array_[x];
		}
	}

	// Copy-assignment. 
	Chain& operator=(const Chain &rhs) {
		Chain copy = rhs; 
		std::swap(*this, copy);
		return *this;
	}

	// Move-constructor. 
	Chain(Chain &&rhs): array_{rhs.array_} {
		size_ = rhs.size_;
		
		array_ = new Object[rhs.size_];          
		for(size_t x = 0; x < rhs.size_; x++) { ///copy every item in rhs arrary_ to this array_
			array_[x] = rhs.array_[x];
		}
		rhs.size_ = 0;     //erase the value for rhs.size_ after move
		rhs.array_ = nullptr; //set rhs.array_to nullptr after move
	}

	// Move-assignment.
	Chain& operator=(Chain &&rhs) {
		//swap the contents from rhs to this
		std::swap(size_, rhs.size_);
		std::swap(array_, rhs.array_); 
		//set rhs private members to default values after swapping
		rhs.size_ = 0;  
		rhs.array_ = nullptr;
		return *this;
	}

	// Destructor.
	~Chain() {
		delete []array_; //deleting array typed pointer
		array_ = nullptr;
	}

/*************** End of big-five ***********************/

	// One parameter constructor.
	Chain(const Object& item) {
		size_ = 1; 		 //only one item is initialized
		array_ = new Object{item};
	}

/*
	 @line: stores a line from a text file
	 @size: stores size based on the text file
	 @text: stores character from the text file
	 @item: stores type Object item from the text file
	 Read a chain from standard input that has a format of [size_t size: item1 item2 item3 item4]
	 	and stores in array_ and size_
	 abort() if file is not in a right format

*/
	void ReadChain() {
		std::string line;
		getline(std::cin, line); 	
		if(line.empty())
			std::cout << "Empty file" << std::endl;
		else {
			line.pop_back();   	         //delete the ']' at the end of the line 
			std::stringstream ss(line);  
			size_t size;
			Object item;
			char text;
			
			ss >> text; 	//read '[' 
			if(text == '[')	{
				ss >> size;        
				ss >> text;       //read ':'
				size_ = size;    //read the number after ':'
				array_ = new Object[size_];  //allocates and constructs size_ objects
				for(size_t i = 0; i < size_ ; i++) { //storing items in array_
					ss >> item; 
					array_[i] = item;
				}
			}
			else     // abort when a line doesn't start with a '[' 
				abort();
		}
		
	}

	// returns size_
	size_t size() const { 
		return size_; 
	}

	// @location: an index to a location in the chain.
	// @returns the Object at @location.
	// const version.
	// abort() if out-of-range.
	const Object& operator[](size_t location) const { 
		if(location < 0 || location >= size_)
			abort();
		else
			return array_[location];
	}

	// @location: an index to a location in the range.
	// @returns the Object at @location.
	// non-const version.
	// abort() if out-of-range.
	Object& operator[](size_t location) { 
		if(location < 0 || location >= size_)
			abort();
		else
			return array_[location];
	}

	//  @c1: A chain.
	//  @c2: A second chain.
	//  @return the concatenation of the two chains.
	friend Chain operator+(const Chain &c1, const Chain &c2) {
		Chain result;
		result.array_= new Object[c1.size_ + c2.size_]; //construct a new array_ with a size of c1 and c2 combined
		result.size_ = c1.size_ + c2.size_;
		for(size_t i = 0; i < c1.size(); i++) { //storing items from c1 to array_
			result.array_[i] = c1.array_[i];
		}
		for(size_t i = 0; i < c2.size(); i++) { //storing items from c2 to array_
			result.array_[c1.size_ + i] = c2.array_[i];
		}
		return result;
	
	}
	//  @c1: A string chain.
	//  @item: a string 
	//  @return the concatenation of the chain and a string typed item
	friend Chain operator+(const Chain &c1, const std::string &item) {
		Chain result;
		result.size_ = c1.size_ + 1;  //size_ increased by one, so array_ can store one more item
		result.array_ = new std::string[result.size_];
		for(size_t x = 0; x < c1.size_; x++) {//storing items from c1.array_ to array_
			result.array_[x] = c1.array_[x];
		}
		result.array_[result.size_-1] = item;
		return result;
	}
	  
	// helper function for overloading the << operator
	void Print(std::ostream &out) const {
		out << "[";                   // prints '[' at the beginning
		for(size_t i = 0; i < size(); i++){
			out << array_[i];
			if(i <size()-1)
				out<<" ";    // prints " " between items
		}
		out << "]" <<std::endl; // prints ']' at the end
	}
	
	// Overloading the << operator.
	friend std::ostream &operator<<(std::ostream &out, const Chain &a_chain) {
		a_chain.Print(out);
		return out;
	}
 
private:
	size_t size_;
	Object *array_;
};

}  // namespace teaching_project
#endif // CSCI_335_HOMEWORK1_CHAIN_
