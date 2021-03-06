#ifndef _COMMON_H_
#define _COMMON_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <nrf.h>

#ifdef DUMMY_LTO
#define EXTERN static
#else
#define EXTERN
#endif

#define NOINIT_DATA __attribute__((section(".noinit")))

#define BLOCK_SIZE 128
#define MAX_BLOCK_COUNT 256
#define MAX_PACKET_SIZE (BLOCK_SIZE + 16)
#define MAX_SEND_SIZE 66 // Get Device Info Response with 32 bytes Name length

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

#define FORCE_LONG_JUMP(func) ((typeof(&(func)))_LONG_JUMP_helper(&(func)))
static inline void* _LONG_JUMP_helper(void* p) {
	__asm__ volatile ("":"+r"(p));
	return p;
}

// TODO: Mark some functions as non-essential, e.g. shutdown, sendDeviceInfo, sendHash, aes_hash
/*
Those functions will be moved at the end of code (after .rodata). If partial page 0 write was done
then it will reduce probabilyty of unusable recovery-bootloader. Controller, knowing that
recovery-bootloader is not fully functional, will use subset of protocol to avoid calling those
functions.
*/
#define NON_ESSENTIAL __attribute__((section(".non-essential")))
#define NON_ESSENTIAL_ENTRY __attribute__((section(".non-essential"))) __attribute__((noinline))

#ifdef __unix
#undef __unix
#endif

#include "main.h"
#include "utils.h"
#include "crypto.h"
#include "startup.h"
#include "radio.h"
#include "timer.h"
#include "rand.h"
#include "conn.h"
#include "req.h"

#endif