/**
 * @file main.c
 *
 * \brief Main of dummy
 *
 */
#include "autoconf.h"
#include "libc/types.h"
#include "libc/syscall.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libconsole.h"

char    buf[128] = { 0 };

/*
 * We use the local -fno-stack-protector flag for main because
 * the stack protection has not been initialized yet.
 */
int _main(uint32_t task_id)
{
    e_syscall_ret ret = 0;
    uint8_t id = 0;
    uint8_t id_crypto = 0;
    uint32_t len;
    uint32_t maxlen = 127;

    printf("Hello ! I'm benchlog, my id is %x\n", task_id);

    ret = sys_init(INIT_GETTASKID, "smart", &id);
    printf("smart is task %x !\n", id);

    ret = console_early_init(CONFIG_APP_BENCHLOG_USART, 115200);
    printf("Registered USART through libconsole. Returns %s !\n",
           strerror(ret));

    ret = sys_init(INIT_GETTASKID, "crypto", &id_crypto);
    printf("crypto is task %x !\n", id_crypto);

    ret = sys_init(INIT_DONE);
    printf("sys_init DONE returns %x !\n", ret);

    ret = console_init();
    if (ret == 0) {
        printf("USART%d is now configured !\n", CONFIG_APP_BENCHLOG_USART);
    } else {
        printf("error during configuration of USART%d\n", CONFIG_APP_BENCHLOG_USART);
    }

    printf("sending welcome msg to UART%d\n", CONFIG_APP_BENCHLOG_USART);
    console_log("[USART%x initialized for console output, baudrate=%d]\n", CONFIG_APP_BENCHLOG_USART,
                115200);

    console_log("Starting logger prompt\n");

    while (1) {
	console_show_prompt();
	console_readline(buf, &len, maxlen);
	console_log("You wrote: %s\n", buf);
	memset(buf, 0x0, 128);
    }

    return 0;
}
