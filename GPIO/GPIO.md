# GPIO Driver

**Files:** `gpio.cpp` / `gpio.h`  
**What it does:** Low-level digital input/output for an AVR-style microcontroller. It lets the rest of the project configure a whole port or a single pin, write HIGH/LOW, and read pin levels.

---

## Registers Used

Each port `x` has three registers, accessed through pointer arrays declared in `gpio.h`:

| Register | Name | Purpose |
|---|---|---|
| `ddr[x]` | Data Direction Register | Chooses input or output for each pin (`1` = output, `0` = input) |
| `port[x]` | PORT register | Output level (HIGH/LOW). On an input pin it enables the internal pull-up. |
| `pin[x]` | PIN register | Reads the actual logic level on the pins |

---

## Functions

| Function | Description |
|---|---|
| `port_config(x, value)` | Writes `value` to the whole DDR register. Example: `0b00001111` = pins 0-3 outputs, pins 4-7 inputs. |
| `pin_config(x, i, value)` | Sets pin `i` as input or output without changing other pins. |
| `port_write(x, value)` | Writes all 8 output bits at once (overwrites the entire PORT register). |
| `pin_write(x, i, value)` | Sets pin `i` HIGH or LOW without changing other pins. |
| `port_read(x)` | Reads all 8 pins from the PIN register. |
| `pin_read(x, i)` | Reads one pin; returns `0` or `1`. |

---

## Bit Manipulation Explained

To change **one** pin without disturbing the others, the code uses bit masks.

| Operation | Code | Effect |
|---|---|---|
| Set bit `i` | `*reg \|= (1 << i);` | OR keeps all other bits, forces bit `i` to 1 |
| Clear bit `i` | `*reg &= ~(1 << i);` | AND with inverted mask forces bit `i` to 0 |
| Read bit `i` | `(*pin[x] >> i) & 1` | Shift bit `i` to position 0, mask everything else |

### Example: pin 3
```
1 << 3     = 0b00001000
Set:   REG |= 0b00001000      (only bit 3 becomes 1)
Clear: REG &= 0b11110111      (only bit 3 becomes 0)
```

---

## Notes

- **Output pin:** HIGH drives the pin toward VCC, LOW toward GND.
- **Input pin:** writing HIGH to the PORT bit enables the internal pull-up resistor instead of driving the pin.
- **`volatile`:** register pointers should be declared `volatile` in `gpio.h` so the compiler never removes hardware reads/writes.

## Limitations

- `gpio.cpp` does not include `gpio.h`, so `INPUT`, `OUTPUT`, `HIGH`, `LOW` and the exact port-to-register mapping cannot be verified from this file alone.
