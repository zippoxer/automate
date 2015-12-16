# automate /unfinished
A Windows automation engine handing out automation power to Lua scripts.

## Features
* Lua scripting
* Read process' memory
* Sends key strokes and mouse actions **even when the window is minimized**!

### findProcess(name, cmpType)
Finds a process by name, sets it as default and returns it's handle.

*cmpType* defines how to match a process or window name:
  * 0 - equals
  * 1 - starts with
  * 2 - ends with
  * 3 - contains

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
