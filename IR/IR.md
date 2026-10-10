# IR Sensor Driver

**File:** `ir.cpp`  
**What it does:** Reads one digital IR sensor and reports whether a car/object is detected (occupied) or the spot is clear.

The driver assumes the sensor is **active LOW**: LOW means an object is detected, HIGH means clear.

---

## Functions

| Function | Description |
|---|---|
| `ir_init(port, pin)` | Stores the port and pin and configures the pin as INPUT. |
| `ir_is_occupied()` | Returns `1` if the pin reads LOW, otherwise `0`. |
| `ir_get_available_slots()` | Returns `0` free slots when occupied, `1` when clear. |

> The return values are simple indicators for **one** sensor, not a count of multiple parking spaces.

---

## Logic Table

| Pin level | Meaning | `ir_is_occupied()` | `ir_get_available_slots()` |
|---|---|:---:|:---:|
| LOW (`0`) | Object detected / occupied | 1 | 0 |
| HIGH (`1`) | Clear / not occupied | 0 | 1 |

The key line:
```c
return (pin_read(ir_port, sensor_pin) == 0) ? 1 : 0;
```
The conditional operator `condition ? A : B` returns `A` if the condition is true, otherwise `B`.

---

## Calculations

None. There are no timer, prescaler or distance calculations; the driver only converts one digital input into an occupancy result.

## Things to Verify on Hardware

- Some IR modules output **active HIGH** instead. Test the module's polarity before use, because this code is specifically active-LOW.
- Some modules have an adjustable sensitivity threshold that affects detection range.
