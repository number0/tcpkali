#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <signal.h>

static int *flagvar;
static void signal_handler(int __attribute__((unused)) sig) {
    signal(sig, SIG_DFL);
    fprintf(stderr, "Ctrl+C pressed, finishing up...\n");
    *flagvar = 1;
}

void
block_term_signals() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, 0);
}

void
flagify_term_signals(int *flag) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_UNBLOCK, &set, 0);

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_mask = set;
    act.sa_flags = 0;
    act.sa_handler = signal_handler;
    sigemptyset(&act.sa_flags);
    flagvar = flag;
    sigaction(SIGINT, &act, 0);
}
