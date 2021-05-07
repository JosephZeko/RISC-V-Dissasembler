#include "memory.h"
#include "hex.h"
#include <sstream>
#include <iostream>
#include <cstring>
#include <ctype.h>
using namespace std;

/*
* Function Name: memory
* 
* Use: defualt constructor
*/
memory::memory(uint32_t siz){
    siz = (siz+15)&0xfffffff0;  //round the length up, mod-16
    size = siz;
    mem = new uint8_t[size];
    for(unsigned int i = 0; i < siz; i++){
        mem[i] = 0xa5;                        //fils the array with a5 by defualt
       
    }
}

/*
* Function Name: ~memory
* 
* Use: deallocator
*/
memory::~memory(){
    delete[] mem;
}

/*
* Function Name: check_address
* takes a uint32_t i that reperesents the address to be checked
* Use: checks to see if the address could fit in the array
*/
 bool memory::check_address(uint32_t i) const{
    if(i < get_size()){
        return true;
    }
    else{
        std::cout << "WARNING: Address out of range: " << hex0x32(i) << std::endl;
        return false;
    }
}

/*
* Function Name: get_size
*  Takes nothing
* Use: retursn the size of the array
*/
uint32_t memory::get_size() const{
 return size;
}

/*
* Function Name: get8
*  takes uint32_t addr that reperesents the address to recieve that value from
* Use: returns a value at the passed in address
*/
uint8_t memory::get8(uint32_t addr) const{
if(check_address(addr)){
    return mem[addr];
}
else{
    return 0;
}
}

/*
* Function Name: get16
* takes a uint32_t addr that represents the address to recieve that value from
* Use: Calls get8 twice and then compbins them to return
*/
uint16_t memory::get16(uint32_t addr) const {
  return get8(addr+1) << 8  | get8(addr);
}

/*
* Function Name: get 32
*  takes uint32_t addr that reperesents the address to recieve a value from
* Use: calls get16 twice then returns the combined two numbers
*/
uint32_t memory::get32(uint32_t addr) const{
    return get16(addr+1) << 16 | get16(addr);
}

/*
* Function Name: set8
* takes addr and val. That reperesetns the address to set val too.
* Use: sets the address of addr to val in the array
*/
void memory::set8(uint32_t addr, uint8_t val){
    if(check_address(addr)){
        mem[addr] = val;
    }
}

/*
* Function Name: set16
*  same as set8
* Use: calls set8 twice to store a 16 number
*/
void memory::set16(uint32_t addr, uint16_t val){
    set8((addr+1)  , val) ;
    set8(addr,val);
}

/*
* Function Name: set32
* same as set8
* Use: calls set16 twice to store a 32 number
*/
void memory::set32(uint32_t addr, uint32_t val){
  set16((addr+1)   , val) ;
    set16(addr,val);
}

/*
* Function Name: dump
* Takes no passes
* Use: dumps the memory array and its corrisponding ascii
*/
void memory::dump() const{
uint8_t counter = 0x0;              //set counter to zero for formating
cout << hex32(counter)<<": ";       //print the first counter in hex32
char ascii[ 16 ] = {};              //create the ascii array

 for( uint32_t i = 0; i < size; i++ ) {         //loop as big as the mem array is
    uint8_t ch = get8(i);                       
    ascii[i%16] = std::isprint(ch) ? ch : '.'; //if printable character: put it in the array, if  not: put a period
    
    if ((i%16 == 0)&&(i!=0)) {                //if the loop is at 16 but not at zero
    counter += 0x10;                         //add 0x10 to the counter
    cout << "*" << ascii << "*" << endl;    //print out the ascii line
    cout << hex32(counter) <<": ";          //print out hte next counter
    }
    cout << hex8(mem[i]) << " ";           //print out the next hex value in the mem array
  }
  cout << "*" << ascii << "*" <<  endl;   //print out the final ascii line
}

/*
* Function Name: load_file
* takes in a string &fname which is the file name to read from
* Use: reads in a file containging text
*/
bool memory::load_file(const std::string &fname){
    std::ifstream infile(fname, ios::in|ios::binary);  //open the file in binary

    if(!infile.is_open()){                           //if the file did not open
         std::cout << "ERROR: Cant open file";              //print out error
         return false;                                     //end file loading
    }
    int i = 0;                        //start counter
    char ch;                          //character to  be read into
    while(infile >> std::noskipws >> ch){ //ready infile one char at a time not skipping white spaces
        mem[i] = ch;                      //set the place in the array to the char
        i++;                              //iterate through
    }
    return true;
}