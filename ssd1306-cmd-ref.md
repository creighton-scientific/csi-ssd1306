# SSD1306 Command Reference

Condensed from Solomon Systech SSD1306 Rev 1.1 (Apr 2008) Sections 9–10, plus the
Jan 2009 Application Note (charge pump). All commands are single-byte opcodes sent
with control byte `0x00` (D/C#=0); multi-byte commands are followed by their
argument bytes under the *same* control byte (Co=0 streams them together).

Note1:  Be sure to check section 9 tables against the section 10 descriptions. 
Note2:

## 1. Fundamental

| Hex   | Fundamental                   | Args                      | Purpose                                                                   | Reset default
|-------|-------------------------------|---------------------------|---------------------------------------------------------------------------|-------------
| `81h` | Set Contrast Control          | 1 byte: `A[7:0]` (0–255)  | Sets segment output current / brightness. Higher = brighter.              | `7Fh`
| `A4h` | Entire Display ON — resume    | —                         | Output follows GDDRAM content (normal operation).                         | active (RESET)
| `A5h` | Entire Display ON — force     | —                         | Forces all pixels on, ignoring GDDRAM. Useful for a hardware/wiring test. |
| `A6h` | Set Normal mid-display        | —                         | `1` bit in RAM = pixel ON.                                                | active (RESET)
| `A7h` | Set Inverse Display           | —                         | `0` bit in RAM = pixel ON (inverted).                                     | —
| `AEh` | Display OFF                   | —                         | Sleep mode. SEG/COM outputs go to VSS/high-Z.                             | active (RESET)
| `AFh` | Display ON                    | —                         | Normal operation. Must follow a valid init sequence + VCC stable.         | —

## 2. Addressing Setting

| Hex         | Name                            | Args                                  | Purpose                                                         | Notes
|-------------|---------------------------------|---------------------------------------|-----------------------------------------------------------------|-------------
| `00h`–`0Fh` | Set Lower Column Start Address  | opcode encodes `X[3:0]`               | Lower nibble of column start addr.                              | **Page Addressing Mode only.**
| `10h`–`1Fh` | Set Higher Column Start Address | opcode encodes `X[3:0]`               | Upper nibble of column start addr.                              | **Page Addressing Mode only.**
| `20h`       | Set Memory Addressing Mode      | 1 byte: `A[1:0]`                      | `00`=Horizontal, `01`=Vertical, `10`=Page (RESET), `11`=invalid | Governs pointer auto-increment behavior — see below.
| `21h`       | Set Column Address              | 2 bytes: start `A[6:0]`, end `B[6:0]` | Column window, range 0–127.                                     | **Horizontal/Vertical mode only.** Reset: start=0, end=127.
| `22h`       | Set Page Address                | 2 bytes: start `A[2:0]`, end `B[2:0]` | Page window, range 0–7.                                         | **Horizontal/Vertical mode only.** Reset: start=0, end=7.
| `B0h`–`B7h` | Set Page Start Address          | opcode encodes `X[2:0]`               | Selects active page 0–7.                                        | **Page Addressing Mode only.**

### Addressing mode pointer behavior (Section 10.1.3)

| Mode | After each byte write... | When column hits end... | When page hits end... |
|---|---|---|---|
| Page (`10b`, default) | column++ | wraps to column start; page **unchanged** — must reissue `B0h`–`B7h` manually | n/a |
| Horizontal (`00b`) | column++ | wraps to column start, page++ | wraps to page start too |
| Vertical (`01b`) | page++ | n/a | wraps to page start, column++ |

For a full page-major framebuffer flush (`fb[page][col]` streamed linearly), **Horizontal mode** is almost always what you want — it lets one continuous I2C data write walk the whole GDDRAM without you re-issuing address commands mid-stream.

## 3. Hardware Configuration (panel layout)

| Hex | Name | Args | Purpose | Reset default |
|---|---|---|---|---|
| `40h`–`7Fh` | Set Display Start Line | opcode encodes `X[5:0]` | Which GDDRAM row maps to COM0 (vertical pan of RAM vs. panel). Range 0–63. | `40h` (line 0) |
| `A0h` | Set Segment Re-map — normal | — | Column address 0 → SEG0. | active (RESET) |
| `A1h` | Set Segment Re-map — flipped | — | Column address 127 → SEG0 (horizontal mirror). | — |
| `A8h` | Set Multiplex Ratio | 1 byte: `A[5:0]` | MUX = N+1. Valid N: 15–63 (16–64 MUX). Values 0–14 invalid. | `3Fh` (63 → 64 MUX) |
| `C0h` | Set COM Scan Direction — normal | — | Scan COM0 → COM[N-1]. | active (RESET) |
| `C8h` | Set COM Scan Direction — remapped | — | Scan COM[N-1] → COM0 (vertical mirror). Takes effect immediately, even mid-display. | — |
| `D3h` | Set Display Offset | 1 byte: `A[5:0]` | Vertical shift of COM output by N rows (0–63). Distinct from Start Line — see Tables 10-1/10-2 for interaction. | `00h` |
| `DAh` | Set COM Pins HW Config | 1 byte: `A[5:4]` | `A[4]`: 0=sequential, 1=alternative COM config. `A[5]`: 0=disable, 1=enable L/R remap. | `12h` (alt config, no L/R remap) |

## 4. Timing & Driving Scheme

| Hex | Name | Args | Purpose | Reset default |
|---|---|---|---|---|
| `D5h` | Set Display Clock Divide / Osc Freq | 1 byte: `A[7:4]`=Fosc, `A[3:0]`=divide ratio−1 | Sets DCLK = Fosc / (divide ratio). Higher `A[7:4]` = higher Fosc. | `80h` (divide=1, Fosc mid-range) |
| `D9h` | Set Pre-charge Period | 1 byte: `A[7:4]`=Phase 2, `A[3:0]`=Phase 1 | DCLK counts for each charge phase (Section 8.6). 0 is invalid in either nibble. | `22h` (2/2 DCLKs) |
| `DBh` | Set VCOMH Deselect Level | 1 byte: `A[6:4]` | COM deselect voltage as fraction of VCC: `000b`≈0.65×, `010b`≈0.77× (RESET), `011b`≈0.83× | `20h` |
| `E3h` | NOP | — | No operation. | — |

## 5. Graphic Acceleration (Scrolling)

| Hex | Name | Args | Purpose |
|---|---|---|---|
| `26h` / `27h` | Continuous Horizontal Scroll Setup | 6 bytes (dummy, start page, speed, end page, dummy, dummy) | `26h`=scroll right, `27h`=scroll left, by 1 column per step. **Must deactivate (`2Eh`) before reissuing.** |
| `29h` / `2Ah` | Continuous Vertical + Horizontal Scroll Setup | 5 bytes (dummy, start page, speed, end page, vertical offset) | `29h`=right+vertical, `2Ah`=left+vertical. Vertical offset=0 behaves like pure horizontal scroll. |
| `2Eh` | Deactivate Scroll | — | Stops scrolling. **RAM must be rewritten afterward** — content may be corrupted. |
| `2Fh` | Activate Scroll | — | Starts scrolling using the most recently issued setup command's parameters. **RAM access (read/write) prohibited while active.** |
| `A3h` | Set Vertical Scroll Area | 2 bytes: top fixed rows `A[5:0]`, scroll rows `B[6:0]` | Defines which rows participate in vertical scroll vs. stay fixed. Constraint: `A+B ≤ MUX ratio`. |

## 6. Charge Pump (from Application Note, not main datasheet)

| Hex | Name | Args | Purpose |
|---|---|---|---|
| `8Dh` | Charge Pump Setting | 1 byte: bit 2 | `0`=disabled (RESET), `1`=enabled. **Required if your module has no external VCC supply** (most common breakout boards). |

Enable sequence per the App Note: `8Dh, 14h` then later `AFh` (Display ON). If VBAT/VCC are supplied externally instead, leave the charge pump disabled and wire VCC per Section 14's application diagram.

## Typical init order (Application Note Fig. 2, adapted)

```
AEh                  Display OFF
D5h, 80h             Clock divide / osc freq
A8h, 3Fh             Multiplex ratio = 64 (adjust for 32-row panels: 1Fh)
D3h, 00h             Display offset = 0
40h                  Display start line = 0
8Dh, 14h             Charge pump: enable
20h, 00h             Memory addressing: horizontal
A0h or A1h           Segment re-map (orientation-dependent)
C0h or C8h           COM scan direction (orientation-dependent)
DAh, 12h             COM pins hardware config
81h, 7Fh             Contrast
D9h, F1h             Pre-charge period
DBh, 20h             VCOMH deselect level
A4h                  Resume RAM content display
A6h                  Normal (non-inverted) display
AFh                  Display ON
```

## Description of Pre-charge period, Datasheet 8.6, 10.1.17. Also interactions of charge pump and Dclock
## From Claude.ai

The three-phase drive cycle (Section 8.6)

Each row's pixels go through this cycle every refresh:

Phase 1 — Discharge: the OLED pixel's residual charge from the previous frame's content is discharged, driving it back toward VSS. This clears out whatever voltage was left on the pixel's parasitic capacitance so the next phase starts from a known state.
Phase 2 — Pre-charge: the pixel is driven up toward its target voltage using a voltage source, as fast as the programmed period allows.
Phase 3 — Current drive: the driver switches from voltage-mode to a constant-current source, which is what actually produces light output proportional to the programmed contrast/current.

Command D9h sets the durations of Phase 1 and Phase 2, each independently, in units of DCLK ticks (1–15 each, 0 invalid, both reset to 2h) — Section 10.1.17 and the fuller Table 9-1 entry: A[3:0] = Phase 1 length, A[7:4] = Phase 2 length.

Why pre-charging (Phase 2) exists at all

The reason you can't just jump straight from Phase 1 (discharged, ~0V) into Phase 3 (constant current) is that an OLED pixel behaves like a capacitor in parallel with a diode. A constant-current source charging a capacitor from 0V doesn't reach a stable operating voltage instantly — it ramps up over time following the capacitor's charge curve, and how long that takes depends on the specific panel's capacitance (which varies by panel size, pixel size, and manufacturer). If Phase 3's fixed-length current-drive window were forced to do both "get the pixel up to its operating point" and "hold it there long enough to emit the right amount of light," you'd get inconsistent brightness — pixels wouldn't fully reach their target voltage before the row's time slot ends, especially at higher multiplex ratios where each row only gets a short slice of the frame period (recall the K term — Phase1 + Phase2 + Phase3 lengths — directly divides into your frame rate formula from Section 8.3).

Pre-charge (Phase 2) solves this by using a faster voltage-source drive to get the pixel close to its target voltage first, so Phase 3's current source only has a small remaining gap to cover — meaning Phase 3 can reliably reach true steady-state brightness within its fixed window, giving consistent, predictable light output row to row rather than a dim or uneven image.

Why you'd ever change it from reset default

The reset value (2h/2h, encoded as 0x22) is the datasheet's generic default, but the correct value is genuinely panel-dependent — a physically larger pixel or different OLED material has different capacitance, and thus needs a longer Phase 2 to reach target voltage in the same number of DCLKs. This is also tangled up with the charge pump: modules relying on the internal charge pump (rather than an external supplied VCC) commonly need a longer Phase 2 than the reset default, because the charge-pump-generated VCC settles to its operating voltage more gradually than a hard external supply would — hence you'll frequently see community-derived init sequences (including the D9h, 0xF1 I used in the reference table) departing from the 0x22 reset value specifically for charge-pump-enabled boards. That particular byte isn't from the Solomon Systech datasheet or App Note directly — it's an empirically-settled value that's become a de facto standard across hobbyist SSD1306 libraries for charge-pump modules, which is worth knowing if you ever need to explain why your init sequence departs from official reset defaults at that specific command.

If your display looks dim, washed out, or has visible ghosting/streaking from previous frame content bleeding into the next, Phase 1/Phase 2 lengths in D9h are one of the first things worth tuning — alongside DBh (VCOMH level) and D5h (clock/frame rate), since all three interact to determine how much time and voltage headroom each pixel actually gets per refresh.

##
