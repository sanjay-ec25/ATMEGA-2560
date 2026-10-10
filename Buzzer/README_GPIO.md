# GPIO Driver — Judge Explanation

## 1. Purpose
This module provides low-level digital I/O operations for an AVR-style microcontroller. It lets the rest of the project configure a whole port or an individual pin, write HIGH/LOW, and read the input state.

## 2. Registers used
The implementation accesses three register-pointer arrays declared elsewhere in `gpio.h`:

- `ddr[x]`: Data Direction Register for port `x`.
- `port[x]`: PORT register for port `x`.
- `pin[x]`: PIN input register for port `x`.

The exact port-to-register mapping is not shown in the supplied `gpio.cpp`; it depends on the definitions in `gpio.h`.

## 3. Functions and bit calculations

### `port_config(x, value)`
Writes `value` directly to the selected DDR register. Each bit controls one pin:
- `1` = output
- `0` = input

For example, `value = 0b00001111` configures pins 0–3 as outputs and pins 4–7 as inputs.

### `pin_config(x, i, value)`
Configures one bit without changing the other DDR bits.

**Set bit `i`:**
```c
*ddr[x] |= (1 << i);
```
`1 << i` creates a mask with only bit `i` set. OR preserves all other bits.

**Clear bit `i`:**
```c
*ddr[x] &= ~(1 << i);
```
The complement makes every bit 1 except bit `i`; AND clears only that bit.

Example for pin 3:
- `1 << 3 = 0b00001000`
- Set: `DDR |= 0b00001000`
- Clear: `DDR &= 0b11110111`

### `port_write(x, value)`
Writes all eight output-latch bits at once. This overwrites the entire selected PORT register, so it can affect every pin on that port.

### `pin_write(x, i, value)`
Sets or clears only the selected PORT bit:
```c
*port[x] |=  (1 << i);   // HIGH
*port[x] &= ~(1 << i);   // LOW
```
For an AVR GPIO output, HIGH generally drives the pin toward VCC and LOW toward GND. If the pin is configured as an input, writing the PORT bit typically controls its internal pull-up instead.

### `port_read(x)` and `pin_read(x, i)`
Read the PIN register, which reflects the sampled logic level at the physical pins. The single-pin expression:
```c
(*pin[x] >> i) & 1
```
shifts the chosen bit to bit 0, then masks away all other bits. The result is `0` or `1`.

## 4. What does `volatile` mean here?
The actual register pointers should be declared using `volatile` in the header so the compiler does not optimize away hardware register reads/writes. The supplied `.cpp` uses the pointers but does not show their declarations.

## 5. How to explain it to a judge
> “The GPIO layer hides direct register manipulation behind simple functions. DDR decides input versus output, PORT writes the output latch or enables a pull-up, and PIN reads the actual pin level. Bit masks let me change one pin without disturbing the other pins.”

## 6. Important source limitation
The supplied `gpio.cpp` does not include `gpio.h`, so the definitions of `INPUT`, `OUTPUT`, `HIGH`, `LOW`, and the `ddr`, `port`, and `pin` mappings cannot be verified from this file alone.
