#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include <Arduino.h>

void ps2_init(void);
void ps2_read(void);
void ps2_buf_push(unsigned char c);
unsigned char ps2_buf_pull(void);
void ps2_buf_empty(void);
void ps2_check_clock();
void _decode(unsigned char sc);
void init_scan_code_to_ascii();

#endif
