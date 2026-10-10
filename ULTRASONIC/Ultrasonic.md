# Ultrasonic Distance Driver

**Files:** `ultra.cpp` / `ultra.h`  
**Microcontroller:** ATmega2560 @ 16 MHz  
**Sensor:** HC-SR04 type (TRIG and ECHO pins)  
**What it does:** Measures distance by timing a sound echo, using Timer1 as a stopwatch.

---

## Part 1: The Concept

### 1.1 How the sensor measures distance

1. The microcontroller sends a short HIGH pulse (about 10 us) on **TRIG**.
2. The sensor sends out a burst of ultrasound.
3. The sensor sets **ECHO** HIGH.
4. When the sound bounces back, the sensor sets **ECHO** LOW.
5. The time ECHO stayed HIGH is the round-trip time of the sound.

```
TRIG:  ___|‾‾|_______________________________
ECHO:  ______________|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|______
                      |<-- echo time -->|
```

### 1.2 Converting time to distance

Sound travels at about 343 m/s, which is 0.0343 cm per microsecond. The echo time covers the trip **out and back**, so the distance is half of the travelled path:

```
distance = (echo time x 0.0343) / 2
         = echo time / 58.3
```

The common shortcut used by the code is:

```
distance (cm) = echo time (us) / 58
```

### 1.3 Why Timer1?

Echo times are hundreds to thousands of microseconds, so we need a fast, accurate counter. Timer1 is a **16-bit** timer (counts 0 to 65535). It counts by itself in hardware while the CPU watches the ECHO pin.

---

## Part 2: The Calculation

**Given:** `f_CPU = 16 MHz`, `Prescaler = 8`

**Step 1 - Timer clock**
```
f_timer = 16,000,000 / 8 = 2,000,000 Hz = 2 MHz
```

**Step 2 - One tick**
```
T_tick = 1 / 2,000,000 = 0.5 us
```

**Step 3 - Ticks to echo time**
```
echo time (us) = elapsed ticks x 0.5
```

**Step 4 - Ticks to distance**
```
distance (cm) = (elapsed ticks x 0.5) / 58
              = elapsed ticks / 116
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

### Limits of the 16-bit counter
```
Wraps after   65,536 x 0.5 us = 32,768 us = 32.768 ms
Max distance  65,535 / 116    = about 564 cm
```
In practice the sensor itself is usually limited to about 400 cm.

### Summary of constants

| Quantity | Value |
|---|---|
| Timer clock | 2 MHz |
| One tick | 0.5 us |
| Ticks per cm | 116 |
| Counter wrap time | 32.768 ms |

---

## Part 3: The Program

### 3.1 Variables

```cpp
static uint8_t ultra_port;
static uint8_t trig_pin;
static uint8_t echo_pin;
```
These remember which port and pins the sensor uses. `static` keeps them private to this file.

### 3.2 Timer1 registers

```cpp
#define TCCR1A  (*(volatile uint8_t *)0x80)
#define TCCR1B  (*(volatile uint8_t *)0x81)
#define TCNT1   (*(volatile uint16_t *)0x84)
#define CS11 1
#define CS10 0
```

| Name | Purpose |
|---|---|
| `TCCR1A`, `TCCR1B` | Control registers (mode and prescaler) |
| `TCNT1` | The 16-bit counter (low byte `0x84`, high byte `0x85`) |
| `CS11` | Bit 1 of `TCCR1B`; setting it selects **prescaler 8** |

Each `#define` names a memory address, so writing `TCCR1B = ...` writes to the hardware register. `volatile` stops the compiler from optimizing away hardware reads and writes.

### 3.2.1 Timer1 register settings

| Register | Value | Meaning |
|---|---|---|
| `TCCR1A` | `0` | Normal mode (counts 0 to 65535, then wraps) |
| `TCCR1B` | `0` | No clock selected, timer stopped |
| `TCCR1B` | `(1 << CS11)` | Prescaler 8, timer starts |
| `TCNT1` | `0` | Clear the counter |

### 3.3 `ultra_init(port, trig, echo)`

| Line | What it does |
|---|---|
| `pin_config(..., trig_pin, OUTPUT)` | TRIG is an output (we send the pulse) |
| `pin_config(..., echo_pin, INPUT)` | ECHO is an input (we listen to the sensor) |
| `pin_write(..., trig_pin, LOW)` | TRIG starts LOW (idle) |
| `TCCR1A = 0; TCCR1B = 0; TCNT1 = 0;` | Timer1 prepared in normal mode but **stopped** |

### 3.4 `ultra_trigger()`

| Step | Code | Purpose |
|---|---|---|
| 1 | TRIG LOW, then 10 `nop` loops | Make sure the pulse starts from a clean LOW |
| 2 | TRIG HIGH, then 50 `nop` loops | Hold TRIG HIGH for the pulse (approx. 10 us) |
| 3 | TRIG LOW | End the pulse; the sensor now sends ultrasound |

`nop` means "do nothing for one clock cycle". The loop is only an approximate delay.

### 3.5 `ultra_get_distance_cm()`

| Step | Code | What happens |
|---|---|---|
| 1 | `ultra_trigger();` | Sends the TRIG pulse |
| 2 | `TCCR1B = (1 << CS11); TCNT1 = 0;` | Timer1 starts at 2 MHz from 0 |
| 3 | `while (ECHO == LOW)` with timeout | Wait for ECHO to go HIGH. On timeout: stop timer, return `0` |
| 4 | `start = TCNT1;` | Save the counter value when the echo begins |
| 5 | `while (ECHO == HIGH)` with timeout | Wait for ECHO to go LOW (sound returned). On timeout: stop timer, return `0` |
| 6 | `elapsed = TCNT1 - start;` | Echo length in ticks |
| 7 | `TCCR1B = 0;` | Stop Timer1 |
| 8 | `return elapsed / 116;` | Convert ticks to cm |

The subtraction `TCNT1 - start` is done in 16-bit unsigned arithmetic, so it still gives the right result if the counter wrapped once during the measurement.

### 3.6 `ultra_get_distance_mm()`

```cpp
distance_cm = ultra_get_distance_cm();
return distance_cm * 10;
```
Takes the whole-centimetre result and multiplies by 10. The value only changes in **10 mm steps**, so it is not true 1 mm resolution.

### 3.7 Whole process at a glance

| Step | Software | Hardware |
|---|---|---|
| 1 | `ultra_trigger()` | TRIG pulse of about 10 us |
| 2 | Start Timer1 | Counter increases every 0.5 us |
| 3 | Wait for ECHO HIGH | Sensor has sent the sound |
| 4 | `start = TCNT1` | Note the start time |
| 5 | Wait for ECHO LOW | Sound has returned |
| 6 | `elapsed = TCNT1 - start` | Echo time in ticks |
| 7 | `elapsed / 116` | Distance in cm |

---

## Output Notes

- Distance is returned in **whole centimetres**; fractions are truncated (10.9 cm reads as 10 cm).
- A return value of **`0` means timeout / no valid echo**, not a real distance of 0 cm.

---

## Limitations

- **Trigger pulse:** built from a `nop` loop with a `volatile` counter. It is not a calibrated 10 us delay, because compiler output and loop overhead change its real length. HC-SR04 sensors usually tolerate a pulse a little longer than 10 us.
- **Timeout:** `timeout = 60000` counts software loop passes, not Timer1 ticks or a guaranteed number of microseconds.
- **16-bit counter wrap:** the counter wraps after 32.768 ms. One wrap is handled by the unsigned subtraction, but an echo longer than 32.768 ms would give a wrong result.
- **Speed of sound:** the `/58` rule is an approximation; temperature changes the real value slightly.
- **Unused include:** `ultra.cpp` includes `timer.h` but does not use it.