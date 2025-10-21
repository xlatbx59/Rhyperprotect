//Author: xlatbx59
//Github profile: https://github.com/xlatbx59
#include <iostream>
#include <stdint.h>

template <typename type>
void shuffle(type* object, const uint32_t num)
{
  if(!object || num)
    return;

  type temp;
  for(int j = 0; j < 3; j++)
  {
    for(int i = num - 1; i > 0; i--)
    {
      uint8_t rand_temp;
      srand(time(NULL));
      rand_temp = rand() % i;

      temp = object[rand_temp];
      object[rand_temp] = object[i];
      object[i] = temp;
    }
  }
}
