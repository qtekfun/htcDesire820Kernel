#ifndef _LIBIPTC_H
#define _LIBIPTC_H

#include <linux/types.h>
#include <libiptc/ipt_kernel_headers.h>
#ifdef __cplusplus
#	include <climits>
#else
#	include <limits.h> 
#endif
#include <linux/netfilter_ipv4/ip_tables.h>

#ifdef __cplusplus
extern "C" {
#endif

struct iptc_handle;

typedef char ipt_chainlabel[32];

#define IPTC_LABEL_ACCEPT  "ACCEPT"
#define IPTC_LABEL_DROP    "DROP"
#define IPTC_LABEL_QUEUE   "QUEUE"
#define IPTC_LABEL_RETURN  "RETURN"

int iptc_is_chain(const char *chain, struct iptc_handle *const handle);

struct iptc_handle *iptc_init(const char *tablename);

void iptc_free(struct iptc_handle *h);

const char *iptc_first_chain(struct iptc_handle *handle);
const char *iptc_next_chain(struct iptc_handle *handle);

const struct ipt_entry *iptc_first_rule(const char *chain,
					struct iptc_handle *handle);

const struct ipt_entry *iptc_next_rule(const struct ipt_entry *prev,
				       struct iptc_handle *handle);

const char *iptc_get_target(const struct ipt_entry *e,
			    struct iptc_handle *handle);

int iptc_builtin(const char *chain, struct iptc_handle *const handle);

const char *iptc_get_policy(const char *chain,
			    struct ipt_counters *counter,
			    struct iptc_handle *handle);


int iptc_insert_entry(const ipt_chainlabel chain,
		      const struct ipt_entry *e,
		      unsigned int rulenum,
		      struct iptc_handle *handle);

int iptc_replace_entry(const ipt_chainlabel chain,
		       const struct ipt_entry *e,
		       unsigned int rulenum,
		       struct iptc_handle *handle);

int iptc_append_entry(const ipt_chainlabel chain,
		      const struct ipt_entry *e,
		      struct iptc_handle *handle);

int iptc_check_entry(const ipt_chainlabel chain,
		      const struct ipt_entry *origfw,
		      unsigned char *matchmask,
		      struct iptc_handle *handle);

int iptc_delete_entry(const ipt_chainlabel chain,
		      const struct ipt_entry *origfw,
		      unsigned char *matchmask,
		      struct iptc_handle *handle);

int iptc_delete_num_entry(const ipt_chainlabel chain,
			  unsigned int rulenum,
			  struct iptc_handle *handle);

const char *iptc_check_packet(const ipt_chainlabel chain,
			      struct ipt_entry *entry,
			      struct iptc_handle *handle);

int iptc_flush_entries(const ipt_chainlabel chain,
		       struct iptc_handle *handle);

int iptc_zero_entries(const ipt_chainlabel chain,
		      struct iptc_handle *handle);

int iptc_create_chain(const ipt_chainlabel chain,
		      struct iptc_handle *handle);

int iptc_delete_chain(const ipt_chainlabel chain,
		      struct iptc_handle *handle);

int iptc_rename_chain(const ipt_chainlabel oldname,
		      const ipt_chainlabel newname,
		      struct iptc_handle *handle);

int iptc_set_policy(const ipt_chainlabel chain,
		    const ipt_chainlabel policy,
		    struct ipt_counters *counters,
		    struct iptc_handle *handle);

int iptc_get_references(unsigned int *ref,
			const ipt_chainlabel chain,
			struct iptc_handle *handle);

struct ipt_counters *iptc_read_counter(const ipt_chainlabel chain,
				       unsigned int rulenum,
				       struct iptc_handle *handle);

int iptc_zero_counter(const ipt_chainlabel chain,
		      unsigned int rulenum,
		      struct iptc_handle *handle);

int iptc_set_counter(const ipt_chainlabel chain,
		     unsigned int rulenum,
		     struct ipt_counters *counters,
		     struct iptc_handle *handle);

int iptc_commit(struct iptc_handle *handle);

int iptc_get_raw_socket(void);

const char *iptc_strerror(int err);

extern void dump_entries(struct iptc_handle *const);

#ifdef __cplusplus
}
#endif


#endif 
