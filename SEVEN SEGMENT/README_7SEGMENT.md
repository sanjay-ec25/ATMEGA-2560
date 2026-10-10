# Two-Digit 7-Segment Display — Multiplexing

## 1. Purpose
`two_7segment.cpp` displays a decimal number from 00 to 99 using a common-cathode segment-code table and two digit-select lines. The two digits share the segment bus, so the code lights one digit at a time in rapid succession.

## 2. Segment codes
The table stores the segment pattern for digits 0–9. Examples:
- `0` → `0x3F`
- `1` → `0x06`
- `2` → `0x5B`
- `8` → `0x7F`
- `9` → `0x6F`

These are bit patterns for the segment wiring convention assumed by the source. Confirm that the physical segment bit order matches the board wiring.

## 3. Splitting a number into digits
For a number from 0 to 99:
\[
\text{tens}=\left\lfloor\frac{\text{number}}{10}\right\rfloor
\]
\[
\text{units}=\text{number}\bmod10
\]

Example for 47:
- Tens = `47 / 10 = 4` (integer division)
- Units = `47 % 10 = 7`

If the input is greater than 99, the code replaces it with 0.

## 4. Multiplex refresh operation
Every call to `sevensegment2_refresh()`:
1. Turns both digit-select outputs LOW.
2. Writes the tens digit's segment pattern and enables digit 1, or writes the units pattern and enables digit 2.
3. Toggles `current_digit` so the next call selects the other digit.

The code therefore expects `sevensegment2_refresh()` to be called continuously. If the refresh call rate is too slow, flicker becomes visible. The source does not specify a fixed refresh interval, so an exact refresh frequency cannot be calculated from this file alone.

For example, if an external loop calls refresh 2000 times per second, each digit is selected about 1000 times per second, assuming calls alternate evenly. This is an example only, not a rate configured in this file.

## 5. Electrical notes
The source sets all segment-port pins as outputs and both digit-select pins as outputs. It initially drives both digit selects LOW and writes `0x00` to the segment port. The assumed active level of digit-select lines is LOW=OFF and HIGH=ON. Use suitable current-limiting resistors for the LED segments and verify the common-cathode wiring.

## 6. Short judge answer
> “I use multiplexing because both digits share the same eight-bit segment bus. I calculate tens with integer division and units with modulo 10, then rapidly enable one digit at a time with its segment pattern. Persistence of vision makes both digits appear illuminated together.”
