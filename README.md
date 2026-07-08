# 🎵 Blueberry Wolfz

A modern UI **desktop music player** built with **C++ and Qt Quick (QML)**, focused on performance, clean architecture, and building a responsive UI.

Blueberry Wolfz is built to explore how a modern desktop music player can be designed using a QML frontend and a C++ backend while maintaining clean separation between UI and application logic.

This project is developed as a **learning-driven application**, exploring how a music player is structured using a **QML frontend with a C++ backend**.

Learning how to design efficient data structures and application architecture with modern C++.

> ⚠️ Status: Early development (Work in Progress)

---

## ✨ Overview

Blueberry Wolfz Player aims to become a **lightweight and extensible local music player**, with a strong focus on:

* Declarative UI with QML
* Separation between UI and core logic
* Smooth and responsive user experience
* Scalable project architecture

---

## 🛠️ Tech Stack

* **Language:** C++
* **UI Framework:** Qt Quick (QML)
* **Backend:** Qt (Multimedia, Core)
* **Architecture:** QML (UI) + C++ (logic layer)

---

## 🚀 Current Features

* Basic audio playback
* Playlist management
* Volume control
* QML-based UI (in progress)

---

## 📁 Project Structure

```id="9r2hxm"
/src
  /ui          # QML UI (views, components)
  /control     # Controllers / business logic
  /data        # Models, player, playlist, metadata
/assets        # Images, icons
```

> Structure is evolving as the architecture becomes more defined.

---

## 🧠 Architecture

The project follows a **UI–Logic separation approach**:

* **QML Layer**

  * Handles UI rendering and interaction
  * Uses bindings and declarative design

* **C++ Layer**

  * Handles playback logic, data models, and state
  * Exposed to QML via properties and signals

This approach allows:

* Better maintainability
* Clear separation of concerns
* Easier UI iteration without touching core logic



---

## 🧭 Roadmap

### Core System

* [x] Basic playback engine
* [x] Playlist system
* [x] Volume control

### UI / UX

* [x] Initial QML UI
* [ ] UI redesign (YouTube Music-inspired)
* [ ] Animations & transitions refinement

### Features

* [ ] Metadata system (album art, tags)
* [ ] Playback modes (repeat, shuffle, history)
* [ ] Image caching (album covers)
* [x] Queue & history system

### Technical

* [ ] Optimize QML performance
* [ ] Improve C++ ↔ QML integration
* [ ] Resource & cache management

### Long-term

* [ ] Cross-platform polish (Windows / macOS / Linux)
* [ ] Plugin / extensibility system

---

## 🛠️ Prerequisites
* Qt 6.x (Core, Multimedia, QML)
* CMake 3.x
* C++17 Compiler

## 🚀 Build

Build instructions are not finalized yet.
They will be added once the project structure stabilizes.

---

## 📄 License

This project is licensed under the **MIT License**.

---

## 👤 Author

* GitHub: https://github.com/SoiConk

---

## 💬 Notes

This project is primarily for **learning and experimentation**, especially in:

* QML development
* UI architecture design
* Audio application design

The codebase may change frequently as new ideas are explored.
