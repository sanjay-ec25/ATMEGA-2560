# Buzzer Driver

**File:** `buzzer.cpp`  
**What it does:** Gives distance-based audio feedback by switching a buzzer pin HIGH/LOW in different patterns.

---

## Functions

| Function | Description |
|---|---|
| `buzzer_init(port, pin)` | Saves the port/pin, configures the pin as OUTPUT, starts LOW. |
| `buzzer_set(state)` | Directly writes HIGH or LOW to the buzzer. |
| `buzzer_update_pattern(distance_cm)` | Chooses the sound pattern from the distance. |

This driver uses plain GPIO HIGH/LOW, **not** hardware PWM.

---

## Distance Logic

| Distance | Mode | Output |
|---|---|---|
| `0` or above `50 cm` | SAFE | Silent (LOW) |
| `31 - 50 cm` | CAUTION | Slow beep: HIGH 100 ms, LOW 400 ms |
| `16 - 30 cm` | WARNING | Faster beep: HIGH 100 ms, LOW 100 ms |
| `15 cm or less` (not 0) | STOP | Continuous sound (HIGH) |

Boundary behaviour: 30 cm counts as WARNING, and 15 cm counts as STOP. A distance of `0` means "no valid reading", not an object at 0 cm.

---

## Timing Calculations

### CAUTION
```
Period    = 100 + 400 = 500 ms
Frequency = 1 / 0.5 s = 2 Hz
Duty      = 100 / 500 = 20 %
```

### WARNING
```
Period    = 100 + 100 = 200 ms
Frequency = 1 / 0.2 s = 5 Hz
Duty      = 100 / 200 = 50 %
```

### STOP
The output stays HIGH continuously, so it is not a periodic beep and no frequency or duty cycle applies.

### Summary

| Mode | ON | OFF | Period | Frequency | Duty |
|---|---:|---:|---:|---:|---:|
| CAUTION | 100 ms | 400 ms | 500 ms | 2 Hz | 20 % |
| WARNING | 100 ms | 100 ms | 200 ms | 5 Hz | 50 % |
| STOP | continuous | - | - | - | 100 % |

---

## Limitations

- Delays use `_delay_ms()` from `<util/delay.h>`, which needs the correct `F_CPU` defined before the header is included.
- The beep delays are **blocking**: the CPU cannot do other foreground work while waiting.
