

#ifdef CONFIG_MEMCHECK


#include "memcheck/memcheck_proc_management.h"
#include "memcheck/memcheck_api.h"

AddrArray   ret_addresses = { 0 };

static inline int
watch_call_stack(DisasContext *s)
{
    if (!memcheck_enabled || !memcheck_watch_call_stack) {
        return 0;
    }

#ifndef CONFIG_USER_ONLY
    if (!s->user) {
        
        return 0;
    }
#endif  

    if (s->pc >= 0xC0000000 || (0xB0000000 <= s->pc && s->pc <= 0xB00FFFFF)) {
        return 0;
    }
    return 1;
}

static inline int
is_arm_bl_or_blx(uint32_t insn)
{
    if ((insn & 0x0F000000) == 0x0B000000 ||    
        (insn & 0xFE000000) == 0xFA000000 ||    
        (insn & 0x0FF000F0) == 0x12000030) {    
        return 1;
    }
    return 0;
}

static inline int
is_thumb_bl_or_blx(uint16_t insn, target_ulong pc, target_ulong* ret_off)
{
    if ((insn & 0xFF80) == 0x4780) {            
        *ret_off = 2;
        return 1;
    } else if ((insn & 0xF800) == 0xF000) {     
        
        insn = lduw_code(pc + 2);
        if ((insn & 0xC000) == 0xC000) {
            *ret_off = 4;
            return 1;
        }
    }
    return 0;
}

static int
register_ret_address(CPUState* env, target_ulong addr)
{
    int ret;
    if ((0x90000000 <= addr && addr <= 0xBFFFFFFF)) {
        ret = addrarray_add(&ret_addresses, get_phys_addr_code(env, addr));
    } else {
        ret = addrarray_add(&ret_addresses, get_phys_addr_code(env, addr));
    }
    assert(ret != 0);

    if (ret == 1) {
        TranslationBlock* tb;
        const target_ulong phys_pc = get_phys_addr_code(env, addr);
        const target_ulong phys_page1 = phys_pc & TARGET_PAGE_MASK;

        for(tb = tb_phys_hash[tb_phys_hash_func(phys_pc)]; tb != NULL;
            tb = tb->phys_hash_next) {
            if (tb->pc == addr && tb->page_addr[0] == phys_page1) {
                tb_phys_invalidate(tb, -1);
            }
        }
    }
    return ret;
}

static inline int
is_ret_address(CPUState* env, target_ulong addr)
{
    if ((0x90000000 <= addr && addr <= 0xBFFFFFFF)) {
        return addrarray_check(&ret_addresses, get_phys_addr_code(env, addr));
    } else {
        return addrarray_check(&ret_addresses, get_phys_addr_code(env, addr));
    }
}

static inline void
set_on_call(target_ulong pc, target_ulong ret)
{
    TCGv_ptr tmp_pc = tcg_const_ptr(pc & ~1);
    TCGv_ptr tmp_ret = tcg_const_ptr(ret & ~1);

    gen_helper_on_call(tmp_pc, tmp_ret);

    tcg_temp_free_ptr(tmp_ret);
    tcg_temp_free_ptr(tmp_pc);
}

static inline void
set_on_ret(target_ulong ret)
{
    TCGv_ptr tmp_ret = tcg_const_ptr(ret & ~1);

    gen_helper_on_ret(tmp_ret);

    tcg_temp_free_ptr(tmp_ret);
}


#  define ANDROID_WATCH_CALLSTACK_ARM(s) \
    if (watch_call_stack(s)) { \
        if (is_ret_address(env, s->pc)) { \
            set_on_ret(s->pc); \
        } \
        if (is_arm_bl_or_blx(insn)) { \
            set_on_call(s->pc, s->pc + 4); \
            if (!s->search_pc) { \
                register_ret_address(env, s->pc + 4); \
            } \
        } \
    }

#  define ANDROID_WATCH_CALLSTACK_THUMB(s) \
    if (watch_call_stack(s)) { \
        target_ulong ret_off; \
        if (is_ret_address(env, s->pc)) { \
            set_on_ret(s->pc); \
        } \
        if (is_thumb_bl_or_blx(insn, s->pc, &ret_off)) { \
            set_on_call(s->pc, s->pc + ret_off); \
            if (!s->search_pc) { \
                register_ret_address(env, s->pc + ret_off); \
            } \
        } \
    }

#  define ANDROID_DISAS_CONTEXT_FIELDS \
    int search_pc;

#  define ANDROID_START_CODEGEN(search_pc) \
    dc->search_pc = search_pc

#  define ANDROID_CHECK_CODEGEN_PC(search_pc) \
        ((search_pc) || (memcheck_enabled && dc->user))

#  define ANDROID_END_CODEGEN() \
    do { \
        if (memcheck_enabled && dc->user) { \
            j = gen_opc_ptr - gen_opc_buf; \
            lj++; \
            while (lj <= j) \
                gen_opc_instr_start[lj++] = 0; \
        } \
    } while (0)

#else 

#  define ANDROID_WATCH_CALLSTACK_ARM     ((void)0)
#  define ANDROID_WATCH_CALLSTACK_THUMB   ((void)0)
#  define ANDROID_DISAS_CONTEXT_FIELDS     
#  define ANDROID_START_CODEGEN(s)         ((void)(s))
#  define ANDROID_CHECK_CODEGEN_PC(s)      (s)
#  define ANDROID_END_CODEGEN()            ((void)0)

#endif  



#ifdef CONFIG_TRACE

#include "android-trace.h"
#define  gen_traceInsn()   gen_helper_traceInsn()

static void
gen_traceTicks( int  count )
{
    TCGv  tmp = tcg_temp_new_i32();
    tcg_gen_movi_i32(tmp, count);
    gen_helper_traceTicks(tmp);
    tcg_temp_free_i32(tmp);
}

static void
gen_traceBB( uint64_t  bbNum, void* tb )
{
#if HOST_LONG_BITS == 32
    TCGv_i64  tmpNum = tcg_temp_new_i64();
    TCGv_i32  tmpTb  = tcg_temp_new_i32();

    tcg_gen_movi_i64(tmpNum, (int64_t)bbNum);
    tcg_gen_movi_i32(tmpTb,  (int32_t)tb);
    gen_helper_traceBB32(tmpNum, tmpTb);
    tcg_temp_free_i32(tmpTb);
    tcg_temp_free_i64(tmpNum);
#elif HOST_LONG_BITS == 64
    TCGv_i64  tmpNum = tcg_temp_new_i64();
    TCGv_i64  tmpTb  = tcg_temp_new_i64();

    tcg_gen_movi_i64(tmpNum, (int64_t)bbNum);
    tcg_gen_movi_i64(tmpTb,  (int64_t)tb);
    gen_helper_traceBB64(tmpNum, tmpTb);
    tcg_temp_free_i64(tmpTb);
    tcg_temp_free_i64(tmpNum);
#endif
}

#  define ANDROID_TRACE_DECLS   int ticks = 0;

#  define ANDROID_TRACE_START_ARM() \
    do { \
        if (tracing) { \
            trace_add_insn(insn, 0); \
            ticks = get_insn_ticks_arm(insn); \
            gen_traceInsn(); \
        } \
    } while (0)

#  define ANDROID_TRACE_START_THUMB() \
    do { \
        if (tracing) { \
            int  ticks = get_insn_ticks_thumb(insn); \
            trace_add_insn( insn_wrap_thumb(insn), 1 ); \
            gen_traceInsn(); \
            gen_traceTicks(ticks); \
        } \
    } while (0)

#  define ANDROID_TRACE_GEN_TICKS() \
    do { \
        if (tracing) { \
        } \
    } while (0)

#  define ANDROID_TRACE_GEN_SINGLE_TICK() \
    do { \
        if (tracing) { \
            gen_traceTicks(1); \
            ticks -= 1; \
        } \
    } while (0)

# define ANDROID_TRACE_GEN_OTHER_TICKS() \
    do { \
        if (tracing && ticks > 0) { \
            gen_traceTicks(ticks); \
        } \
    } while (0)

#  define ANDROID_TRACE_START_BB() \
    do { \
        if (tracing) { \
            gen_traceBB(trace_static_bb_num(), tb); \
            trace_bb_start(dc->pc); \
        } \
    } while (0)

#  define ANDROID_TRACE_END_BB() \
    do { \
        if (tracing) { \
            trace_bb_end(); \
        } \
    } while (0)

#else 

#  define ANDROID_TRACE_DECLS         
#  define ANDROID_TRACE_START_ARM()   ((void)0)
#  define ANDROID_TRACE_START_THUMB() ((void)0)

#  define ANDROID_TRACE_GEN_TICKS()        ((void)0)
#  define ANDROID_TRACE_GEN_SINGLE_TICK()  ((void)0)
#  define ANDROID_TRACE_GEN_OTHER_TICKS()  ((void)0)

#  define ANDROID_TRACE_START_BB()         ((void)0)
#  define ANDROID_TRACE_END_BB()           ((void)0)

#endif 

