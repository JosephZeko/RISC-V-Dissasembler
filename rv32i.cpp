#include "rv32i.h"
#include "hex.h"
#include "memory.h"
#include <string>
#include <iostream>
#include <cassert>
#include <sstream>
using namespace std;

rv32i::rv32i(memory *m){
    mem = m;
}

void rv32i::disasm(void){
   for(pc =0; pc<mem->get_size();){	//loops through memory
		std::cout << hex32(pc)<< ": " << endl;
		pc +=4 ;
	}
   
}

std::string rv32i::decode(uint32_t insn) const{
uint32_t opcode = get_opcode(insn);
uint32_t funct3 = get_funct3(insn);
uint32_t funct7 = get_funct7(insn);
int32_t imm_i = get_imm_i(insn);

    switch(opcode)
    {
    default:                    return render_illegal_insn();
    case opcode_lui:            return render_lui(insn);
    case opcode_auipc:          return render_auipc(insn);
    //R TYPE
    case opcode_rtype:
        switch(funct3)
        {
        default:                return render_illegal_insn();
        case funct3_add:
            switch(funct7)
            {
            default:            return render_illegal_insn();
            case funct7_add:    return render_rtype(insn, "add");
            case funct7_sub:    return render_rtype(insn, "sub");
            }
            assert(0 && "unhandled funct7");
        case 0b001:             return render_rtype(insn, "sll"); //RTYPES SLL SLT SLTU XOR SRL SRA OR AND
        case 0b010:             return render_rtype(insn, "slt");                         
        case 0b011:             return render_rtype(insn, "sltu");
        case 0b100:             return render_rtype(insn, "xor");
        case 0b101:
            switch(funct7){
                default:        return render_illegal_insn();
                case 0b0000000: return render_rtype(insn, "srl");
                case 0b0100000: return render_rtype(insn, "sra");
            }
        case 0b110:             return render_rtype(insn, "or");
        case 0b111:             return render_rtype(insn, "and");
        }
        assert(0 && "unhandled funct3");
        // I TYPE
    case opcode_alu_imm:
        switch (funct3)
        {
        default:                return render_illegal_insn();
        case 0b001:             return render_itype_alu(insn, "slli", imm_i%XLEN);
        case 0b000:            return render_itype_alu(insn, "addi", imm_i);
        case 0b010:            return render_itype_alu(insn, "slti", imm_i);
        case 0b011:            return render_itype_alu(insn, "sltiu", imm_i);
        case 0b100:            return render_itype_alu(insn, "xori", imm_i);
        case 0b110:            return render_itype_alu(insn, "ori", imm_i);
        case 0b111:            return render_itype_alu(insn, "andi", imm_i);
        case 0b101:
            switch(funct7)
            {
            default:           return render_illegal_insn();
            case 0b0100000:    return render_itype_alu(insn, "srai", imm_i%XLEN);
            case 0b0000000:    return render_itype_alu(insn, "srli", imm_i%XLEN);
            }
            assert(0 && "unhandled funct7");
        }
            assert(0 && "unhandled funct3");
        //I TYPE
    case opcode_load_imm:
        switch(funct3){
            default:               return render_illegal_insn();   
            case 0b000:            return render_itype_load(insn, "lb");
            case 0b001:            return render_itype_load(insn, "lh");
            case 0b010:            return render_itype_load(insn, "lw");
            case 0b100:            return render_itype_load(insn, "lbu");
            case 0b101:            return render_itype_load(insn, "lhu");
        }
        //B TYPE
    case opcode_btype:
        switch(funct3){
            default:               return render_illegal_insn();
            case 0b000:            return render_btype(insn, "beq");
            case 0b001:            return render_btype(insn, "bne");
            case 0b100:            return render_btype(insn, "blt");
            case 0b101:            return render_btype(insn, "bge");
            case 0b110:            return render_btype(insn, "bltu");
            case 0b111:            return render_btype(insn, "bgeu");
        }
    case opcode_stype:
        switch(funct3){
            default:               return render_illegal_insn();
            case 0b000:            return render_stype(insn, "sb");
            case 0b001:            return render_stype(insn, "sh");
            case 0b010:            return render_stype(insn, "sw");
        }
    }
    assert(0 && "unhandled opcode");
}
}





 uint32_t rv32i::get_opcode(uint32_t insn){
    return (insn &0x0000007f);
}

 uint32_t rv32i::get_rd(uint32_t insn){
    return (insn &0x00000f80) >> (7-0); 
}

 uint32_t rv32i::get_funct3(uint32_t insn){
    return (insn &00007000) >> (12);
}

 uint32_t rv32i::get_rs1(uint32_t insn){
    return (insn &0x000f8000) >> (15);
}

 uint32_t rv32i::get_rs2(uint32_t insn){
    return(insn &0x01f00000) >> (20);
}

uint32_t rv32i::get_funct7(uint32_t insn){
    return (insn &0xfe000000) >> (25);
}

uint32_t rv32i::get_imm_i(uint32_t insn){
    int32_t imm_i = (insn &0xfff00000) >> (20);
    if(insn &0x80000000)
        imm_i |= 0xfffff000;

    return imm_i;
}

uint32_t rv32i::get_imm_u(uint32_t insn){
    return insn;
}

uint32_t rv32i::get_imm_b(uint32_t insn){
    int32_t imm_b = (insn &0x00000080) << (4);
    imm_b |= (insn &0x00000f00) >> (7);
    imm_b |= (insn &0x7e000000) >> (20);
    imm_b |= (insn &0x80000000) >> (19);
    if(insn & 0x80000000)
        imm_b |= 0xffffe000;

    return imm_b;
}

uint32_t rv32i::get_imm_s(uint32_t insn)
{
    int32_t imm_s = (insn & 0xfe000000) >> (25-5);	// extract & shift bits 5-11
    imm_s |= (insn & 0x00000f80) >> (7-0);			// extract & shift bits 0-4
    if (insn & 0x80000000)							// sign-extend
        imm_s |= 0xfffff000;

    return imm_s;
}

uint32_t rv32i::get_imm_j(uint32_t insn){
    int32_t imm_j = (insn &0x000ff000);
    imm_j |= (insn &0x8000000) >> (11);
    imm_j |= (insn &0x00100000) >> (9);
    imm_j |= (insn &0x7fe00000) >> (20);
    if (insn & 0x80000000)							// sign-extend
        imm_j |= 0xfffff000;
    return imm_j;
}

std::string rv32i::render_illegal_insn() const{
  std::string temp = "Error";
 return temp;
}


std::string rv32i::render_lui(uint32_t insn) const{
   std::ostringstream os;
   os << " lui    x" << std::dec << get_rd(insn) << ".0x" << std::hex << ((get_imm_u(insn) >> 12) &0x000fffff);
   return os.str();
}

 std::string rv32i::render_auipc(uint32_t insn) const{
    std::ostringstream os;
    os << " auipc  x" << std::dec << get_rd(insn) << ",0x" << std::hex << ((get_imm_u(insn) >> 12) &0x000fffff);
    return os.str(); 
}

 std::string rv32i::render_jal(uint32_t insn) const{
  std::ostringstream os;
  os << " jal    x" << std::dec << get_rd(insn) << ",0x" << std::hex << (get_imm_j(insn) + pc);
  return os.str();
}

 std::string rv32i::render_jalr(uint32_t insn) const{
   std::ostringstream os;
   os << " jalr   x" << std::dec << get_rd(insn) << get_imm_i(insn) << "(x" << get_rs1(insn) << ")";
   return os.str();
}


std::string rv32i::render_btype(uint32_t insn, const char *mnemonic) const{ 
    std::ostringstream os;
    os << *mnemonic << "  x" << std::dec << get_rs1(insn) << ",x" << get_rs2(insn) << "," << "0x" << std::hex << (get_imm_b(insn) + pc);
      return os.str();

}
std::string rv32i::render_itype_load(uint32_t insn, const char *mnemonic) const{
    std::ostringstream os;
    os << *mnemonic << "  x" << std::dec << get_rd(insn) << "," << get_imm_i(insn) << "(x" << get_rs1(insn) << "(";
      return os.str();

} 

std::string rv32i::render_stype(uint32_t insn, const char *mnemonic) const{
    std::ostringstream os;
    os << *mnemonic << "  x" << get_rs2(insn) << "," << get_imm_s(insn) << "(x" << get_rs1(insn) << ")";
      return os.str();
}

std::string rv32i::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const{
    std::ostringstream os;
    os << *mnemonic << "  x" <<  std::dec << get_rd(insn) << ",x" << get_rs1(insn) << "," << imm_i;
      return os.str();
}

std::string rv32i::render_rtype(uint32_t insn, const char *mnemonic) const{
    std::ostringstream os;
    os << *mnemonic << "  x" << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << ",x" << get_rs2(insn);
      return os.str();
} 

