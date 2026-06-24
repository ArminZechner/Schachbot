# Schachbot — Presentation (reveal.js)

A talk about **Schachbot**, our summer project: a chess board that physically moves its own pieces
with a self-built 2D gantry, an ESP32, a hand-milled PCB, and the Stockfish chess engine.

## ▶ How to open it

**Easiest:** double-click **`index.html`** — it opens in your browser and runs **fully offline**
(reveal.js and Chart.js are bundled in `assets/vendor/`, no internet needed).

**If anything looks off** (some browsers are strict with `file://`), serve it locally instead:

```
cd presentation
npx serve .            # then open the printed http://localhost:… address
```
or, since Python is common:
```
python -m http.server 8000      # open http://localhost:8000
```

## ⌨ Controls during the talk

| Key | Action |
|---|---|
| **→ / Space** | next slide |
| **←** | back |
| **S** | **Speaker view** — notes + timer + next slide *(recommended)* |
| **F** | fullscreen |
| **O** or **Esc** | slide overview |
| **B** | black-out screen (pause) |
| **Ctrl/Cmd + F** | search the slides |

A small chip **bottom-left** shows **who is speaking** + their topic + target time.

## 🗣 Running order (~7.5 min)

1. **Daniel** — Intro + Vision + Features … 1:00
2. **Arthur** — Hardware … 2:00
3. **Armin** — The Program … 2:00
4. **Daniel** — 3D Design … 2:00
5. **Daniel** — Finances + Conclusion … 0:30
6. **Demo video** — full-screen, plays with sound (drag the bar to skip)

Each person's full script is in **`speaker-notes/`**
(`01-daniel.md`, `02-arthur.md`, `03-armin.md`). The same lines are also embedded in the
**speaker view** (press **S**).

## 📁 Structure

```
presentation/
├─ index.html              ← the deck
├─ assets/
│  ├─ css/schabot.css      ← custom chess/electronics theme
│  ├─ js/deck.js           ← reveal init, speaker chip, cost chart
│  ├─ img/                 ← photos, CAD renders, animated SVGs (title, gantry-path)
│  ├─ video/               ← demo_video.mp4 (the final full-screen demo slide)
│  └─ vendor/              ← reveal.js + Chart.js (bundled → runs offline, no install)
├─ speaker-notes/          ← one script per presenter
└─ package.json            ← provenance only; nothing to install to run the deck
```

> Notes: the deck shows **no real source code** (the architecture is explained in words), and all
> photos / CAD renders are from our own documentation. Everything needed to run is bundled in
> `assets/vendor/`, so there is **no `npm install` step**. The **final demo video plays with sound**
> when you reach the last slide — for that to work reliably, **serve the deck locally** (see above)
> rather than opening it from `file://`.
