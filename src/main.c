/**
 * @file main.c
 *
 * \brief Main of dummy
 *
 */
#include "libc/types.h"
#include "libc/syscall.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libconsole.h"

char buf[128] = { 0 };
/*
 * We use the local -fno-stack-protector flag for main because
 * the stack protection has not been initialized yet.
 */
int _main(uint32_t task_id)
{
    e_syscall_ret ret = 0;
    uint8_t id = 0;
    uint8_t id_crypto = 0;

    printf("Hello ! I'm benchlog, my id is %x\n", task_id);

    ret = sys_init(INIT_GETTASKID, "smart", &id);
    printf("smart is task %x !\n", id);

    ret = console_early_init(4, 115200);
    printf("Registered USART through libconsole. Returns %s !\n", strerror(ret));

    ret = sys_init(INIT_GETTASKID, "crypto", &id_crypto);
    printf("crypto is task %x !\n", id_crypto);



    ret = sys_init(INIT_DONE);
//    ret = sys_ipc(IPC_SEND_SYNC, 1, 15, buffer_out); // to sdio
    printf("sys_init DONE returns %x !\n", ret);

    ret = console_init();
    if (ret == 0) {
      printf("USART4 is now configured !\n");
    } else {
      printf("error during configuration of USART3\n");
    }

    printf("sending welcome msg to UART4\n");
    console_log("[USART%x initialized for console output, baudrate=%x]\n", 4, 115200);
    while (1) {
        logsize_t size = 128;
        id = id_crypto;
        ret = sys_ipc(IPC_RECV_SYNC, &id, &size, buf);
        if (ret == SYS_E_INVAL) {
            printf("error: recv_sync() returns SYS_E_INVAL\n");
        }
        if (ret == SYS_E_DONE) {
          console_log(buf);
        }
    }
    printf("welcome msg sent\n");

    while (1) {
        sys_yield();
    }
    /* should return to do_endoftask() */
    return 0;
}
