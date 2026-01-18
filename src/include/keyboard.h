#ifndef KFS_KEYBOARD_H
#define KFS_KEYBOARD_H

#include "io.h"

#define PS2_DATA   0x60
#define PS2_STATUS 0x64
#define PS2_OBF    0x01

static const char _kb_scancode_map[128] = {
    0,  27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
   '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\','z','x',
    'c','v','b','n','m',',','.','/', 0,  '*', 0,' ', 0,
};

#define KEYBOARD_POLL_CHAR() ({ \
    int _ch = -1; \
    if ((inb(PS2_STATUS) & PS2_OBF) != 0) { \
        unsigned char _sc = inb(PS2_DATA); \
        if ((_sc & 0x80) == 0 && _sc < 128) { \
            char _c = _kb_scancode_map[_sc]; \
            if (_c != 0) { \
                _ch = (int)_c; \
            } \
        } \
    } \
    _ch; \
})

#endif /* KFS_KEYBOARD_H */
