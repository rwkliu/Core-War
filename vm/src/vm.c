#include "vm_parse.h"
#include "core.h" 
#include "memory.h"
#include "op.h"
#include "inst_ex.h"

int main(int argc, char **argv) {
  // parse arguments
  if (argc < 2) {
    print_usage();
    return EXIT_FAILURE;
  }
  
  flag_t *flags = init_flag();
  champion_t* head = parse_args(argc, argv, &flags);

  // init core, load champs, run core
  core_t *core = init_core(head, flags);

  // print winner

  // memory cleanup
  cleanup(head, core);

  return 0;
}