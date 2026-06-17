[Firebrand-README.md](https://github.com/user-attachments/files/29030580/Firebrand-README.md)
# Firebrand

*A 2D squad survival-horror prototype — the game-focused successor to Opticus.*

Firebrand is what happened after [Opticus](https://github.com/oniontherock/Opticus): I took the same world out for another run, removed the non-Euclidean vision system (which was too heavy to build a game on), and focused on making an actual game.

## The game

You lead a squad of three soldiers through an apocalypse, trying to reach a safe zone by traveling a branching path that leads north. The world is meant to be hostile and atmospheric, populated by uncanny beings and monsters.

## What's implemented vs. planned

- **Working:** AI pathfinding and basic enemy behavior.
- **Designed and partly built:** a fully customizable weapon system, and a general-purpose procedural animation system driven by inverse kinematics. The IK animation work was started but not finished.

I've kept this honest rather than tidied up after the fact: it's a prototype where some systems run and others were still on the workbench.

## Tech

C++17 with SFML 3.0.0 (bundled in the repo) and a little GLSL, built as a Visual Studio solution.

## Status

An unfinished prototype. The pathfinding and core loop work; the larger systems — weapons, IK animation — were in progress when I set it aside.
