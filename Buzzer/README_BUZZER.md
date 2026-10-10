# Buzzer Driver — Distance-Based Sound Pattern

## 1. Purpose
`buzzer.cpp` stores the selected port and pin, configures the pin as an output, starts it LOW, and changes the buzzer output based on a distance in centimetres.

## 2. GPIO setup
`buzzer_init(port, pin)`:
1. Saves the port and pin in static variables.
2. Calls `pin_config(..., OUTPUT)`.
3. Drives the output LOW initially.

`buzzer_set(state)` directly writes the requested state. `buzzer_update_pattern(distance_cm)` selects the sound pattern.

## 3. Distance logic exactly as coded

| Distance input | Selected behavior | Output timing |
|---|---|---|
| `distance_cm == 0` or `> 50` | SAFE / silent | LOW |
| `31–50 cm` | CAUTION / slow beep | HIGH 100 ms, then LOW 400 ms |
| `16–30 cm` | WARNING / faster beep | HIGH 100 ms, then LOW 100 ms |
| `15 cm` or less, excluding zero | STOP / continuous sound | HIGH |

The source checks `distance_cm > 30` after excluding values above 50, then checks `distance_cm > 15`. Thus 30 cm is in WARNING and 15 cm is in STOP.

## 4. Timing calculations

### CAUTION
- ON time = 100 ms
- OFF time = 400 ms
- Period = \(100+400=500\text{ ms}\)
- Frequency = \(1/0.5\text{ s}=2\text{ Hz}\)
- ON duty fraction = \(100/500=0.20=20\%\)

### WARNING
- ON time = 100 ms
- OFF time = 100 ms
- Period = \(100+100=200\text{ ms}\)
- Frequency = \(1/0.2\text{ s}=5\text{ Hz}\)
- ON duty fraction = \(100/200=0.50=50\%\)

### STOP
The output remains HIGH continuously while this branch is repeatedly selected. This is not a periodic beep, so a beep frequency/duty cycle does not apply.

## 5. Important behavior and limitations
- The zero-distance value is treated as “no valid reading / silent,” not as an object at zero centimetres.
- The delays use `_delay_ms()` from `<util/delay.h>`. Accurate compile-time delay generation normally requires the project to define the correct `F_CPU` before including the delay header.
- The function is blocking during the beep ON/OFF delays. While it is delaying, the CPU cannot proceed with other foreground tasks.
- This file uses GPIO functions, not hardware PWM. The buzzer receives simple HIGH/LOW patterns.

## 6. Short judge answer
> “The buzzer gives distance-dependent feedback. Beyond 50 cm it is silent; from 31 to 50 cm it beeps at 2 Hz with a 20% ON duty cycle; from 16 to 30 cm it beeps at 5 Hz with a 50% ON duty cycle; at 15 cm or less it stays continuously ON. A zero reading is treated as invalid/silent.”
