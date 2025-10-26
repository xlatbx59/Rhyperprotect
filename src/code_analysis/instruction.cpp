//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "instruction.hpp"


Instruction::Instruction(const ZydisMnemonic mnemonic, const uint64_t label, const ZydisDecodedOperand* operands, const uint8_t op_num, const ZydisMachineMode machine_mode, const uint64_t branch, const uint64_t fall, const bool has_branch, const bool has_fall)
{
  this->mnemonic = mnemonic;
  this->machine_mode = machine_mode;
  this->label = label;
  for(int i = 0; i < op_num; i++)
    this->operands[i] = operands[i];
  this->operand_num = op_num;
  this->branch = branch;
  this->fall = fall;
  this->has_branch_m = has_branch;
  this->has_fall_m = has_fall;
}
Instruction::Instruction( const Refs* refs,
                          const ZydisDecodedInstruction* z_inst,
                          const ZydisDecodedOperand* operand,
                          const uint64_t label)
{
  this->machine_mode = z_inst->machine_mode;
  switch (z_inst->meta.category)
  {
    case ZydisInstructionCategory::ZYDIS_CATEGORY_RET:
      this->branch_type = BranchType::Ret;
      break;
    case ZydisInstructionCategory::ZYDIS_CATEGORY_UNCOND_BR:
      if(operand[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
        this->branch = label + z_inst->length + operand[0].imm.value.u;
      this->branch_type = BranchType::UncondBranch;
      break;
    case ZydisInstructionCategory::ZYDIS_CATEGORY_CALL:
      this->branch_type = BranchType::Call;
      if(operand[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
        this->branch = label + z_inst->length + operand[0].imm.value.u;
      break;
    case ZydisInstructionCategory::ZYDIS_CATEGORY_COND_BR:
      this->branch_type = BranchType::CondBranch;
      this->branch = label + z_inst->length + operand[0].imm.value.u;
      this->fall = label + z_inst->length;
      break;
    default:
      this->fall = label + z_inst->length;
      this->branch_type = BranchType::Fall;
  }
  this->mnemonic = z_inst->mnemonic;
  this->label = label;
  this->operand_num = z_inst->operand_count_visible;
	for(int i = 0; i < z_inst->operand_count_visible; i++)
  {
		this->operands[i] = operand[i];
		this->ref[i] = refs[i];
  }
}

ZydisMnemonic Instruction::get_mnemonic() const noexcept
{
  return this->mnemonic;
}
uint64_t Instruction::get_address() const noexcept
{
  return this->label;
}

bool Instruction::get_operand(ZydisDecodedOperand& operand, const uint8_t index) const noexcept
{
	if (index >= this->operand_num)
		return false;
	operand = this->operands[index];
	return true;
}
BranchType Instruction::get_branch_type() const noexcept
{
  return this->branch_type;
}
RefType Instruction::get_ref_type(const uint8_t index) const noexcept
{
  return this->ref[index].type;
}
bool Instruction::get_ref(const uint8_t index, uint64_t& branch) const noexcept
{
  if(this->ref[index].type == NoRef)
    return false;

  branch = ref[index].ref;
  return true;
}
bool Instruction::get_fall(uint64_t& fall) const noexcept
{
  if(this->has_fall_m == false)
    return false;

  fall = this->fall;
  return true;
}

bool Instruction::has_branch() const 
{
  return this->has_branch_m;
}
bool Instruction::has_fall() const 
{
  return this->has_fall_m;
}

uint8_t Instruction::get_operand_count() const noexcept
{
  return this->operand_num;
}

bool Instruction::set_fall(uint64_t fall) noexcept
{
  if(!this->has_fall_m)
    return false;

  this->fall = fall;
  return true;
}


ZydisMachineMode Instruction::get_machine_mode() const noexcept
{
  return this->machine_mode;
}
