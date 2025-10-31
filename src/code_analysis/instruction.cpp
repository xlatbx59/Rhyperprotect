//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "instruction.hpp"

Instruction::Instruction( const Refs* refs,
                          const ZydisDecodedInstruction* z_inst,
                          const ZydisDecodedOperand* operand,
                          const uint64_t label)
{
  ZydisEncoderDecodedInstructionToEncoderRequest(z_inst, operand, z_inst->operand_count_visible, &this->req);
  switch (z_inst->meta.category)
  {
    case ZydisInstructionCategory::ZYDIS_CATEGORY_RET:
      this->branch_type = BranchType::Ret;
      break;
    case ZydisInstructionCategory::ZYDIS_CATEGORY_UNCOND_BR:
      this->branch_type = BranchType::UncondBranch;
      break;
    case ZydisInstructionCategory::ZYDIS_CATEGORY_CALL:
      this->branch_type = BranchType::Call;
      break;
    case ZydisInstructionCategory::ZYDIS_CATEGORY_COND_BR:
      this->branch_type = BranchType::CondBranch;
      this->fall = label + z_inst->length;
      break;
    default:
      this->fall = label + z_inst->length;
      this->branch_type = BranchType::Fall;
  }
  this->label = label;
	for(int i = 0; i < z_inst->operand_count_visible; i++)
		this->ref[i] = refs[i];
}

ZydisMnemonic Instruction::get_mnemonic() const noexcept
{
  return this->req.mnemonic;
}
uint64_t Instruction::get_address() const noexcept
{
  return this->label;
}

bool Instruction::get_operand(ZydisEncoderOperand& operand, const uint8_t index) const noexcept
{
	if (index >= this->req.operand_count)
		return false;
	operand = this->req.operands[index];
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
  if(this->has_fall())
    return false;

  fall = this->fall;
  return true;
}

bool Instruction::has_branch() const 
{
  return this->branch_type == UncondBranch || this-> branch_type == CondBranch ? true : false;
}
bool Instruction::has_fall() const 
{
  return this->branch_type == Fall || this-> branch_type == CondBranch ? true : false;
}
uint8_t Instruction::get_operand_count() const noexcept
{
  return this->req.operand_count;
}

bool Instruction::set_fall(uint64_t fall) noexcept
{
  if(!this->has_fall())
    return false;

  this->fall = fall;
  return true;
}

ZydisMachineMode Instruction::get_machine_mode() const noexcept
{
  return this->req.machine_mode;
}
void Instruction::reg(const ZydisRegister reg)
{
  this->req.operand_count++;
  this->req.operands[this->req.operand_count - 1].type = ZYDIS_OPERAND_TYPE;
  this->req.operands[this->req.operand_count - 1].reg.value = reg;
}
