#ifndef rv32i_H
#define rv32i_H
#include <string>
#include "memory.h"


class rv32i{
    public:
    memory *mem;
    uint32_t pc = 0;
    static constexpr uint32_t XLEN = 32;
    static constexpr int mnemonic_width = 8;
    static constexpr int instruction_width = 35;
    static constexpr uint32_t opcode_lui = 0b0110111;
    static constexpr uint32_t opcode_auipc = 0b0010111;
    static constexpr uint32_t opcode_jal = 0b1101111;
    static constexpr uint32_t opcode_jalr = 0b1100111;
    static constexpr uint32_t opcode_btype = 0b1100011;
    static constexpr uint32_t opcode_rtype = 0b0110011;
    static constexpr uint32_t opcode_load_imm = 0b0000011;
    static constexpr uint32_t opcode_alu_imm = 0b0010011;
    static constexpr uint32_t opcode_stype = 0b0100011;
    static constexpr uint32_t funct3_add = 0b000;
    static constexpr uint32_t funct7_add = 0b0000000;
    static constexpr uint32_t funct7_sub = 0b0100000;
    

rv32i(memory *m);
void disasm(void);
std::string decode(uint32_t insn) const;

//Potentially use
std::string render_illegal_insn() const;
std::string render_lui(uint32_t insn) const;
std::string render_auipc(uint32_t insn) const;
std::string render_jal(uint32_t insn) const;
std::string render_jalr(uint32_t insn) const;
std::string render_btype(uint32_t insn, const char *mnemonic) const;
std::string render_itype_load(uint32_t insn, const char *mnemonic) const;
std::string render_stype(uint32_t insn, const char *mnemonic) const;
std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const;
std::string render_rtype(uint32_t insn, const char *mnemonic) const;
std::string render_fence(uint32_t insn) const;
std::string render_ecall(uint32_t insn) const;
std::string render_ebreak(uint32_t insn) const;

//Getters
static uint32_t get_opcode(uint32_t insn);
static uint32_t get_rd(uint32_t insn);
static uint32_t get_funct3(uint32_t insn);
static uint32_t get_rs1(uint32_t insn);
static uint32_t get_rs2(uint32_t insn);
static uint32_t get_funct7(uint32_t insn);
static uint32_t get_imm_i(uint32_t insn);
static uint32_t get_imm_u(uint32_t insn);
static uint32_t get_imm_b(uint32_t insn);
static uint32_t get_imm_s(uint32_t insn);
static uint32_t get_imm_j(uint32_t insn);

};






#endif