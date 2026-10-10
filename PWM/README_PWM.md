# PWM Demo — What the Supplied File Actually Does

## 1. Purpose of this demo
The supplied `pwm.cpp` contains Arduino-style `setup()` and `loop()` functions. It initializes the timer and PWM modules, requests a PWM duty value of 128, waits for one second, stops PWM, and waits another second.

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

## 2. Duty-cycle calculation
The call uses the value **128**, but the supplied file does not include `pwm.h` or the implementation of `pwm_set_duty()`. Therefore, the PWM resolution and exact duty-cycle formula cannot be confirmed from the provided source.

If (and only if) `pwm_set_duty()` uses the common 8-bit convention where 0 means 0% and 255 means 100%, then:
\[
D=\frac{128}{255}\times100\%\approx50.20\%
\]
Some implementations instead divide by 256, giving:
\[
D=\frac{128}{256}\times100\%=50\%
\]
You should describe it as approximately 50% only after checking the missing PWM implementation.

## 3. Timing intended by `loop()`
- PWM is requested at duty value 128.
- `timer_delay_ms(1000)` intends to wait about 1 second.
- `pwm_stop()` disables/stops the PWM output according to the missing implementation.
- A second `timer_delay_ms(1000)` intends to leave the output off for about 1 second.
- The loop repeats.

**Important:** the supplied Timer0 code makes `timer_delay_ms()` approximately 1 ms per iteration, so the intended on/off intervals are approximately 1 second plus software overhead.

## 4. Frequency and prescaler
PWM frequency cannot be calculated responsibly from this file alone. It depends on the missing `pwm_init()` configuration: which hardware timer/channel is used, the timer prescaler, waveform-generation mode, and TOP value.

For a common fast-PWM timer with TOP = 255, the usual formula is:
\[
f_{PWM}=\frac{f_{CPU}}{N(255+1)}
\]
But do **not** present a numeric PWM frequency until you verify that your implementation uses that mode, TOP value, clock and prescaler.

## 5. What is missing to complete the calculation
To calculate exact PWM frequency and duty cycle, check/share:
- `pwm.h`
- the file that defines `pwm_init()`
- the file that defines `pwm_set_duty()`
- the file that defines `pwm_stop()`

## 6. Short judge answer
> “The demo initializes the timer and PWM, applies duty command 128 for about one second, stops PWM for about one second, and repeats. The source supplied here does not contain the PWM register setup, so the exact PWM frequency and duty-cycle scaling must be confirmed from the PWM driver.”
