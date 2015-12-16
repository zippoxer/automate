# automate /unfinished
A Windows automation engine handing out automation power to Lua scripts.

## Features
* Lua scripting
* Read process' memory
* Sends key strokes and mouse actions **even when the window is minimized**!

## Example
```
findWindow("Chrome", 3)
leftClick(20, 20)
```
Switches to the first tab of your Chrome window.

## Functions

### findProcess(name, cmpType)
Finds a process by name, sets it as default and returns it's handle.

*cmpType* defines how to match a process or window name:
  * 0 - equals
  * 1 - starts with
  * 2 - ends with
  * 3 - contains

### findProcess([w])
Finds the process of the window *w* or the default window, sets the process as default
and returns it's handle.

### findWindow(name, cmpType)
Finds a window by name, sets it as default and returns it's handle.

### read([p,] address, what)
Reads a value of type *what* from the process *p* or the default process at *address*.

*what* is a string argument that sets the type to be read:
  * **i** - int, a signed 4 byte integer
  * **I** - uint, an unsigned 4 byte integer
  * **h** - short, a signed 2 byte integer
  * **H** - ushort, an unsigned 2 byte integer
  * **l** - long, a signed 8 byte integer
  * **L** - long, an unsigned 8 byte integer
  * **b** - unsigned byte
  * **c** - char, a signed byte
  * **s:n** - a string of **n** length

### typewrite([w,] text)
Sends *text* to the window *w* or the default window.

### keyDown([w,] keyCode)
Sends a key down action at the given key to the window *w* or the default window.

### keyUp([w,] keyCode)
Sends a key up action at the given key to the window *w* or the default window.

### leftClick([w,] x, y)
Sends a left click at *x*/*y* to the window *w* or the default window.

### rightClick([w,] x, y)
Sends a right click at *x*/*y* to the window *w* or the default window.

## Key Codes
* rbutton
* cancel
* mbutton
* back
* tab
* clear
* return
* shift
* control
* menu
* pause
* capital
* kana
* hangeul
* hangul
* junja
* final
* hanja
* kanji
* escape
* convert
* nonconvert
* accept
* modechange
* space
* prior
* next
* end
* home
* left
* up
* right
* down
* select
* print
* execute
* snapshot
* insert
* delete
* help
* lwin
* rwin
* apps
* sleep
* numpad0
* numpad1
* numpad2
* numpad3
* numpad4
* numpad5
* numpad6
* numpad7
* numpad8
* numpad9
* multiply
* add
* separator
* subtract
* decimal
* divide
* f1
* f2
* f3
* f4
* f5
* f6
* f7
* f8
* f9
* f10
* f11
* f12
* f13
* f14
* f15
* f16
* f17
* f18
* f19
* f20
* f21
* f22
* f23
* f24
* numlock
* scroll
* lshift
* rshift
* lcontrol
* rcontrol
* lmenu
* rmenu
* oem_1
* oem_2
* oem_3
* oem_4
* oem_5
* oem_6
* oem_7
* oem_8
* processkey
* attn
* crsel
* exsel
* ereof
* play
* zoom
* noname
* pa1
* oem_clear
* 0
* 1
* 2
* 3
* 4
* 5
* 6
* 7
* 8
* 9
