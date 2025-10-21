//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include <stdint.h>
#include "../utils.hpp"
#include "../code_analysis/basic_block.hpp"

using std::swap;

static void append_jmp();

void shuffle_bbs(vector<BasicBlock>& cfg)
{
  static uint64_t label = 0xb00b000000000000;
  uint32_t bb_num = cfg.size();
  ZydisDecodedOperand operand;
  operand.type = ZYDIS_OPERAND_TYPE_IMMEDIATE;

  for(int j = 0; j < 3; j++)
  {
    for(int i = cfg.size() - 1; i > 0; i--)
    {
      uint8_t rand_temp;
      srand(time(NULL));
      rand_temp = rand() % i;

      swap(cfg[rand_temp], cfg[i]);
    }
  }

  //Since the basic blocks have been scattered, the instructions 
  //with no branches that fall to a new basic block have to jump now
  for(int i = 0; i < bb_num; i++)
  {
    if( cfg[i].has_fall &&
        (i != cfg.size() - 1 && cfg[i].fall_addr != cfg[i + 1].bb_addr || i == cfg.size() - 1))
    {
      //Inserting jmps
      if(!cfg[i].has_branch)
        cfg[i].set_branch(cfg[i].fall_addr);
      else
      {
        Instruction JMP_DIRECT(jmp, label, &operand, cfg[i].fall_addr, cfg[i].machine_mode);
        cfg[i].fall_addr = label++;
        cfg.insert(cfg.begin() + i + 1, new_bb(jmp));
        bb_num++;
      }
    }
  }
}
