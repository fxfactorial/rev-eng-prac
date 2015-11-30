#include <stdio.h>
#include <stdlib.h>
#include <mach-o/arch.h>
#include <mach-o/loader.h>
#include <mach-o/swap.h>
#include <stdbool.h>

#import <Foundation/Foundation.h>

bool is_magic_64(uint32_t magic)
{
  return magic == MH_MAGIC_64 || magic == MH_CIGAM_64;
}

bool should_swap_bytes(uint32_t magic)
{
  return magic == MH_CIGAM || magic == MH_CIGAM_64;
}

void *load_bytes(FILE *obj_file, int offset, int size)
{
  void *buf = calloc(1, size);
  fseek(obj_file, offset, SEEK_SET);
  fread(buf, size, 1, obj_file);
  return buf;
}

uint32_t read_magic(FILE *obj_file, int offset)
{
  uint32_t magic;
  fseek(obj_file, offset, SEEK_SET);
  fread(&magic, sizeof(uint32_t), 1, obj_file);
  NSLog(@"%x", magic);
  return magic;
}

void dump_mach_header(FILE *obj_file, int offset, bool is_64, bool is_swap)
{
  uint32_t n_cmds;
  int load_commands_offset = offset;
  const NXArchInfo *arch_info = NULL;

  if (is_64) {
    int header_size = sizeof(struct mach_header_64);
    struct mach_header_64 *header = load_bytes(obj_file, offset, header_size);
    n_cmds = header->ncmds;
    load_commands_offset += header_size;
    if (is_swap)
      swap_mach_header_64(header, 0);
    arch_info = NXGetArchInfoFromCpuType(header->cputype, header->cpusubtype);
    free(header);

  } else {
    int header_size = sizeof(struct mach_header);
    struct mach_header *header = load_bytes(obj_file, offset, header_size);
    arch_info = NXGetArchInfoFromCpuType(header->cputype, header->cpusubtype);
    if (is_swap)
      swap_mach_header(header, 0);
    n_cmds = header->ncmds;
    load_commands_offset += header_size;
    free(header);
  }
  NSLog(@"%s", arch_info->name);
  /* dump_segment_commands(obj_file, load_commands_offset, is_swap, ncmds); */
}

// Driver
void dump_segments (FILE *obj_file)
{
  uint32_t magic = read_magic(obj_file, 0);
  bool is_64 = is_magic_64(magic);
  bool should_swap = should_swap_bytes(magic);
  dump_mach_header(obj_file, 0, is_64, should_swap);
}

int main(int argc, char **argv)
{
  // Needs the __block type specifier to be able to mutate in a block.
  __block int value = 10;
  int (^foo)(int, int) = ^(int first, int second) {
    value = 20;
    return first + second;
  };

  const char *filename = argv[1];
  FILE *obj_file = fopen(filename, "rb");
  dump_segments(obj_file);
  fclose(obj_file);
  return 0;
}
