//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include <stdint.h>
#include <Zydis/Zydis.h>

class Instruction
{
public:
  uint64_t label;
private:
  ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT_VISIBLE];
  uint64_t branch;
  uint64_t fall;
  ZydisMnemonic mnemonic;
  uint8_t operand_num;
  bool has_fall_m;
  bool has_branch_m;
  ZydisMachineMode machine_mode;


public:
  Instruction() {}
  Instruction(const ZydisMnemonic mnemonic, const uint64_t label, const ZydisDecodedOperand* operands, const uint8_t op_num, ZydisMachineMode machine_mode, const uint64_t branch = 0, const uint64_t fall = 0, const bool has_branch = false, const bool has_fall = false);
  Instruction(const ZydisDecodedInstruction* z_inst, const ZydisDecodedOperand* operand, const uint64_t address);
  uint64_t get_address() const noexcept;
  ZydisMnemonic get_mnemonic() const noexcept;
  bool get_operand(ZydisDecodedOperand& operand, const uint8_t index) const noexcept;
  uint8_t get_operand_count() const noexcept;
  ZydisInstructionCategory get_branch_type() const noexcept;
  bool set_fall(uint64_t fall) noexcept;
  //bool prefix2attributes(ZydisInstructionAttributes& attributes)const noexcept;
  bool get_fall(uint64_t& fall) const noexcept;
  bool set_branch(uint64_t branch) noexcept;
  bool get_branch(uint64_t& branch) const noexcept;
  bool has_branch() const;
  ZydisMachineMode get_machine_mode() const noexcept;
  bool has_fall() const;
};

#define INST_NO_BRANCH(inst, mnemonic, label, operands, operand_num, machine_mode, fall) inst(mnemonic, label, operands, operand_num, machine_mode, 0, fall, false, true)
#define JMP_DIRECT(inst, label, operands, branch, machine_mode) inst(ZYDIS_MNEMONIC_JMP, label, operands, 1, machine_mode, branch, 0, true, false)
#define JMP_INDIRECT(inst, label, operands, machine_mode) inst(ZYDIS_MNEMONIC_JMP, label, operands, 1, machine_mode, 0, 0, false, false)
