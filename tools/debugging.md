## How to use the pretty printers for temperature formats while debugging
When debugging with gdb, the temperature formats cannot be easily inspected due to the templating and boost dependencies.
They are printed in the inspector as a deeply nested object.

gdb supports custom pretty printers to make the variables more readable.
To use the custom pretty printer for `temp_t`, `temp_long_t` and `temp_precise_t`:
- modify `pretty.gdbinit` to have the correct absolute path of the tools directory on your system
- set pretty.init as gdb init file in debugger settings   