# Timer Driver (Timer0)

**Files:** `timer.cpp` / `timer.h`  
**Microcontroller:** ATmega2560 @ 16 MHz  
**What it does:** Uses hardware Timer0 to create accurate millisecond delays.

---

## Part 1: The Concept

### 1.1 What is a timer?
A timer is a counter inside the microcontroller that increases by 1 on every "tick" of a clock, completely independent of the CPU's program. We can use it as a very accurate stopwatch.

### 1.2 Why do we need a prescaler?
The CPU clock is 16 MHz, which is too fast to count slow events (a tick every 0.0625 µs). A **prescaler** divides the clock so the timer counts more slowly.

```
Timer clock = CPU clock / Prescaler
```

### 1.3 What is CTC mode?
**CTC (Clear Timer on Compare match)** means:
1. The counter counts up from 0.
2. When it reaches the value stored in the compare register (`OCR0A`), a flag (`OCF0A`) is set.
3. The counter automatically resets to 0 and starts again.

This gives us a repeating event at a fixed, known interval.

### 1.4 Formulas

| Quantity | Formula |
|---|---|
| Timer clock | `f_timer = f_CPU / Prescaler` |
| One tick | `T_tick = 1 / f_timer` |
| Counts per cycle | `OCR0A + 1` (counter includes 0) |
| Compare-match interval | `T_match = (OCR0A + 1) x T_tick` |

---

## Part 2: The Calculation

**Given:** `f_CPU = 16 MHz`, `Prescaler = 64`, `OCR0A = 249`

**Step 1 - Timer clock**
```
f_timer = 16,000,000 / 64 = 250,000 Hz
```

**Step 2 - One tick**
```
T_tick = 1 / 250,000 = 4 us
```

**Step 3 - Counts until match** (counts from 0 to 249)
```
Counts = 249 + 1 = 250
```

**Step 4 - Time between matches**
```
T_match = 250 x 4 us = 1000 us = 1 ms
```

**Result:** the compare flag is set exactly once every **1 millisecond**.

---

## Part 3: The Program

### 3.1 Register settings

| Register | Value written | Meaning |
|---|---|---|
| `TCCR0A` | `WGM01 = 1` | CTC mode |
| `OCR0A` | `249` | Compare value (gives 1 ms) |
| `TCCR0B` | `(1<<CS01) \| (1<<CS00)` | Bits `CS02:CS00 = 011` -> prescaler **64** |
| `TIFR0` | write 1 to `OCF0A` | Clears the compare flag |

### 3.2 Functions

| Function | What it does |
|---|---|
| `timer_init()` | Sets CTC mode, clears `TCNT0`, loads `OCR0A = 249`, clears any old flag, starts the timer with prescaler 64. |
| `timer_delay_ms(ms)` | Repeats `ms` times: wait for `OCF0A = 1`, then clear it. Each pass is about 1 ms. |
| `timer_delay_us(us)` | Intended for microsecond delays (see known issue below). |

### 3.3 How `timer_delay_ms()` works

```
for each millisecond requested:
    wait until compare flag (OCF0A) is 1
    clear the flag by writing 1 to it
```

Example: `timer_delay_ms(1000)` waits for 1000 compare events, which is about **1 second** plus a little software overhead.

---

## Known Issues and Limitations

- **`timer_delay_us()` is not accurate.** Its comment says it works in 4 us steps, but it polls the same compare flag as the millisecond function, which only fires every **1 ms**. For example, `timer_delay_us(100)` loops 25 times and waits about **25 ms**, not 100 us. It needs redesigning (a shorter compare interval or a calibrated delay loop).
- **Blocking delays.** While waiting, the CPU cannot do any other work.
