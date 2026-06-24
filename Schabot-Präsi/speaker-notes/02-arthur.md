# Speaker script, Arthur P███████

Your block: **Hardware, 2 minutes** (slides 3–7). Your slides show a **blue** speaker dot.
You start right after Daniel's feature slide and hand over to **Armin**.

> Tip: press **S** for the speaker view with timer.

---

## Slide 3, Divider "The Hardware"  ·  ~10 s
> 🔺 You take over from **Daniel**.

"Thanks Daniel. I'm **Arthur**, and I took care of the **electronics**, the heart of the whole
system. I'll show you the **brain**, the **circuit board** I designed and milled myself, and the
building blocks that turn a move into motion."

## Slide 4, One brain controls everything  ·  ~30 s
"Everything is controlled by **one ESP32** microcontroller. I picked it because it's powerful,
cheap, and has **built-in WiFi**, which we need for the online AI (Stockfish). Around it sit the parts the
players interact with: **two 4×4 keypads** for entering moves, a **MOSFET** that switches the electromagnet,
and **stop buttons** the gantry uses to find its zero position. I'll come to the motors, drivers and display
in a moment. My job was to connect all of this **reliably on one board**."

## Slide 5, Self-designed, self-milled PCB  ·  ~35 s
> *(This is your highlight slide, schematic → routing → milled board.)*

"This is the part I'm **most proud of**. I drew the full **schematic in EasyEDA**, then turned it
into a **single-sided layout**, all the copper tracks on **one side**, which is much harder because
tracks aren't allowed to cross. We **milled the board ourselves** here at school. One of our
professors actually called the layout **'beautiful'**, and yes, we **hid a little chess king in the
copper** as a signature. Designing it taught me a lot; we even had to redesign it a couple of times
when we'd forgotten components."

## Slide 6, From 24 V to a moving piece  ·  ~30 s
"Quickly, the power and motion side. **Power**: a 24-volt supply powers the electromagnet, and step-down
converters drop it to **12 V for the motors** and **5 V for the ESP32 and display**. **Motor drivers**: two
stepper drivers turn the controller's pulses into precise step-and-direction signals. **12 V stepper motors**:
those drivers spin two 12-volt steppers that pull the toothed belts and move the gantry. **Display**: a
3.5-inch SPI screen that mirrors the board. I tested every one of these **individually** before putting them
on the final board."

## Slide 7, The magnet trick  ·  ~25 s
"And the clever bit, how pieces actually move. Each piece has a **small permanent magnet** in its
base. Under the board, the gantry carries an **electromagnet**; switch it on and the piece above
**sticks** to the board, so we can drag it across. The challenge was **tuning the strength**: strong
enough to pull the target piece, but **not** so strong it drags the neighbours. About five-by-five-centimetre
squares and a **small, precise coil** solved it.

That's the hardware, **Armin** will now explain the software that decides every move."

> 🔻 Hand over to **Armin**.

---

### Timing summary (Arthur)
| Slide | Topic | ~ |
|---|---|---|
| 3 | Intro / divider | 0:10 |
| 4 | ESP32 + system | 0:30 |
| 5 | The milled PCB | 0:35 |
| 6 | Building blocks | 0:30 |
| 7 | The magnet trick | 0:25 |
| **Total** | | **~2:10** |

> If you're short on time, trim slide 6 (building blocks), it's the most compressible.
