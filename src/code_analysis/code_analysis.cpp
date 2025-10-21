//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include "basic_block.hpp"
#include <queue>
#include <string.h>

using std::queue;

struct Symbol 
{
	uint64_t id;
	int64_t pc;
};

static bool next_branch(queue<ZyanU64>& branches, uint64_t& pc)
{
  if(!branches.size())
    return false;

  pc = branches.front();
  branches.pop();
  return true;
}
static bool next_call(queue<ZyanU64>& calls, uint64_t& pc)
{
  if(!calls.size())
    return false;

  pc = calls.front();
  calls.pop();
  return true;
}

bool disassemble( ZydisDecoder& decoder,
                      vector<BasicBlock>& cfg,
                      vector<uint64_t>& disassembled,
                      const uint8_t* start,
                      const uint64_t size,
                      uint64_t pc,
                      bool bits,
                      ZyanU64 runtime_address)
{
  ZydisDecodedOperand operand[ZYDIS_MAX_OPERAND_COUNT_VISIBLE]{0};
	ZydisDecodedInstruction z_inst;
  BasicBlock bb;
	vector<Instruction> insts;
	queue<ZyanU64> branches;
	queue<ZyanU64> calls;

	if (!start || !size)
		return false;

	ZydisDecoderInit( &decoder, 
                    bits ? ZYDIS_MACHINE_MODE_LONG_COMPAT_32 : ZYDIS_MACHINE_MODE_LONG_64,
                    bits ? ZYDIS_STACK_WIDTH_32 : ZYDIS_STACK_WIDTH_64);
	memset(&z_inst, 0, sizeof(z_inst));

	while (pc < size &&
		ZYAN_SUCCESS(ZydisDecoderDecodeFull(  //The function is good
			/* decoder:         */ &decoder,
			/* buffer:          */ start + pc,
			/* length:          */ size - pc,
			/* instruction:     */ &z_inst,
			/* operand:     	  */ operand
		)))
	{
    //New instruction to analyze/disassemble
		Instruction inst(&z_inst, operand, runtime_address + pc);
		insts.push_back(inst);
		disassembled.push_back(pc);

		switch (z_inst.meta.category)
		{
			case ZydisInstructionCategory::ZYDIS_CATEGORY_RET:
        cfg.push_back(new_bb(insts));
        insts.clear();
        if(!next_branch(branches, pc))
        {
          if(!next_call(calls, pc))
            return false;
        }
				break;

      case ZydisInstructionCategory::ZYDIS_CATEGORY_CALL:
        if(operand[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
          calls.push(z_inst.length + pc + operand[0].imm.value.u);
				pc += z_inst.length;
				break;

			case ZydisInstructionCategory::ZYDIS_CATEGORY_UNCOND_BR:
        cfg.push_back(new_bb(insts));
        insts.clear();
        if(operand[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
          branches.push(z_inst.length + pc + operand[0].imm.value.u);

        if(!next_branch(branches, pc))
        {
          if(!next_call(calls, pc))
            return true;
        }
				break;

			case ZydisInstructionCategory::ZYDIS_CATEGORY_COND_BR:
        cfg.push_back(new_bb(insts));
        insts.clear();
        branches.push(z_inst.length + pc + operand[0].imm.value.u);
				pc += z_inst.length;
				break;

			default:
				pc += z_inst.length;
		}

    //New pc was already disassembled
    for(int i = 0; i < disassembled.size(); i++)
    {
      if(disassembled[i] == pc)
      {
        if( z_inst.meta.category != ZydisInstructionCategory::ZYDIS_CATEGORY_UNCOND_BR &&
            z_inst.meta.category != ZydisInstructionCategory::ZYDIS_CATEGORY_COND_BR &&
            z_inst.meta.category != ZydisInstructionCategory::ZYDIS_CATEGORY_RET)
        {
          cfg.push_back(new_bb(insts));
          insts.clear();
        }

        //Looks for basic block if ever when it needs to be splitted 
        for(int i = 0;i < cfg.size(); i++)
        {
          if(cfg[i].bb_addr < runtime_address + pc && cfg[i].last_addr >= runtime_address + pc)
          {
            cfg[i].split(runtime_address + pc, bb);
            cfg.insert(cfg.begin() + i, bb);
            break;
          }
        }

        //Skip to new pc and restart the loop just in case
        if(!next_branch(branches, pc))
        {
          if(!next_call(calls, pc))
            return true;
        }
        i = 0;
      }
    }
	}
	return true;
}
