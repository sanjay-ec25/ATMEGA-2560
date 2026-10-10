# Ultrasonic Distance Driver

**File:** `ultra.cpp`  
**Microcontroller:** ATmega2560 @ 16 MHz  
**What it does:** Measures distance by sending an ultrasonic pulse and timing how long the echo takes to return, using Timer1.

---

## How It Works

1. The driver sends a short **TRIG** pulse.
2. The sensor emits ultrasound and raises **ECHO** HIGH.
3. ECHO stays HIGH until the sound bounces back.
4. The driver measures how long ECHO was HIGH and converts that time to distance.

---

## Timer1 Setup

| Register | Value | Purpose |
|---|---|---|
| `TCCR1A` | `0` | Normal mode |
| `TCCR1B` | `0` (init) | Timer stopped |
| `TCNT1` | `0` | Clear the 16-bit counter before measuring |
| `TCCR1B` | `(1 << CS11)` (measuring) | Prescaler **8** |

---

## Calculations

### Step 1 - Timer clock
```
f_timer = 16,000,000 / 8 = 2,000,000 Hz = 2 MHz
```

### Step 2 - Tick duration
```
T_tick = 1 / 2,000,000 = 0.5 us
```

### Step 3 - Ticks to time
```
echo time (us) = elapsed ticks x 0.5
```

### Step 4 - Time to distance
The common approximation for ultrasonic modules is:
```
distance (cm) = echo time (us) / 58
```
Combining with Step 3:
```
distance (cm) = (elapsed ticks x 0.5) / 58 = elapsed ticks / 116
```
This is why the code returns:
```c
return (uint16_t)(elapsed / 116);
```

### Worked example
```
Echo time = 580 us
Ticks     = 580 / 0.5 = 1160
Distance  = 1160 / 116 = 10 cm
```

---

## Measurement Sequence

| Step | Action |
|---|---|
| 1 | `ultra_trigger()` drives TRIG LOW, then HIGH briefly, then LOW. |
| 2 | Timer1 starts (prescaler 8) with the counter cleared. |
| 3 | Wait for ECHO to go HIGH. If the software timeout expires, stop Timer1 and return `0`. |
| 4 | Record `start = TCNT1`. |
| 5 | Wait while ECHO stays HIGH (with another software timeout). |
| 6 | `elapsed = TCNT1 - start`, stop Timer1, convert ticks to cm. |

---

## Output Notes

- Returns distance in **centimetres** (whole numbers; fractions are truncated).
- `ultra_get_distance_mm()` multiplies the centimetre value by 10, so it changes in **10 mm steps**, not true 1 mm resolution.
- A result of **`0` means timeout / no valid echo**, not a real distance of 0 cm.

---

## Limitations

- **Trigger pulse:** made with a loop of `nop` instructions described as about 10 us. It is not a calibrated delay; compiler output and loop overhead affect the real length.
- **Timeout:** `timeout = 60000` counts software loop iterations, not Timer1 ticks or guaranteed microseconds.
- **16-bit counter wrap:** Timer1 wraps after `65,536 x 0.5 us = 32.768 ms`. The code does not handle a wrap during the echo pulse.
- **Speed of sound:** the `/58` rule is an approximation; real distance varies with temperature and environment.
