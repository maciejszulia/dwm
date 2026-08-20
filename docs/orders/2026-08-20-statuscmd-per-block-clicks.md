# Order — Integrate per-block status clicks (statuscmd) into this patched dwm

Repository: /home/ms/src/dwm (this worktree, current branch). Companion change in
/home/ms/src/dwmblocks happens in a SEPARATE later order — do not touch that repo.
Work MUST NOT: restart dwm or X, install the binary, or modify anything outside this repo.
Second brain: /home/ms/.local/share/silverbullet/space — read index.md, operating-model.md,
briefings/2026-08-12-omx-stack.md before starting (path only; do not paste their contents).

## Problem

Clicks on the status bar are global: config.h:164-165 binds ClkStatusText Button1 to
`weatherack` and Button2 to termcmd, regardless of which block was clicked. The goal
(owner backlog) is per-block click actions. This dwm is heavily patched locally
(commit 564a693): hover-folded status via STATUSSEP '\037' (dwm.c:59), rawstext
published by dwmblocks (dwm.c:249), 18 persistent tags. The upstream statuscmd patch
(dwm.suckless.org/patches/statuscmd/) will NOT apply cleanly — integrate its mechanism
by hand into the existing code.

## Required outcome

1. dwm identifies which status block was clicked from the click x-position and the
   CURRENTLY RENDERED status text — correct in BOTH hover states (folded text with
   STATUSSEP segments hidden, and unfolded on hover). A click on a hidden/folded
   segment must resolve to the block actually drawn under the cursor.
2. dwm signals dwmblocks (SIGRTMIN+signum of the clicked block, statuscmd convention:
   signal byte prefix in the status text or an equivalent mechanism consistent with
   the existing rawstext pipeline) and exports the button number (statuscmd BUTTON
   convention) so block commands can react per button.
3. Existing behaviour preserved: Button1 anywhere on the weather block still acks the
   weather warning (the current global weatherack binding is replaced by the per-block
   路 mechanism; keep a fallback binding only if some status area maps to no block).
4. Clean `make` (no install). config.def.h updated to match config.h changes.
5. OMX-REPORT.md at the worktree root: what was integrated, how block resolution works
   with the hover fold, and 2-3 concrete colour-scheme proposals to brighten the bar
   (values only, NOT implemented — owner picks one later).

## Contract

- Smallest integration that works; no refactor of unrelated patches; keep the local
  style of dwm.c. Signal numbers must match blocks.h in /home/ms/src/dwmblocks
  (read-only reference: VOL=10, BRI=11, weather=12; blocks without a signal get none).
- The separate later dwmblocks order will add BUTTON handling; design the dwm side to
  the standard statuscmd contract so that order needs no dwm rework.

## Verification (record exit codes in OMX-REPORT.md)

1. make clean && make → exit 0, no new warnings vs a baseline build of HEAD~0
2. grep -n the new click-resolution function in the report with a 5-line walkthrough
   of one folded and one unfolded click example.

## Constraints

No new dependencies. Diff surface: dwm.c, config.h, config.def.h, docs/orders/ (this
file), OMX-REPORT.md. English commits, no Co-Authored-By. Ponytail style.

## Definition of done

Branch with the integration committed, clean build, report present. Stop there: no
install, no dwm restart, no push, no dwmblocks changes.
