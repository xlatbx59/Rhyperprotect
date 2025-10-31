//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#pragma once
#include <stdint.h>
#include <Zydis/Zydis.h>

enum BranchType : uint8_t
{
  Ret, CondBranch, UncondBranch, Fall,
  Call
};
enum RefType : uint8_t
{
  NoRef, Rel, Absol
};

struct Refs
{
  RefType type;
  uint64_t ref;
};

class Instruction
{
public:
  uint64_t label;
private:
  Refs ref[ZYDIS_MAX_OPERAND_COUNT_VISIBLE];
  ZydisEncoderRequest req;
  BranchType branch_type;
  uint64_t fall;

public:
  Instruction() {}
  Instruction(const ZydisMnemonic mnemonic, const BranchType branch);
  Instruction(const Refs* refs, const ZydisDecodedInstruction* z_inst, const ZydisDecodedOperand* operand, const uint64_t address);
  uint64_t get_address() const noexcept;
  ZydisMnemonic get_mnemonic() const noexcept;
  bool get_operand(ZydisEncoderOperand& operand, const uint8_t index) const noexcept;
  uint8_t get_operand_count() const noexcept;
  bool set_fall(uint64_t fall) noexcept;
  bool get_fall(uint64_t& fall) const noexcept;
  BranchType get_branch_type() const noexcept;
  bool has_branch() const;
  bool get_ref(const uint8_t index, uint64_t& ref)const noexcept;
  RefType get_ref_type(const uint8_t index) const noexcept;
  ZydisMachineMode get_machine_mode() const noexcept;
  bool has_fall() const;
  void imm(const RefType ref, const uint64_t value);
  void reg(const ZydisRegister reg);
};
