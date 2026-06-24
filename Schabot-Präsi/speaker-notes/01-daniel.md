# Speaker script, Daniel S█████

You **open** the presentation and **close** it. You speak on 4 blocks:
**Intro + Vision (30 s)**, **Features (30 s)**, **3D Design (2 min)**, **Finances + Conclusion (30 s)**.

> Tip: press **S** in the browser to open the speaker view (these notes + timer + next slide).
> Your slides show a **gold** speaker dot.

---

## ① Slide 1, Title  ·  ~20 s
> *(Slide: animated Schachbot title)*

"Hi everyone, we are **Armin, Arthur and Daniel**, and this is **Schachbot**, our summer project.
In one sentence: **it's a chess board that moves its own pieces.**" The idea came from our shared love of chess. Back in 1997 **Deep Blue beat the world chesschampion**, that 'machine plays chess' moment stuck with me. We wanted to make it **physical**:
not a screen, but a **real board where the pieces slide to their squares on their own.**



## ② Slide 2, What it does  ·  ~30 s
> *(Slide: feature tiles + a 4-step "how one move works" flow)*

"So what can it do? **Three game modes**: classic **one-versus-one** with two keypads; **player
against the AI online**, where we send the position to the **Stockfish** engine; and an **offline
mode** so it still works without WiFi. It knows the **complete rules**, even castling and en
passant, and it catches illegal moves.

A single move is simple for the player *(walk along the 4 steps)*: you type the **start** and
**target** square, the **logic checks the rules**, and if it's legal the **gantry slides under the
piece**, then the **electromagnet grabs it** and drags it across. In AI mode the computer's reply plays itself.

That's the magic, now **Arthur** will show you the hardware behind it."

> 🔻 Hand over to **Arthur**.

---

## ④ Slides 14–17, 3D Design & Build  ·  ~2 min
> 🔺 You take over from **Armin**.

### Slide 14, Divider "3D Design & Build" · ~10 s
"Thanks Armin. All that software needs a **body** to move, and that was my main technical job:
designing and building the **2D gantry**, the casing, and the printed parts."

### Slide 15, The 2D gantry · ~45 s
"The gantry is the **mechanical heart**. Same principle as a pen-plotter or a 3D-printer: **two
stepper motors with toothed belts** move a carriage in **X and Y**, so the magnet can reach **any
of the 64 squares** from underneath. I designed it **completely from scratch in Oneshaped**, and **every
single** motor mount, idler, pulley bracket and the carriage is a **custom 3D-printed part I
modelled**. There's also a stiff transition plate linking the two axes. The first working gantry
was running in **under five weeks.**"

### Slide 16, Casing & playing field · ~35 s
"Around the gantry I designed all the **casing**: a two-part case that holds the **display** at a
comfortable angle for both players, an angled **holder for each keypad**, and **ten hollow pillars**
that raise a **plexiglass playing field** above the gantry, that gap is exactly where the magnet
travels. The casing and the clear field came out really clean once assembled."

### Slide 17, Precision & printing · ~30 s
"Precision was everything, if the squares don't line up, the magnet grabs the wrong piece. So I
started from the **pure geometry**: square size, frame, and the exact **steps-per-field** the motors
need. Designed in **Onshape and Fusion 360**, sliced in **Bambu Studio**, printed on a **Bambu Lab
A1**, some big parts took **over 7 hours** each. Getting the tolerances right was the hardest
mechanical lesson of the project."

---

## ⑤ Slide 18, Finances  ·  ~20 s
> *(Slide: KPIs + cost doughnut chart)*

"A quick word on money, since I managed the budget. In total we spent about **€414**. Our plan was
**€300**, so we landed roughly **38 % over**. The chart shows where it went, the **gantry mechanics
and motors** were the biggest chunks, then 3D-printing and the display. The honest lesson: **choose
your components carefully before buying**, a few wrong orders cost us extra money and time."

## ⑥ Slide 19, Conclusion  ·  ~15 s
> *(Slide: what went well / what we learned)*

"To wrap up, we're proud of a **working gantry in under five weeks**, a milled PCB a professor
called *beautiful*, a clean case, and solid software. We learned to **decide on the materials and
technology at the start**, to respect **mechanical tolerances**, that **magnet tuning** is tricky,
and that **clear team roles matter**.

**Thank you for listening!** We're happy to take questions, and afterwards, come play a game
against Schachbot."

---

### Timing summary (Daniel)
| Block | Slides | Target |
|---|---|---|
| Intro + Vision | 1 | 0:30 |
| Features | 2 | 0:30 |
| 3D Design | 14–17 | 2:00 |
| Finances + Conclusion | 18–19 | 0:30 |
| **Total** | | **~3:00** |
