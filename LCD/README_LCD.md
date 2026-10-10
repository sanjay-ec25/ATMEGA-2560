# Character LCD Driver — HD44780 4-Bit Interface

## 1. Purpose
`lcd.cpp` drives a character LCD compatible with the HD44780 command interface using four data lines (`D4–D7`) plus `RS` and `EN`. It does not show an `RW` pin; this driver therefore assumes the LCD is wired for write-only operation (commonly `RW` tied LOW).

## 2. Pin roles
- `RS = 0`: the byte is a command.
- `RS = 1`: the byte is character/data.
- `EN`: a pulse tells the LCD to latch the nibble.
- `D4–D7`: transfer one 4-bit nibble at a time.

Each 8-bit command or character is sent as two nibbles: upper nibble first, then lower nibble. For example, `0x28` is sent as `0x02` followed by `0x08`.

## 3. Initialization commands
The source sends the HD44780 4-bit initialization sequence:
1. `0x03` nibble three times.
2. `0x02` nibble to enter 4-bit mode.
3. `0x28`: 4-bit interface, two-line mode, 5×8 font.
4. `0x0C`: display ON, cursor OFF.
5. `0x06`: cursor increments after each character.
6. `0x01`: clear display.

These hexadecimal values are command codes; there is no prescaler or timer tick calculation in this LCD driver.

## 4. Cursor addressing
`lcd_set_cursor(row, column)` chooses:
- Row 0 address = `0x00 + column`
- Any nonzero row address = `0x40 + column`

It then sends `0x80 | address`, where bit 7 is the “set DDRAM address” command bit. This is the common addressing scheme for a standard 16×2 HD44780-compatible LCD.

## 5. Number printing
`lcd_print_number(number)` extracts decimal digits with:
- `number % 10`: last digit
- `number / 10`: removes last digit using integer division

The digits are stored in reverse order and then emitted backwards. The special case for zero prints `'0'` directly.

## 6. Timing caution
`lcd_delay()` uses a loop from 0 to 999 with an empty body and a `volatile` loop variable. This prevents the loop from being entirely optimized away, but it does **not** guarantee a specific delay in microseconds. Its duration depends on CPU clock, compiler and optimization settings. The driver also delays after nibbles and commands, but those delays are not accurately quantified by the source.

## 7. Short judge answer
> “The LCD driver uses four-bit mode to save GPIO pins. It sends each byte in two nibbles, uses RS to distinguish commands from characters, and pulses EN to latch each nibble. Initialization configures a two-line display, turns the display on, sets cursor increment, and clears the screen.”
