# OMX Report — Per-block status clicks

## Integrated result

- Added the signal-based statuscmd contract to `dwm.c` and bound status mouse buttons 1–9 in both `config.h` and `config.def.h`.
- `statussignal()` resolves the click against `stext`, the exact status variant currently rendered, rather than against the complete `rawstext` published by dwmblocks.
- Status signal bytes are zero-width controls alongside the existing colour controls. A matching signal byte opens and closes its clickable region; text outside a marked region resolves to no signal.
- `sigstatusbar()` finds the configured `dwmblocks` process and uses `sigqueue(pid, SIGRTMIN + statussig, ...)`. The queued integer is the mouse button number consumed by the standard dwmblocks statuscmd handler to export `BUTTON` for the block command.
- The read-only `blocks.h` signal assignments remain unchanged: VOL `10`, BRI `11`, weather `12`; blocks without a signal marker receive no click signal.
- Removed the global weather acknowledgement/rotation status bindings. With the companion dwmblocks change, Button1 over weather resolves to signal `12` with button value `1`, preserving weather acknowledgement without making other blocks trigger it.

## Hover-fold resolution

`setstext()` already makes the fold state explicit: folded mode copies only the text after `STATUSSEP`; unfolded mode concatenates the text on both sides while removing the separator. `statussignal()` starts at the same x-origin and measures the same control-delimited spans as `drawbar()` and `statuswidth()`.

Five-line click walkthrough:

1. Folded: `stext` contains only BAT, VOL, weather, and clock; all hidden pre-`STATUSSEP` spans are absent.
2. A click inside VOL advances across visible BAT text, opens raw signal byte `10`, and returns `10` while the cursor is inside the matching marked VOL span.
3. The repeated byte `10` closes VOL, so following unsigned text cannot inherit the volume action.
4. Unfolded: `stext` contains the former hidden spans followed directly by BAT, VOL, weather, and clock, and the status x-origin moves left by their rendered width.
5. A click inside BRI advances through the newly visible spans, opens raw signal byte `11`, and returns `11`; the same screen x would resolve against a different visible block after folding.

Required grep evidence:

```text
$ grep -n 'statussignal' dwm.c
220:static int statussignal(int x);
470:            statussig = statussignal(ev->x);
1837:statussignal(int x)
```

## Verification

- Baseline `make clean && make` on the unmodified branch -> exit `0`; warning lines: none.
- Final `make clean && make` -> exit `0`; warning lines: none (no new warnings versus baseline).
- `git diff --check` -> exit `0`.
- `command -v pidof` -> exit `0` (`/usr/bin/pidof`).
- `pidof -s dwmblocks` -> exit `0` (the configured process name resolves on this host; no signal was sent).
- `grep -n 'statussignal' dwm.c` -> exit `0`.

No install, dwm/X restart, push, or dwmblocks modification was performed.

## Bar colour proposals — values only, not implemented

| Proposal | Background | Foreground | Accent | Warning | Dim | Caution |
| --- | --- | --- | --- | --- | --- | --- |
| A | `#2B2D31` | `#F2F3F5` | `#2F83A5` | `#FF5C5C` | `#9AA0A6` | `#FFD166` |
| B | `#1B2333` | `#F5F7FA` | `#3B82B4` | `#FF6B6B` | `#94A3B8` | `#F6C453` |
| C | `#2D2926` | `#FFF7ED` | `#0F7C8C` | `#FF5A5F` | `#A8A29E` | `#F4C95D` |
