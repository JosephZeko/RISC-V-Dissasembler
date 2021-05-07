//***************************************************************************
//
//  main.cpp
//  Created by Joseph Zeko
//
//***************************************************************************/
#include "memory.h"
#include "hex.h"
#include "rv32i.h"
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void usage()
 {
 std::cerr << " Usage : rv32i [ - m hex - mem - size ] infile " << endl;
 std::cerr << " -m specify memory size ( default = 0 x10000 ) " << endl;
 exit(1);
 }

 /**************************************************************
 * Read a file of RV32I instructions and execute them .
 ********************************************************************/
 int main( int argc, char** argv)
 {
 uint32_t memory_limit = 0x10000 ; // default memory size = 64 k
int opt;

 while ((opt = getopt(argc,argv,"m :" )) != -1)
 {
 switch (opt)
 {
 case 'm':
 memory_limit = std::stoul(optarg ,nullptr,16) ;
 break;
 default: /* ’? ’ */
 usage();
 }
 }

 if ( optind >= argc )
 usage(); // missing filename

 memory mem( memory_limit ) ;

 if (!mem.load_file ( argv [optind]) )
 usage();

 rv32i sim(&mem ) ;
 sim.disasm();
 mem.dump();

 return 0;
 }