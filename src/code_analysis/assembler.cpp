//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "assembler.hpp"

Assembler::Assembler(const uint64_t org, const ZydisMachineMode machine_mode)
{
  this->lc = 0;
  this->org = org;
  this->machine_mode = machine_mode;
}

bool Assembler::register_bytes(const uint64_t size, const uint8_t* bytes)
{
  AsmLine line;

  if(!size || !bytes)
    return false;
  
  line.directive = ASM_DIRECTIVE_DB;
  line.db.bytes = bytes;
  line.db.size = size;

  this->listing.push_back(line);
  return true;
}

bool Assembler::register_inst(const Instruction* inst)
{
  AsmLine line;

  memset(&line, 0, sizeof(AsmLine));
  line.directive = ASM_DIRECTIVE_INST;
  memcpy(&line.inst, inst, sizeof(Instruction));

  this->listing.push_back(line);
  return true;
}

uint8_t* Assembler::assemble_1st_pass(uint64_t& size)
{
  uint8_t* machine_code = nullptr, *temp = nullptr;
  uint64_t temp_size = 0, available_space = 0;
  ZydisEncoderRequest req;
  ZydisDecodedOperand operand;

  //Allocation for temporary machine code buffer
  for(AsmLine line : this->listing)
    if(line.directive != ASM_DIRECTIVE_DB)
      temp_size += 16;
    else
      temp_size += line.db.size;
  temp = new uint8_t[temp_size];
  memset(temp, 0, temp_size);

  //The assembling
  for(AsmLine line : this->listing)
  {
    if(line.directive == ASM_DIRECTIVE_DB)
    {
      memcpy(temp + this->lc, line.db.bytes, line.db.size);
      this->lc += line.db.size;
      continue;
    }

    SymbolEntry symbol_entry;
    symbol_entry.lc = this->lc;
    symbol_entry.label = line.inst.label;
    this->symbol_table.push_back(symbol_entry);

    memset(&req, 0, sizeof(ZydisEncoderRequest));
    req.machine_mode = this->machine_mode;
    req.mnemonic = line.inst.get_mnemonic();
    req.operand_count = line.inst.get_operand_count();
    for(int i = 0; i < req.operand_count; i++)
    {
      line.inst.get_operand(operand, i);
      switch(operand.type)
      {
        case ZYDIS_OPERAND_TYPE_REGISTER:
          req.operands[i].type = ZYDIS_OPERAND_TYPE_REGISTER;
          req.operands[i].reg.value = operand.reg.value;
          break;
        case ZYDIS_OPERAND_TYPE_MEMORY:
          req.operands[i].type = ZYDIS_OPERAND_TYPE_MEMORY;
          req.operands[i].mem.size = operand.size / 8;
          req.operands[i].mem.base = operand.mem.base;
          req.operands[i].mem.index = operand.mem.index;
          req.operands[i].mem.scale = operand.mem.scale;
          req.operands[i].mem.displacement = operand.mem.disp.value;
          break;
        case ZYDIS_OPERAND_TYPE_IMMEDIATE:
          req.operands[i].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
          if(line.inst.has_branch())
          {
            SymbolEntry ref_entry;
            req.branch_width = ZYDIS_BRANCH_WIDTH_32;
            line.inst.get_branch(ref_entry.label);
            ref_entry.lc = this->lc;
            ref_table.push_back(ref_entry);
          }
          else
            req.operands[i].imm.u = operand.imm.value.u;
          break;
        default:
          delete [] temp;
          temp = nullptr;
          return nullptr;
      }
    }
    available_space = temp_size - this->lc;
    ZyanStatus status = ZydisEncoderEncodeInstruction(&req, temp + this->lc, &available_space);
    if(ZYAN_FAILED(status))
    {
      delete [] temp;
      temp = nullptr;
      return nullptr;
    }
    this->lc += available_space;
  }


  machine_code = new uint8_t [lc];
  memcpy(machine_code, temp, this->lc);
  size = lc;

  return machine_code;
}
bool Assembler::assemble_2nd_pass(uint8_t* buffer, const uint64_t& size) const
{
  ZydisDecodedOperand operands[10];
  ZydisDecodedInstruction z_inst;
  uint64_t encoder_size = 0;
  ZydisEncoderRequest req;
  ZydisDecoder decoder;
  

	ZydisDecoderInit( &decoder, 
                    this->machine_mode,
                    this->machine_mode == ZYDIS_MACHINE_MODE_LONG_COMPAT_32 ? ZYDIS_STACK_WIDTH_32 : ZYDIS_STACK_WIDTH_64);

  memset(&req, 0, sizeof(ZydisEncoderRequest));
  req.machine_mode = this->machine_mode;
  req.operands[0].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
  req.branch_width = ZYDIS_BRANCH_WIDTH_32;

  for(SymbolEntry ref : ref_table)
  {
    for(SymbolEntry target : symbol_table)
    {
      if(target.label != ref.label)
        continue;

      if(ZYAN_FAILED(ZydisDecoderDecodeFull(
                /* decoder:         */ &decoder,
                /* buffer:          */ buffer + ref.lc,
                /* length:          */ this->lc - ref.lc,
                /* instruction:     */ &z_inst,
                /* operand:     	  */ operands
      )))
        return false;

      if (z_inst.operand_count_visible != 1 || operands[0].type != ZYDIS_OPERAND_TYPE_IMMEDIATE)
        return false;

      encoder_size = z_inst.length;
      req.mnemonic = z_inst.mnemonic;
      req.operand_count = z_inst.operand_count_visible;
      req.operands[0].imm.u = target.lc - (ref.lc + z_inst.length);
      printf("[*]Branch 0x%lx to %lx\r\n", ref.lc, ref.label);

      if(ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, buffer + ref.lc, &encoder_size)))
        return false;
    }
  }
  return true;
}

uint8_t* Assembler::assemble(uint64_t& size)
{
  uint8_t* machine_code = this->assemble_1st_pass(size);
  if(machine_code && this->assemble_2nd_pass(machine_code, size))
    return machine_code;
  if(machine_code)
    delete [] machine_code;

  size = 0;
  this->lc = 0;
  return nullptr;
}
