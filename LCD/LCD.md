# Character LCD Driver (HD44780, 4-bit mode)

**File:** `lcd.cpp`  
**What it does:** Drives a 16x2-style HD44780-compatible character LCD using only 6 GPIO lines.

---

## Pin Roles

| Pin | Purpose |
|---|---|
| `RS` | `0` = command, `1` = character data |
| `EN` | Enable: a pulse makes the LCD latch the nibble on the data lines |
| `D4-D7` | Four data lines; carry one 4-bit nibble at a time |
| `RW` | Not used by the driver; assumed wired LOW (write-only) |

Using 4 data lines instead of 8 saves GPIO pins.

---

## How a Byte Is Sent

Each 8-bit command or character is sent as **two nibbles**: upper nibble first, then lower nibble, each latched by an EN pulse.

Example: `0x28` is sent as `0x02` followed by `0x08`.

---

## Initialization Sequence

| Step | Value | Meaning |
|---|---|---|
| 1 | `0x03` (x3) | Reset sequence |
| 2 | `0x02` | Switch to 4-bit mode |
| 3 | `0x28` | 4-bit interface, 2 lines, 5x8 font |
| 4 | `0x0C` | Display ON, cursor OFF |
| 5 | `0x06` | Cursor moves right after each character |
| 6 | `0x01` | Clear display |

These are command codes, so there are no prescaler or timer calculations in this driver.

---

## Cursor Addressing

`lcd_set_cursor(row, column)`:

| Row | Start address |
|---|---|
| 0 | `0x00 + column` |
| Any nonzero row | `0x40 + column` |

It then sends `0x80 | address`. Bit 7 (`0x80`) is the "set DDRAM address" command. This matches a standard 16x2 HD44780 LCD.

---

## Printing Numbers

`lcd_print_number(number)` extracts digits with integer maths:

| Operation | Result |
|---|---|
| `number % 10` | Last digit |
| `number / 10` | Removes the last digit |

Digits are collected in reverse order and printed backwards. A value of `0` is handled as a special case and printed directly as `'0'`.

**Example (253):** digits extracted 3, 5, 2, printed as `2`, `5`, `3`.

---

## Timing Caution

`lcd_delay()` is a loop counting 0 to 999 with a `volatile` counter. The `volatile` keyword stops the compiler from removing it, but the real delay is **not guaranteed**: it depends on CPU clock, compiler and optimization settings. The delays after nibbles and commands are likewise not precisely quantified.
