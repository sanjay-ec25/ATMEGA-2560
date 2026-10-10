# Timer0 Driver — Prescaler, Tick and Delay Calculations

## 1. Purpose
The supplied `timer.cpp` configures **Timer0 of an ATmega2560** in Clear Timer on Compare (CTC) mode. `timer_delay_ms()` and `timer_delay_us()` poll the Output Compare A flag to make blocking delays.

The source comment states a CPU clock of **16 MHz**.

## 2. Timer configuration
- Mode: CTC, selected by setting `WGM01 = 1` in `TCCR0A`.
- Compare register: `OCR0A = 249`.
- Clock select: code writes `(1 << CS01) | (1 << CS00)` to `TCCR0B`.
- On the ATmega2560, Timer0 clock-select bits `CS02:CS00 = 011` mean **prescaler 64**. The source defines `CS01` as bit position 1 and `CS00` as bit position 0, so this write produces `0b011`.

## 3. Full calculation

### Step 1 — Timer clock after prescaler
\[
f_{timer}=\frac{f_{CPU}}{N}=\frac{16{,}000{,}000}{64}=250{,}000\text{ Hz}
\]

### Step 2 — One timer tick
\[
T_{tick}=\frac{1}{f_{timer}}=\frac{1}{250{,}000}\text{ s}=4\ \mu s
\]

### Step 3 — Counts to compare match
The counter counts from 0 through `OCR0A = 249`, inclusive:
\[
\text{counts}=249+1=250
\]

### Step 4 — Compare-match interval
\[
T_{match}=250\times4\ \mu s=1000\ \mu s=1\text{ ms}
\]

So the intended result is approximately one compare match per millisecond.

## 4. How the delay functions work

### `timer_init()`
1. Sets CTC mode.
2. Sets normal port operation for the compare-output pins.
3. Clears `TCNT0`.
4. Loads `OCR0A = 249`.
5. Clears a pending compare flag by writing a 1 to `OCF0A` in `TIFR0` (AVR flags are commonly cleared this way).
6. Starts Timer0 with prescaler 64.

### `timer_delay_ms(ms)`
Repeats `ms` times:
1. Waits until `OCF0A` becomes 1.
2. Writes 1 to `OCF0A` to clear the flag.
Each iteration waits for one compare event, intended to represent 1 ms. Therefore, `timer_delay_ms(1000)` intends a delay of about 1000 ms (1 second), plus software overhead.

### `timer_delay_us(us)`
The loop runs `us / 4` times because each compare event is intended to be 4? **Correction: each compare event is 1 ms, not 4 µs.** The code comments say this function works in 4 µs steps, but the implementation polls the same `OCR0A = 249` compare flag, which occurs every 1 ms. Therefore, as written, this function does **not** produce 4-µs steps. For example, `timer_delay_us(100)` loops 25 times and waits for roughly 25 ms, not 100 µs.

## 5. Important code issue to tell the judge
`timer_delay_us()` is inconsistent with the Timer0 setup. `OCR0A = 249` creates a 1-ms compare interval, so the microsecond function's comment and calculation are incorrect. It should be redesigned (for example, use a dedicated shorter timer interval or a cycle-calibrated delay) before claiming accurate microsecond delays.

Also, polling is a **blocking** approach: the CPU cannot do other work while waiting.

## 6. Short judge answer
> “At 16 MHz with a divide-by-64 prescaler, Timer0 runs at 250 kHz, so each timer tick is 4 microseconds. CTC counts 250 ticks from 0 to 249, giving a 1-ms compare event. The millisecond delay waits for that event repeatedly. The current microsecond helper needs correction because it polls the same 1-ms event.”
