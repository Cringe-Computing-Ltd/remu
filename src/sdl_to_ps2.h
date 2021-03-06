#pragma once
#include <map>
#include <SDL2/SDL.h>
#include "ps2_driver.h"

inline std::map<SDL_Scancode, PS2Driver::Key> SDL_TO_PS2 = {
    { SDL_SCANCODE_F3, PS2Driver::Key::KEY_F3 },
    { SDL_SCANCODE_F4, PS2Driver::Key::KEY_F4 },
    { SDL_SCANCODE_LCTRL, PS2Driver::Key::KEY_LEFT_CONTROL  },
    { SDL_SCANCODE_A, PS2Driver::Key::KEY_A },
    { SDL_SCANCODE_E, PS2Driver::Key::KEY_E },
    { SDL_SCANCODE_T, PS2Driver::Key::KEY_T },
    { SDL_SCANCODE_G, PS2Driver::Key::KEY_G },
    { SDL_SCANCODE_U, PS2Driver::Key::KEY_U },
    { SDL_SCANCODE_O, PS2Driver::Key::KEY_O },
    { SDL_SCANCODE_SEMICOLON, PS2Driver::Key::KEY_SEMICOLON },
    { SDL_SCANCODE_LEFTBRACKET, PS2Driver::Key::KEY_L_BRACKET },
    { SDL_SCANCODE_CAPSLOCK, PS2Driver::Key::KEY_CAPSLOCK },
    { SDL_SCANCODE_KP_7, PS2Driver::Key::KEY_KP_7 },
    { SDL_SCANCODE_KP_0, PS2Driver::Key::KEY_KP_0 },
    { SDL_SCANCODE_KP_6, PS2Driver::Key::KEY_KP_6 },
    { SDL_SCANCODE_F11, PS2Driver::Key::KEY_F11 },
    { SDL_SCANCODE_KP_MULTIPLY, PS2Driver::Key::KEY_KP_ASTERISK },
    { SDL_SCANCODE_AC_SEARCH, PS2Driver::Key::KEY_MM_WWW_SEARCH },
    { SDL_SCANCODE_RCTRL, PS2Driver::Key::KEY_RIGHT_CONTROL },
    { SDL_SCANCODE_AC_BOOKMARKS, PS2Driver::Key::KEY_MM_WWW_FAVOURITES },
    { SDL_SCANCODE_AC_REFRESH, PS2Driver::Key::KEY_MM_WWW_REFRESH },
    { SDL_SCANCODE_AC_STOP, PS2Driver::Key::KEY_MM_WWW_STOP },
    { SDL_SCANCODE_AC_FORWARD, PS2Driver::Key::KEY_MM_WWW_FORWARD },
    { SDL_SCANCODE_PAUSE, PS2Driver::Key::KEY_MM_PLAY_PAUSE },
    { SDL_SCANCODE_AC_BACK, PS2Driver::Key::KEY_MM_WWW_BACK },
    { SDL_SCANCODE_COMPUTER, PS2Driver::Key::KEY_MM_MY_COMPUTER },
    { SDL_SCANCODE_MAIL, PS2Driver::Key::KEY_MM_EMAIL },
    { SDL_SCANCODE_MEDIASELECT, PS2Driver::Key::KEY_MM_MEDIA_SELECT },
    { SDL_SCANCODE_HOME, PS2Driver::Key::KEY_HOME },
    { SDL_SCANCODE_INSERT, PS2Driver::Key::KEY_INSERT },
    { SDL_SCANCODE_RIGHT, PS2Driver::Key::KEY_CURSOR_RIGHT },
    { SDL_SCANCODE_F9, PS2Driver::Key::KEY_F9 },
    { SDL_SCANCODE_F1, PS2Driver::Key::KEY_F1 },
    { SDL_SCANCODE_F10, PS2Driver::Key::KEY_F10 },
    { SDL_SCANCODE_TAB, PS2Driver::Key::KEY_TAB },
    { SDL_SCANCODE_LALT, PS2Driver::Key::KEY_LEFT_ALT },
    { SDL_SCANCODE_Q, PS2Driver::Key::KEY_Q },
    { SDL_SCANCODE_W, PS2Driver::Key::KEY_W },
    { SDL_SCANCODE_C, PS2Driver::Key::KEY_C },
    { SDL_SCANCODE_4, PS2Driver::Key::KEY_4 },
    { SDL_SCANCODE_SPACE, PS2Driver::Key::KEY_SPACE },
    { SDL_SCANCODE_R, PS2Driver::Key::KEY_R },
    { SDL_SCANCODE_N, PS2Driver::Key::KEY_N },
    { SDL_SCANCODE_Y, PS2Driver::Key::KEY_Y },
    { SDL_SCANCODE_7, PS2Driver::Key::KEY_7 },
    { SDL_SCANCODE_COMMA, PS2Driver::Key::KEY_COMMA },
    { SDL_SCANCODE_0, PS2Driver::Key::KEY_0 },
    { SDL_SCANCODE_PERIOD, PS2Driver::Key::KEY_DOT },
    { SDL_SCANCODE_P, PS2Driver::Key::KEY_P },
    { SDL_SCANCODE_EQUALS, PS2Driver::Key::KEY_EQUAL },
    { SDL_SCANCODE_RSHIFT, PS2Driver::Key::KEY_RIGHT_SHIFT },
    { SDL_SCANCODE_BACKSLASH, PS2Driver::Key::KEY_BACK_SLASH },
    { SDL_SCANCODE_KP_1, PS2Driver::Key::KEY_KP_1 },
    { SDL_SCANCODE_KP_PERIOD, PS2Driver::Key::KEY_KP_DOT },
    { SDL_SCANCODE_KP_8, PS2Driver::Key::KEY_KP_8 },
    { SDL_SCANCODE_KP_PLUS, PS2Driver::Key::KEY_KP_PLUS },
    { SDL_SCANCODE_KP_9, PS2Driver::Key::KEY_KP_9 },
    { SDL_SCANCODE_RALT, PS2Driver::Key::KEY_RIGHT_ALT },
    { SDL_SCANCODE_AUDIOPREV, PS2Driver::Key::KEY_MM_PREVIOUS_TRACK },
    { SDL_SCANCODE_VOLUMEDOWN, PS2Driver::Key::KEY_MM_VOLUME_DOWN },
    { SDL_SCANCODE_AUDIONEXT, PS2Driver::Key::KEY_MM_NEXT_TRACK },
    { SDL_SCANCODE_END, PS2Driver::Key::KEY_END },
    { SDL_SCANCODE_DELETE, PS2Driver::Key::KEY_DELETE },
    { SDL_SCANCODE_UP, PS2Driver::Key::KEY_CURSOR_UP },
    { SDL_SCANCODE_PAGEUP, PS2Driver::Key::KEY_PAGE_UP },
    { SDL_SCANCODE_F2, PS2Driver::Key::KEY_F2 },
    { SDL_SCANCODE_F8, PS2Driver::Key::KEY_F8 },
    { SDL_SCANCODE_GRAVE, PS2Driver::Key::KEY_BACKTICK },
    { SDL_SCANCODE_LSHIFT, PS2Driver::Key::KEY_LEFT_SHIFT },
    { SDL_SCANCODE_1, PS2Driver::Key::KEY_1 },
    { SDL_SCANCODE_Z, PS2Driver::Key::KEY_Z },
    { SDL_SCANCODE_2, PS2Driver::Key::KEY_2 },
    { SDL_SCANCODE_X, PS2Driver::Key::KEY_X },
    { SDL_SCANCODE_3, PS2Driver::Key::KEY_3 },
    { SDL_SCANCODE_V, PS2Driver::Key::KEY_V },
    { SDL_SCANCODE_5, PS2Driver::Key::KEY_5 },
    { SDL_SCANCODE_B, PS2Driver::Key::KEY_B },
    { SDL_SCANCODE_6, PS2Driver::Key::KEY_6 },
    { SDL_SCANCODE_M, PS2Driver::Key::KEY_M },
    { SDL_SCANCODE_8, PS2Driver::Key::KEY_8 },
    { SDL_SCANCODE_K, PS2Driver::Key::KEY_K },
    { SDL_SCANCODE_9, PS2Driver::Key::KEY_9 },
    { SDL_SCANCODE_SLASH, PS2Driver::Key::KEY_FORWARD_SLASH },
    { SDL_SCANCODE_MINUS, PS2Driver::Key::KEY_MINUS },
    { SDL_SCANCODE_APOSTROPHE, PS2Driver::Key::KEY_QUOTE },
    { SDL_SCANCODE_RETURN, PS2Driver::Key::KEY_ENTER },
    { SDL_SCANCODE_BACKSPACE, PS2Driver::Key::KEY_BACKSPACE },
    { SDL_SCANCODE_KP_2, PS2Driver::Key::KEY_KP_2 },
    { SDL_SCANCODE_ESCAPE, PS2Driver::Key::KEY_ESCAPE },
    { SDL_SCANCODE_KP_3, PS2Driver::Key::KEY_KP_3 },
    { SDL_SCANCODE_SCROLLLOCK, PS2Driver::Key::KEY_SCROLLLOCK },
    { SDL_SCANCODE_VOLUMEUP, PS2Driver::Key::KEY_MM_VOLUME_UP },
    { SDL_SCANCODE_AC_HOME, PS2Driver::Key::KEY_MM_WWW_HOME },
    { SDL_SCANCODE_KP_DIVIDE, PS2Driver::Key::KEY_KP_FORWARD_SLASH },
    { SDL_SCANCODE_KP_ENTER, PS2Driver::Key::KEY_KP_ENTER },
    //{ SDL_SCANCODE_, PS2Driver::Key::KEY_ACPI_WAKE },
    { SDL_SCANCODE_DOWN, PS2Driver::Key::KEY_CURSOR_DOWN },
    { SDL_SCANCODE_PAGEDOWN, PS2Driver::Key::KEY_PAGE_DOWN },
    { SDL_SCANCODE_F5, PS2Driver::Key::KEY_F5 },
    { SDL_SCANCODE_F12, PS2Driver::Key::KEY_F12 },
    { SDL_SCANCODE_F6, PS2Driver::Key::KEY_F6},
    { SDL_SCANCODE_S, PS2Driver::Key::KEY_S },
    { SDL_SCANCODE_D, PS2Driver::Key::KEY_D },
    { SDL_SCANCODE_F, PS2Driver::Key::KEY_F },
    { SDL_SCANCODE_H, PS2Driver::Key::KEY_H },
    { SDL_SCANCODE_J, PS2Driver::Key::KEY_J },
    { SDL_SCANCODE_I, PS2Driver::Key::KEY_I },
    { SDL_SCANCODE_L, PS2Driver::Key::KEY_L },
    { SDL_SCANCODE_RIGHTBRACKET, PS2Driver::Key::KEY_R_BRACKET },
    { SDL_SCANCODE_KP_4, PS2Driver::Key::KEY_KP_4 },
    { SDL_SCANCODE_KP_5, PS2Driver::Key::KEY_KP_5 },
    { SDL_SCANCODE_NUMLOCKCLEAR, PS2Driver::Key::KEY_NUMBERLOCK },
    { SDL_SCANCODE_KP_MINUS, PS2Driver::Key::KEY_KP_MINUS },
    { SDL_SCANCODE_F7, PS2Driver::Key::KEY_F7 },
    { SDL_SCANCODE_LGUI, PS2Driver::Key::KEY_LEFT_GUI },
    { SDL_SCANCODE_AUDIOMUTE, PS2Driver::Key::KEY_MM_MUTE },
    { SDL_SCANCODE_RGUI, PS2Driver::Key::KEY_RIGHT_GUI },
    { SDL_SCANCODE_CALCULATOR, PS2Driver::Key::KEY_MM_CALCULATOR },
    { SDL_SCANCODE_APPLICATION, PS2Driver::Key::KEY_APPS },
    { SDL_SCANCODE_POWER, PS2Driver::Key::KEY_ACPI_POWER },
    { SDL_SCANCODE_AC_STOP, PS2Driver::Key::KEY_MM_STOP },
    { SDL_SCANCODE_SLEEP, PS2Driver::Key::KEY_ACPI_SLEEP },
    { SDL_SCANCODE_LEFT, PS2Driver::Key::KEY_CURSOR_LEFT }
};