//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include <iostream>
#include <vector>
#include <Zydis/Zydis.h>
#include "../code_analysis/function.hpp"

enum AsmDirective
{
  ASM_DIRECTIVE_DB,   //define byte
  ASM_DIRECTIVE_INST  //an instruction is here
};
struct AsmLine
{
  AsmLine(){}
  AsmDirective directive;
  union
  {
    Instruction inst;
    struct
    {
      uint64_t size;
      const uint8_t* bytes;
    }db;
  };
};

struct SymbolEntry
{
  int64_t lc;
  uint64_t label;
};

class Assembler
{
public:
  uint64_t org; //org just like teh directive in an assembler to give the origin address

private:
  uint64_t lc;  //local counter just like instruction pointer or program counter but for assemblers
  std::vector<AsmLine>listing;
  std::vector<SymbolEntry>symbol_table;
  std::vector<SymbolEntry>ref_table;
  //std::vector<uint64_t> globals;  //global variables in the file
  ZydisMachineMode machine_mode;
  uint8_t* assemble_1st_pass(uint64_t& size);
  bool assemble_2nd_pass(uint8_t* buffer, const uint64_t& size) const;

public:
  Assembler(const uint64_t org, const ZydisMachineMode machine_mode);
  void collect_globals();
  uint8_t* assemble(uint64_t& size);
  bool register_inst(const Instruction* inst);
  bool register_bytes(const uint64_t size, const uint8_t* bytes);
};
