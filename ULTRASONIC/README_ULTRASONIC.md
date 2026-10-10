# Ultrasonic Distance Driver — Timer1 Tick Calculation

## 1. Purpose
`ultra.cpp` creates a trigger pulse, waits for the ultrasonic sensor's ECHO pulse, measures its HIGH duration using Timer1, and converts that duration to centimetres. The source identifies the MCU as an ATmega2560 and assumes a **16 MHz CPU clock**.

## 2. Timer1 setup
The code sets:
- `TCCR1A = 0`: normal waveform-generation mode.
- `TCCR1B = 0` during initialization, stopping the timer.
- `TCNT1 = 0`: clear the 16-bit counter before a measurement.
- During measurement, `TCCR1B = (1 << CS11)`: select Timer1 prescaler 8 on the ATmega2560.

## 3. Timer tick calculation

### Step 1 — Timer clock
\[
f_{timer}=\frac{16{,}000{,}000}{8}=2{,}000{,}000\text{ Hz}=2\text{ MHz}
\]

### Step 2 — Tick duration
\[
T_{tick}=\frac{1}{2{,}000{,}000}\text{ s}=0.5\ \mu s
\]

Each increment of Timer1 represents approximately 0.5 microseconds.

## 4. Convert echo ticks to distance

The source uses the common ultrasonic approximation:
\[
\text{distance in cm}=\frac{\text{echo pulse duration in }\mu s}{58}
\]

Since each tick is 0.5 µs:
\[
\text{echo duration in }\mu s=\text{elapsed ticks}\times0.5
\]
Therefore:
\[
\text{distance in cm}=\frac{\text{elapsed ticks}\times0.5}{58}
=\frac{\text{elapsed ticks}}{116}
\]

That is why the code returns:
```c
return (uint16_t)(elapsed / 116);
```

### Example
If the measured ECHO duration is 580 µs:
- Tick count = `580 / 0.5 = 1160 ticks`
- Distance = `1160 / 116 = 10 cm`

The integer return type truncates fractional centimetres. `ultra_get_distance_mm()` multiplies this already-rounded centimetre result by 10, so it returns millimetres in 10-mm increments, not true 1-mm resolution.

## 5. Measurement sequence
1. `ultra_trigger()` drives TRIG LOW, then HIGH briefly, then LOW.
2. Timer1 starts with prescaler 8 and the counter is cleared.
3. The code waits for ECHO to become HIGH. If a software loop counter reaches zero, it stops Timer1 and returns 0.
4. It records `start = TCNT1`.
5. It waits while ECHO remains HIGH, with another software timeout.
6. It computes `elapsed = TCNT1 - start`, stops Timer1, and converts ticks to centimetres.

## 6. Important accuracy limitations
- The trigger pulse uses a loop of `nop` instructions and comments that it is approximately 10 µs at 16 MHz. It is not a calibrated 10-µs delay: loop overhead and compiler output affect the duration.
- `timeout = 60000` counts software loop iterations, not Timer1 ticks or a guaranteed number of microseconds. The timeout duration depends on compiler and instruction timing.
- The result `0` means a timeout/no valid echo in this implementation; it should not be interpreted as a measured zero-centimetre distance.
- `elapsed` is 16-bit. At 0.5 µs per tick, the 16-bit counter wraps after \(65{,}536 \times 0.5\ \mu s = 32{,}768\ \mu s = 32.768\text{ ms}\). The code does not explicitly handle a wrap during the ECHO pulse.
- Distance depends on the speed of sound and environment; the `/58` rule is an approximation commonly used for ultrasonic modules.

## 7. Short judge answer
> “Timer1 runs at 2 MHz because the 16-MHz CPU clock is divided by 8. Each tick is 0.5 µs. The sensor distance approximation is echo time divided by 58, so converting time to ticks gives distance in centimetres as elapsed ticks divided by 116. The code polls ECHO and uses a software-loop timeout, so that timeout is not a precisely timed interval.”
