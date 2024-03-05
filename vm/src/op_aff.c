#include "op.h"

int inst_aff(champion_t *champion, core_t *core, code_t code, int *args) {
  char c;
  int reg = args[VALUE_1];

  // check if the register is valid
  if (reg < 0 || reg >= REG_NUMBER) {
    return 1;
  }

  // get value from the register
  int register_value = champion->reg[reg];

  c = (char)(register_value % 256);
  write(STDOUT_FILENO, &c, 1);

  return 0;
}
