#ifndef MYOS_ELF64_H
#define MYOS_ELF64_H

#define PT_NULL 0 // Unused entry
#define PT_LOAD 1 // Loadable segment
#define PT_DYNAMIC 2 // Dynamic linking tables
#define PT_INTERP 3 // Program interpreter path name
#define PT_NOTE 4 // Note sections

typedef UINT64  Elf64_Addr;
typedef UINT16  Elf64_Half;
typedef UINT64  Elf64_Off;
typedef INT32   Elf64_Sword;
typedef INT64   Elf64_Sxword;
typedef UINT32  Elf64_Word;
typedef UINT64  Elf64_Lword;
typedef UINT64  Elf64_Xword;

typedef struct {
  unsigned char e_ident[16]; /* ELF identification */
  Elf64_Half e_type; /* Object file type */
  Elf64_Half e_machine; /* Machine type */
  Elf64_Word e_version; /* Object file version */
  Elf64_Addr e_entry; /* Entry point address */
  Elf64_Off e_phoff; /* Program header offset */
  Elf64_Off e_shoff; /* Section header offset */
  Elf64_Word e_flags; /* Processor-specific flags */
  Elf64_Half e_ehsize; /* ELF header size */
  Elf64_Half e_phentsize; /* Size of program header entry */
  Elf64_Half e_phnum; /* Number of program header entries */
  Elf64_Half e_shentsize; /* Size of section header entry */
  Elf64_Half e_shnum; /* Number of section header entries */
  Elf64_Half e_shstrndx; /* Section name string table index */
} Elf64_Ehdr;

typedef struct {
  Elf64_Word p_type; /* Type of segment */
  Elf64_Word p_flags; /* Segment attributes */
  Elf64_Off p_offset; /* Offset in file */
  Elf64_Addr p_vaddr; /* Virtual address in memory */
  Elf64_Addr p_paddr; /* Reserved */
  Elf64_Xword p_filesz; /* Size of segment in file */
  Elf64_Xword p_memsz; /* Size of segment in memory */
  Elf64_Xword p_align; /* Alignment of segment */
} Elf64_Phdr;

#endif
