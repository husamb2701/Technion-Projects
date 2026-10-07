# MatamStory

Final implementation for Matam assignment 4.

## Overview
A text-based role-playing game implemented in C++, featuring a turn-based system with players, jobs, characters, and events (Encounter and Special Events).

## Structure
- **Core Entities:** `Player`, `Job`, and `Character` hierarchies.
- **Game Logic:** `MatamStory` class managing game rounds, turns, and the leaderboard.
- **Events System:** Polymorphic `Event` hierarchy (`Encounter`, `Pack`, `SolarEclipse`, `PotionMerchant`) managed by an `EventFactory`.
- **Utilities:** Printing functions provided by the course for standard output formatting.

## Tech Stack
- **Language:** C++17
- **Build System:** CMake
