/*
 * @name BookFiler Widget IO
 * @author Branden Lee
 * @version 1.00
 * @license GNU LGPL v3
 * @brief BookFiler generic widget IO
 */

#ifndef BOOKFILER_IO_VK_H
#define BOOKFILER_IO_VK_H

/*
 * Virtual Keys, Standard Set
 * Same as the windows API
 */
#define BOOKFILER_IO_VK_LBUTTON        0x01
#define BOOKFILER_IO_VK_RBUTTON        0x02
#define BOOKFILER_IO_VK_CANCEL         0x03
#define BOOKFILER_IO_VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define BOOKFILER_IO_VK_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define BOOKFILER_IO_VK_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */

/*
 * 0x07 : reserved
 */
#define BOOKFILER_IO_VK_BACK           0x08
#define BOOKFILER_IO_VK_TAB            0x09

/*
 * 0x0A - 0x0B : reserved
 */
#define BOOKFILER_IO_VK_CLEAR          0x0C
#define BOOKFILER_IO_VK_RETURN         0x0D

/*
 * 0x0E - 0x0F : unassigned
 */
#define BOOKFILER_IO_VK_SHIFT          0x10
#define BOOKFILER_IO_VK_CONTROL        0x11
#define BOOKFILER_IO_VK_MENU           0x12
#define BOOKFILER_IO_VK_PAUSE          0x13
#define BOOKFILER_IO_VK_CAPITAL        0x14

#define BOOKFILER_IO_VK_KANA           0x15
#define BOOKFILER_IO_VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define BOOKFILER_IO_VK_HANGUL         0x15

/*
 * 0x16 : unassigned
 */
#define BOOKFILER_IO_VK_JUNJA          0x17
#define BOOKFILER_IO_VK_FINAL          0x18
#define BOOKFILER_IO_VK_HANJA          0x19
#define BOOKFILER_IO_VK_KANJI          0x19

/*
 * 0x1A : unassigned
 */
#define BOOKFILER_IO_VK_ESCAPE         0x1B

#define BOOKFILER_IO_VK_CONVERT        0x1C
#define BOOKFILER_IO_VK_NONCONVERT     0x1D
#define BOOKFILER_IO_VK_ACCEPT         0x1E
#define BOOKFILER_IO_VK_MODECHANGE     0x1F

#define BOOKFILER_IO_VK_SPACE          0x20
#define BOOKFILER_IO_VK_PRIOR          0x21
#define BOOKFILER_IO_VK_NEXT           0x22
#define BOOKFILER_IO_VK_END            0x23
#define BOOKFILER_IO_VK_HOME           0x24
#define BOOKFILER_IO_VK_LEFT           0x25
#define BOOKFILER_IO_VK_UP             0x26
#define BOOKFILER_IO_VK_RIGHT          0x27
#define BOOKFILER_IO_VK_DOWN           0x28
#define BOOKFILER_IO_VK_SELECT         0x29
#define BOOKFILER_IO_VK_PRINT          0x2A
#define BOOKFILER_IO_VK_EXECUTE        0x2B
#define BOOKFILER_IO_VK_SNAPSHOT       0x2C
#define BOOKFILER_IO_VK_INSERT         0x2D
#define BOOKFILER_IO_VK_DELETE         0x2E
#define BOOKFILER_IO_VK_HELP           0x2F

/*
 * BOOKFILER_IO_VK_0 - BOOKFILER_IO_VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x3A - 0x40 : unassigned
 * BOOKFILER_IO_VK_A - BOOKFILER_IO_VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
#define BOOKFILER_IO_VK_LWIN           0x5B
#define BOOKFILER_IO_VK_RWIN           0x5C
#define BOOKFILER_IO_VK_APPS           0x5D

/*
 * 0x5E : reserved
 */
#define BOOKFILER_IO_VK_SLEEP          0x5F

#define BOOKFILER_IO_VK_NUMPAD0        0x60
#define BOOKFILER_IO_VK_NUMPAD1        0x61
#define BOOKFILER_IO_VK_NUMPAD2        0x62
#define BOOKFILER_IO_VK_NUMPAD3        0x63
#define BOOKFILER_IO_VK_NUMPAD4        0x64
#define BOOKFILER_IO_VK_NUMPAD5        0x65
#define BOOKFILER_IO_VK_NUMPAD6        0x66
#define BOOKFILER_IO_VK_NUMPAD7        0x67
#define BOOKFILER_IO_VK_NUMPAD8        0x68
#define BOOKFILER_IO_VK_NUMPAD9        0x69
#define BOOKFILER_IO_VK_MULTIPLY       0x6A
#define BOOKFILER_IO_VK_ADD            0x6B
#define BOOKFILER_IO_VK_SEPARATOR      0x6C
#define BOOKFILER_IO_VK_SUBTRACT       0x6D
#define BOOKFILER_IO_VK_DECIMAL        0x6E
#define BOOKFILER_IO_VK_DIVIDE         0x6F
#define BOOKFILER_IO_VK_F1             0x70
#define BOOKFILER_IO_VK_F2             0x71
#define BOOKFILER_IO_VK_F3             0x72
#define BOOKFILER_IO_VK_F4             0x73
#define BOOKFILER_IO_VK_F5             0x74
#define BOOKFILER_IO_VK_F6             0x75
#define BOOKFILER_IO_VK_F7             0x76
#define BOOKFILER_IO_VK_F8             0x77
#define BOOKFILER_IO_VK_F9             0x78
#define BOOKFILER_IO_VK_F10            0x79
#define BOOKFILER_IO_VK_F11            0x7A
#define BOOKFILER_IO_VK_F12            0x7B
#define BOOKFILER_IO_VK_F13            0x7C
#define BOOKFILER_IO_VK_F14            0x7D
#define BOOKFILER_IO_VK_F15            0x7E
#define BOOKFILER_IO_VK_F16            0x7F
#define BOOKFILER_IO_VK_F17            0x80
#define BOOKFILER_IO_VK_F18            0x81
#define BOOKFILER_IO_VK_F19            0x82
#define BOOKFILER_IO_VK_F20            0x83
#define BOOKFILER_IO_VK_F21            0x84
#define BOOKFILER_IO_VK_F22            0x85
#define BOOKFILER_IO_VK_F23            0x86
#define BOOKFILER_IO_VK_F24            0x87

/*
 * 0x88 - 0x8F : UI navigation
 */
#define BOOKFILER_IO_VK_NAVIGATION_VIEW     0x88
#define BOOKFILER_IO_VK_NAVIGATION_MENU     0x89
#define BOOKFILER_IO_VK_NAVIGATION_UP       0x8A
#define BOOKFILER_IO_VK_NAVIGATION_DOWN     0x8B
#define BOOKFILER_IO_VK_NAVIGATION_LEFT     0x8C
#define BOOKFILER_IO_VK_NAVIGATION_RIGHT    0x8D
#define BOOKFILER_IO_VK_NAVIGATION_ACCEPT   0x8E
#define BOOKFILER_IO_VK_NAVIGATION_CANCEL   0x8F

#define BOOKFILER_IO_VK_NUMLOCK        0x90
#define BOOKFILER_IO_VK_SCROLL         0x91

/*
 * NEC PC-9800 kbd definitions
 */
#define BOOKFILER_IO_VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad

/*
 * Fujitsu/OASYS kbd definitions
 */
#define BOOKFILER_IO_VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define BOOKFILER_IO_VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define BOOKFILER_IO_VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define BOOKFILER_IO_VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define BOOKFILER_IO_VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * BOOKFILER_IO_VK_L* & BOOKFILER_IO_VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define BOOKFILER_IO_VK_LSHIFT         0xA0
#define BOOKFILER_IO_VK_RSHIFT         0xA1
#define BOOKFILER_IO_VK_LCONTROL       0xA2
#define BOOKFILER_IO_VK_RCONTROL       0xA3
#define BOOKFILER_IO_VK_LMENU          0xA4
#define BOOKFILER_IO_VK_RMENU          0xA5

#define BOOKFILER_IO_VK_BROWSER_BACK        0xA6
#define BOOKFILER_IO_VK_BROWSER_FORWARD     0xA7
#define BOOKFILER_IO_VK_BROWSER_REFRESH     0xA8
#define BOOKFILER_IO_VK_BROWSER_STOP        0xA9
#define BOOKFILER_IO_VK_BROWSER_SEARCH      0xAA
#define BOOKFILER_IO_VK_BROWSER_FAVORITES   0xAB
#define BOOKFILER_IO_VK_BROWSER_HOME        0xAC

#define BOOKFILER_IO_VK_VOLUME_MUTE         0xAD
#define BOOKFILER_IO_VK_VOLUME_DOWN         0xAE
#define BOOKFILER_IO_VK_VOLUME_UP           0xAF
#define BOOKFILER_IO_VK_MEDIA_NEXT_TRACK    0xB0
#define BOOKFILER_IO_VK_MEDIA_PREV_TRACK    0xB1
#define BOOKFILER_IO_VK_MEDIA_STOP          0xB2
#define BOOKFILER_IO_VK_MEDIA_PLAY_PAUSE    0xB3
#define BOOKFILER_IO_VK_LAUNCH_MAIL         0xB4
#define BOOKFILER_IO_VK_LAUNCH_MEDIA_SELECT 0xB5
#define BOOKFILER_IO_VK_LAUNCH_APP1         0xB6
#define BOOKFILER_IO_VK_LAUNCH_APP2         0xB7

/*
 * 0xB8 - 0xB9 : reserved
 */

#define BOOKFILER_IO_VK_OEM_1          0xBA   // ';:' for US
#define BOOKFILER_IO_VK_OEM_PLUS       0xBB   // '+' any country
#define BOOKFILER_IO_VK_OEM_COMMA      0xBC   // ',' any country
#define BOOKFILER_IO_VK_OEM_MINUS      0xBD   // '-' any country
#define BOOKFILER_IO_VK_OEM_PERIOD     0xBE   // '.' any country
#define BOOKFILER_IO_VK_OEM_2          0xBF   // '/?' for US
#define BOOKFILER_IO_VK_OEM_3          0xC0   // '`~' for US

/*
 * 0xC1 - 0xC2 : reserved
 */

/*
 * 0xC3 - 0xDA : Gamepad input
 */
#define BOOKFILER_IO_VK_GAMEPAD_A                         0xC3
#define BOOKFILER_IO_VK_GAMEPAD_B                         0xC4
#define BOOKFILER_IO_VK_GAMEPAD_X                         0xC5
#define BOOKFILER_IO_VK_GAMEPAD_Y                         0xC6
#define BOOKFILER_IO_VK_GAMEPAD_RIGHT_SHOULDER            0xC7
#define BOOKFILER_IO_VK_GAMEPAD_LEFT_SHOULDER             0xC8
#define BOOKFILER_IO_VK_GAMEPAD_LEFT_TRIGGER              0xC9
#define BOOKFILER_IO_VK_GAMEPAD_RIGHT_TRIGGER             0xCA
#define BOOKFILER_IO_VK_GAMEPAD_DPAD_UP                   0xCB
#define BOOKFILER_IO_VK_GAMEPAD_DPAD_DOWN                 0xCC
#define BOOKFILER_IO_VK_GAMEPAD_DPAD_LEFT                 0xCD
#define BOOKFILER_IO_VK_GAMEPAD_DPAD_RIGHT                0xCE
#define BOOKFILER_IO_VK_GAMEPAD_MENU                      0xCF
#define BOOKFILER_IO_VK_GAMEPAD_VIEW                      0xD0
#define BOOKFILER_IO_VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON    0xD1
#define BOOKFILER_IO_VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON   0xD2
#define BOOKFILER_IO_VK_GAMEPAD_LEFT_THUMBSTICK_UP        0xD3
#define BOOKFILER_IO_VK_GAMEPAD_LEFT_THUMBSTICK_DOWN      0xD4
#define BOOKFILER_IO_VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT     0xD5
#define BOOKFILER_IO_VK_GAMEPAD_LEFT_THUMBSTICK_LEFT      0xD6
#define BOOKFILER_IO_VK_GAMEPAD_RIGHT_THUMBSTICK_UP       0xD7
#define BOOKFILER_IO_VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN     0xD8
#define BOOKFILER_IO_VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT    0xD9
#define BOOKFILER_IO_VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT     0xDA

#define BOOKFILER_IO_VK_OEM_4          0xDB  //  '[{' for US
#define BOOKFILER_IO_VK_OEM_5          0xDC  //  '\|' for US
#define BOOKFILER_IO_VK_OEM_6          0xDD  //  ']}' for US
#define BOOKFILER_IO_VK_OEM_7          0xDE  //  ''"' for US
#define BOOKFILER_IO_VK_OEM_8          0xDF

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */
#define BOOKFILER_IO_VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define BOOKFILER_IO_VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define BOOKFILER_IO_VK_ICO_HELP       0xE3  //  Help key on ICO
#define BOOKFILER_IO_VK_ICO_00         0xE4  //  00 key on ICO

#define BOOKFILER_IO_VK_PROCESSKEY     0xE5

#define BOOKFILER_IO_VK_ICO_CLEAR      0xE6

#define BOOKFILER_IO_VK_PACKET         0xE7

/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
#define BOOKFILER_IO_VK_OEM_RESET      0xE9
#define BOOKFILER_IO_VK_OEM_JUMP       0xEA
#define BOOKFILER_IO_VK_OEM_PA1        0xEB
#define BOOKFILER_IO_VK_OEM_PA2        0xEC
#define BOOKFILER_IO_VK_OEM_PA3        0xED
#define BOOKFILER_IO_VK_OEM_WSCTRL     0xEE
#define BOOKFILER_IO_VK_OEM_CUSEL      0xEF
#define BOOKFILER_IO_VK_OEM_ATTN       0xF0
#define BOOKFILER_IO_VK_OEM_FINISH     0xF1
#define BOOKFILER_IO_VK_OEM_COPY       0xF2
#define BOOKFILER_IO_VK_OEM_AUTO       0xF3
#define BOOKFILER_IO_VK_OEM_ENLW       0xF4
#define BOOKFILER_IO_VK_OEM_BACKTAB    0xF5

#define BOOKFILER_IO_VK_ATTN           0xF6
#define BOOKFILER_IO_VK_CRSEL          0xF7
#define BOOKFILER_IO_VK_EXSEL          0xF8
#define BOOKFILER_IO_VK_EREOF          0xF9
#define BOOKFILER_IO_VK_PLAY           0xFA
#define BOOKFILER_IO_VK_ZOOM           0xFB
#define BOOKFILER_IO_VK_NONAME         0xFC
#define BOOKFILER_IO_VK_PA1            0xFD
#define BOOKFILER_IO_VK_OEM_CLEAR      0xFE

#endif // end BOOKFILER_IO_VK_H

#ifndef BOOKFILER_WIDGET_DATA_H
#define BOOKFILER_WIDGET_DATA_H

// C++
#include <string>
#include <unordered_map>
#include <functional>
namespace bookfiler {

#ifndef BOOKFILER_WIDGET_H
#define BOOKFILER_WIDGET_H
class WidgetData {
public:
  int displaySizeX, displaySizeY, displayFramebufferScale, mousePosX, mousePosY,
      mouseWheelX, mouseWheelY;
  double deltaTime;
  std::string addInputCharacter;
  bool keyCtrl, keyShift, keyAlt, keySuper, windowFocused;
  std::unordered_map<int, bool> mouseDown;
  std::unordered_map<int, bool> keysDown;
  void (*setClipboardTextFn)(void *, const char *);
  const char *(*getClipboardTextFn)(void *);
  void *winId;
};

class WidgetMouseEvent {
public:
  int x, y;
};

class WidgetKeyEvent {
public:
  int x, y;
};
#endif // end BOOKFILER_WIDGET_H

} // namespace bookfiler
#endif // end BOOKFILER_WIDGET_DATA_H
