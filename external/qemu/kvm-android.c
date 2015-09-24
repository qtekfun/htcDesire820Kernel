#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include "android/utils/debug.h"

#define D(...) VERBOSE_PRINT(init,__VA_ARGS__)


#ifndef __linux__
#error "This file should only be compiled under linux"
#endif

int
kvm_check_allowed(void)
{
    
    if (access("/dev/kvm",F_OK)) {
        
        D("No kvm device file detected");
        return 0;
    }

    
    if (access("/dev/kvm",R_OK)) {
        D("KVM device file is not readable for this user.");
        return 0;
    }

    D("KVM mode auto-enabled!");
    return 1;
}

