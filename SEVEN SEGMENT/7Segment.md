# Two-Digit 7-Segment Display

**File:** `two_7segment.cpp`  
**What it does:** Shows a number from 00 to 99 on two common-cathode 7-segment digits that share one segment bus.

---

## Concept: Multiplexing

Both digits share the same 8 segment lines, so they cannot show different numbers at the same time. Instead, the code lights **one digit at a time, very quickly**. Because of persistence of vision, both digits appear to be lit together.

---

## Segment Codes

A lookup table stores the segment pattern for each digit 0-9. Examples:

| Digit | Code |
|:---:|:---:|
| 0 | `0x3F` |
| 1 | `0x06` |
| 2 | `0x5B` |
| 8 | `0x7F` |
| 9 | `0x6F` |

These assume a specific segment wiring order. Confirm it matches your board.

---

## Splitting a Number into Digits

```
tens  = number / 10      (integer division)
units = number % 10      (remainder)
```

**Example (47):**
```
tens  = 47 / 10 = 4
units = 47 % 10 = 7
```
If the input is greater than 99, the code replaces it with 0.

---

## Refresh Operation

Each call to `sevensegment2_refresh()`:

| Step | Action |
|---|---|
| 1 | Turn both digit-select outputs LOW (both digits off) |
| 2 | Output the segment pattern for the current digit (tens or units) and enable that digit |
| 3 | Toggle `current_digit` so the next call shows the other digit |

The function must be **called continuously**. If it is called too slowly, the display flickers. The refresh rate is not set in this file.

*Example only:* if a loop calls refresh 2000 times per second, each digit is shown about 1000 times per second.

---

## Electrical Notes

- Segment-port pins and both digit-select pins are set as outputs.
- At start, both digit selects are LOW and the segment port is `0x00`.
- Digit-select levels: **LOW = off, HIGH = on**.
- Use suitable current-limiting resistors on the LED segments and verify the common-cathode wiring.
