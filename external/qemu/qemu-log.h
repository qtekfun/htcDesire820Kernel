#ifndef QEMU_LOG_H
#define QEMU_LOG_H

extern FILE *logfile;
extern int loglevel;




#define qemu_log_enabled() (logfile != NULL)

#define qemu_loglevel_mask(b) ((loglevel & (b)) != 0)



#define qemu_log(...) do {                 \
        if (logfile)                       \
            fprintf(logfile, ## __VA_ARGS__); \
    } while (0)

#define qemu_log_vprintf(fmt, va) do {     \
        if (logfile)                       \
            vfprintf(logfile, fmt, va);    \
    } while (0)

#define qemu_log_mask(b, ...) do {         \
        if (loglevel & (b))                \
            fprintf(logfile, ## __VA_ARGS__); \
    } while (0)





#define log_cpu_state(env, f) cpu_dump_state((env), logfile, fprintf, (f));
#define log_cpu_state_mask(b, env, f) do {           \
      if (loglevel & (b)) log_cpu_state((env), (f)); \
  } while (0)

#define log_target_disas(start, len, flags) \
        target_disas(logfile, (start), (len), (flags))
#define log_disas(start, len) \
        disas(logfile, (start), (len))

#define log_page_dump() page_dump(logfile)




#define qemu_log_flush() fflush(logfile)

#define qemu_log_close() do { \
        fclose(logfile);      \
        logfile = NULL;       \
    } while (0)

#define qemu_log_set_file(f) do { \
        logfile = (f);            \
    } while (0)

#define qemu_log_try_set_file(f) do { \
        if (!logfile)                 \
            logfile = (f);            \
    } while (0)


#endif
