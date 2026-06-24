# Speaker script, Armin Z██████

Your block: **The Program, 2 minutes** (slides 8–13). Your slides show a **green** speaker dot.
You start after Arthur and hand over to **Daniel**.

> Press **S** for the speaker view with timer.

---

## Slide 8, Divider "The Program"  ·  ~10 s
> 🔺 You take over from **Arthur**.

"Thanks Arthur. I'm **Armin**, the team leader, and I worked mostly on the **software**. Everything
the machine seems to *decide* lives in **1,837 lines of C++** on the ESP32. I'll show the
structure and the four interesting jobs the code does."

## Slide 9, Clean, separate modules  ·  ~30 s

"I split the program into **clean modules**, each with one job: **Chess** holds the rules and the
board, **Gantry** turns a move into motor steps, **Display** draws the board, **Keypad** reads the
matrices, **StockfishAPI** talks to the engine, **WiFi** manages the connection.

The trick is the **main loop**, it **never blocks**. Every pass it nudges the motors, keeps WiFi
alive, reads a key, and **only when a full move is entered** does it validate, apply, redraw, and in
AI mode play the answer. Non-blocking means the **motors stay smooth** while everything else runs."

## Slide 10, A complete rule-book  ·  ~30 s
"The biggest piece of software is the **rule engine**. For every move it checks: is there a piece on
the start square, is it **your colour**, does it obey that piece's **movement rule**, and crucially, 
would it leave **your own king in check**. On top of that it handles the special cases: **castling,
en passant and promotion**. If anything's wrong it returns a **clear reason**, and the display tells
the player to retry, or announces **check** and **checkmate**. Making it catch *every* illegal move
took the most testing of the whole project."

## Slide 11, Asking a grandmaster engine  ·  ~30 s
"For a genuinely strong opponent we don't compute moves ourselves, we ask **Stockfish**, one of the
best engines in the world. The key is **translation**: we convert our board into a **FEN string**,
the standard text format every engine understands *(point to the FEN example)*. We send it over
**HTTPS** at search **depth 15**, and it replies with the best move, like `e7e5`. **No internet?** We
fall back to a **random legal move**, so the game never stops."

## Slide 12, Turning a move into motion  ·  ~20 s

"Finally, how a move becomes **motion**. The code keeps **two boards**: the **target**, how it
should look, and the **real** one. It scans all 64 squares, finds the difference, and drags pieces
until they match. **Captures first**: the taken piece goes to a **graveyard** on the side before the
capturing piece moves in."

## Slide 13, Weaving between the pieces  ·  ~15 s
> *(Point to the animated weaving path on the right.)*

"And the neat detail: instead of pushing a piece straight through other squares, the
magnet steps **half a square onto the grid lines** and **weaves between the pieces**, so it never
knocks anything over. Calibrating those steps so the piece lands **dead-centre** took real effort.

That's the software, back to **Daniel** for the 3D design."

> 🔻 Hand over to **Daniel**.

---

### Timing summary (Armin)
| Slide | Topic | ~ |
|---|---|---|
| 8 | Intro / divider | 0:10 |
| 9 | Architecture / modules | 0:25 |
| 10 | Rule engine | 0:30 |
| 11 | Stockfish / FEN | 0:30 |
| 12 | Motion: diff & drag | 0:20 |
| 13 | Motion: weaving path | 0:15 |
| **Total** | | **~2:10** |

> If short on time, shorten slide 10, keep "check / checkmate + special moves" as the key point.
