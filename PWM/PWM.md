# PWM Demo

**File:** `pwm.cpp`  
**What it does:** Turns a PWM output on at a set duty value for 1 second, then off for 1 second, repeatedly.

---

## Part 1: The Concept

### 1.1 What is PWM?
**PWM (Pulse Width Modulation)** switches a digital pin ON and OFF very quickly. By changing *how long* the pin stays ON in each cycle, we control the average power delivered to a device (LED brightness, motor speed, and so on).

```
     ON      OFF          ON      OFF
   +-----+         +   +-----+         +
   |     |         |   |     |         |
---+     +---------+---+     +---------+---
   |<------ one period ------>|
```

### 1.2 Key terms

| Term | Meaning |
|---|---|
| **Period (T)** | Time of one full ON + OFF cycle |
| **Frequency (f)** | Cycles per second: `f = 1 / T` |
| **Duty cycle (D)** | Percentage of the period the signal is ON |

### 1.3 Formulas

**Duty cycle**
```
D = (T_on / T) x 100 %
```

**Duty cycle from an 8-bit duty value**
```
D = (duty value / 255) x 100 %      (0 = 0 %, 255 = 100 %)
```
Some drivers divide by 256 instead of 255. The difference is tiny but depends on the driver's implementation.

**PWM frequency (common Fast PWM, TOP = 255)**
```
f_PWM = f_CPU / (N x (255 + 1))
```
where `N` is the timer prescaler.

---

## Part 2: The Calculation

### 2.1 Duty cycle for this program
The program uses a duty value of **128**.

```
D = 128 / 255 x 100 % = 50.2 %      (if scaled by 255)
D = 128 / 256 x 100 % = 50 %        (if scaled by 256)
```

So the output is ON for **about 50 %** of each PWM period.

### 2.2 PWM frequency (example only)
The frequency depends on the timer mode, TOP value and prescaler set in `pwm_init()`, which is **not part of this file**. As an illustration only, with 16 MHz CPU, prescaler 64 and TOP = 255:

```
f_PWM = 16,000,000 / (64 x 256) = 976.56 Hz   (about 977 Hz)
```

> Confirm the real values in your `pwm_init()` before quoting a frequency.

---

## Part 3: The Program

### 3.1 Code

```cpp
void setup() {
    timer_init();
    pwm_init();
}

void loop() {
    pwm_set_duty(128);
    timer_delay_ms(1000);
    pwm_stop();
    timer_delay_ms(1000);
}
```

### 3.2 Step-by-step

| Step | Call | Effect |
|---|---|---|
| 1 | `timer_init()` | Starts Timer0 so `timer_delay_ms()` works (see `Timer.md`). |
| 2 | `pwm_init()` | Configures the PWM hardware. |
| 3 | `pwm_set_duty(128)` | PWM runs at about 50 % duty. |
| 4 | `timer_delay_ms(1000)` | Waits about 1 second. |
| 5 | `pwm_stop()` | PWM output is turned off. |
| 6 | `timer_delay_ms(1000)` | Waits about 1 second. |
| 7 | | `loop()` repeats from step 3. |

**Resulting behaviour:** about 1 s of 50 % PWM, followed by about 1 s of silence/off, repeating forever.

---

## Limitations

- `pwm.h`, `pwm_init()`, `pwm_set_duty()` and `pwm_stop()` are not included in this file, so the exact resolution, frequency and duty scaling **cannot be confirmed from this file alone**.
- To finalise the calculations, check the files that define those functions (timer/channel used, prescaler, waveform mode, TOP value).
- The 1-second timings come from `timer_delay_ms()` and include a small software overhead.
