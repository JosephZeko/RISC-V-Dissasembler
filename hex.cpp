#include "hex.h"
#include <string>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <sstream>

/*
* Function Name: hex8
* Takes a uint8_i representing the hex value being passed in
* Use: Formats the value being bassed in 
*/
std::string hex8(uint8_t i){
std::ostringstream os;
 os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
 return os.str();
}

/*
* Function Name: hex32
* Takes a uint8_i representing the hex value being passed in
* Use: Formats the value being bassed in 
*/
std::string hex32(uint32_t i){
std::ostringstream os;
 os << std::hex << std::setfill('0') << std::setw(8) << i;
 return os.str();
}

/*
* Function Name: hex0x32
* Takes a uint8_i representing the hex value being passed in
* Use: Formats the value being bassed in 
*/
std::string hex0x32(uint32_t i){
std::ostringstream os;
 os << "0x" << std::hex << std::setfill('0') << std::setw(8) << i;
 return os.str();
}