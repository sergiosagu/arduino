# arduino
My arduino sandbox space to play with:
- An Arduino UNO R3 board
- A Samsung 16LF01UA3 VFD Module (16-segment 16 characters)
- A Futaba M16MY02A VFD Module (14-segments 16 characters)
- A Noritake GU96x8M-K609C1 VFD Module (96x8 graphic dot matrix)

## clear
Clean-up project to reset the Arduino content.

## display_basic
First project to print a simple "hello world" on the VFD displays. It's compatible with the Samsung and the Futaba displays.

## display_effects
Project which provides few more sophisticated text effects for the VFD displays. It's compatible with the Samsung and the Futaba displays.
The text effects include basically:
- Print a message.
- Slide a text left-to-right and right-to-left.
- Blink a message.
- Fade-in, fade-out and blink with fade effect a message.
- "Crack" a mesage, like deciphering it.
- Print a text like in a terminal.

## display_effects_noritake
Project equivalent to display_effects but slightly adapted for the Noritake display.
