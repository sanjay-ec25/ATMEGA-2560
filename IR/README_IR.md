# IR Sensor Driver — Occupancy and Available Slots

## 1. Purpose
`ir.cpp` reads one digital IR sensor output. It assumes the sensor is **active LOW**: a LOW input means an object/car is detected, and a HIGH input means the path is clear.

## 2. Functions
- `ir_init(port, pin)`: stores the port/pin and configures the pin as INPUT.
- `ir_is_occupied()`: returns `1` if `pin_read()` is LOW; otherwise returns `0`.
- `ir_get_available_slots()`: returns `0` slots free when occupied and `1` slot free when clear.

The return values are integer logic indicators, not a count of multiple parking spaces.

## 3. Logic table

| Electrical input | Assumed meaning | `ir_is_occupied()` | `ir_get_available_slots()` |
|---|---|---:|---:|
| LOW (`0`) | Object detected / occupied | 1 | 0 |
| HIGH (`1`) | Clear / not occupied | 0 | 1 |

The key expression is:
```c
return (pin_read(ir_port, sensor_pin) == 0) ? 1 : 0;
```
The conditional operator `condition ? A : B` returns `A` if the condition is true, otherwise `B`.

## 4. Calculations
There are no timer, prescaler, tick, or distance calculations in this driver. It only converts one digital input state into an occupancy result.

## 5. Assumptions to verify on the hardware
Some IR modules have adjustable thresholds and some output active HIGH instead. Confirm the module's output polarity with a simple test before the demonstration. The code's behavior is specifically active-LOW.

## 6. Short judge answer
> “The IR module is configured as a digital input. Because this driver assumes active-LOW output, a LOW signal means occupied and a HIGH signal means clear. The available-slot function reports either zero or one free slot for this single sensor.”
