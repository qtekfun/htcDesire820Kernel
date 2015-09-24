#ifndef _I386_ALTERNATIVE_H
#define _I386_ALTERNATIVE_H

#include <asm/types.h>
#include <linux/stddef.h>
#include <linux/types.h>

struct alt_instr {
	u8 *instr; 		
	u8 *replacement;
	u8  cpuid;		
	u8  instrlen;		
	u8  replacementlen; 	
	u8  pad;
};

extern void alternative_instructions(void);
extern void apply_alternatives(struct alt_instr *start, struct alt_instr *end);

struct module;
#ifdef CONFIG_SMP
extern void alternatives_smp_module_add(struct module *mod, char *name,
					void *locks, void *locks_end,
					void *text, void *text_end);
extern void alternatives_smp_module_del(struct module *mod);
extern void alternatives_smp_switch(int smp);
#else
static inline void alternatives_smp_module_add(struct module *mod, char *name,
					void *locks, void *locks_end,
					void *text, void *text_end) {}
static inline void alternatives_smp_module_del(struct module *mod) {}
static inline void alternatives_smp_switch(int smp) {}
#endif	

#define alternative(oldinstr, newinstr, feature)			\
	asm volatile ("661:\n\t" oldinstr "\n662:\n" 			\
		      ".section .altinstructions,\"a\"\n"		\
		      "  .align 4\n"					\
		      "  .long 661b\n"            		\
		      "  .long 663f\n"		  	\
		      "  .byte %c0\n"             	\
		      "  .byte 662b-661b\n"       	\
		      "  .byte 664f-663f\n"       	\
		      ".previous\n"					\
		      ".section .altinstr_replacement,\"ax\"\n"		\
		      "663:\n\t" newinstr "\n664:\n"   \
		      ".previous" :: "i" (feature) : "memory")

#define alternative_input(oldinstr, newinstr, feature, input...)	\
	asm volatile ("661:\n\t" oldinstr "\n662:\n"			\
		      ".section .altinstructions,\"a\"\n"		\
		      "  .align 4\n"					\
		      "  .long 661b\n"            		\
		      "  .long 663f\n"		   \
		      "  .byte %c0\n"             	\
		      "  .byte 662b-661b\n"       	\
		      "  .byte 664f-663f\n"        	\
		      ".previous\n"					\
		      ".section .altinstr_replacement,\"ax\"\n"		\
		      "663:\n\t" newinstr "\n664:\n"   \
		      ".previous" :: "i" (feature), ##input)

#define alternative_io(oldinstr, newinstr, feature, output, input...) \
	asm volatile ("661:\n\t" oldinstr "\n662:\n"			\
		      ".section .altinstructions,\"a\"\n"		\
		      "  .align 4\n"					\
		      "  .long 661b\n"            		\
		      "  .long 663f\n"		  	\
		      "  .byte %c[feat]\n"        	\
		      "  .byte 662b-661b\n"       	\
		      "  .byte 664f-663f\n"       	\
		      ".previous\n"					\
		      ".section .altinstr_replacement,\"ax\"\n"		\
		      "663:\n\t" newinstr "\n664:\n"    \
		      ".previous" : output : [feat] "i" (feature), ##input)

#define ASM_OUTPUT2(a, b) a, b


#ifdef CONFIG_SMP
#define LOCK_PREFIX \
		".section .smp_locks,\"a\"\n"	\
		"  .align 4\n"			\
		"  .long 661f\n" 	\
		".previous\n"			\
	       	"661:\n\tlock; "

#else 
#define LOCK_PREFIX ""
#endif

struct paravirt_patch_site;
#ifdef CONFIG_PARAVIRT
void apply_paravirt(struct paravirt_patch_site *start,
		    struct paravirt_patch_site *end);
#else
static inline void
apply_paravirt(struct paravirt_patch_site *start,
	       struct paravirt_patch_site *end)
{}
#define __parainstructions	NULL
#define __parainstructions_end	NULL
#endif

extern void text_poke(void *addr, unsigned char *opcode, int len);

#endif 
