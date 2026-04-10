# Ghost Of Keputih
Final Project for Advanced Programming Course

This project serves as the final assignment for the Advanced Programming course, under the supervision of the presiding lecturer, "Dr. Eko Mulyanto Yuniarno, S.T., M.T.". The objective of this project is to develop a game using the C++ programming language with the Graphics.H library to demonstrate a comprehensive understanding of advanced programming concepts.

Name: Vinsen Dwi Putra
Student ID: 5024241094

## Daftar isi 
- [Overview](#overview)
- [Dependencies](#dependencies)
- [How to build and run Project](#how-build-dan-run-project)
- [Control](#control)
- [Result](#Result)

## Overview
This project is designed to implement 2D fighting game mechanics using the graphics.h library for visualization, with a primary focus on the application of data structures and algorithmic efficiency analysis in handling entity interactions. Through the development of this game, attack detection—specifically hitboxes and hit registration—is executed using a masking method to clarify hitbox functionality. By integrating game logic such as hitbox management and character states into a systematic code structure, this project serves as a practical medium for understanding how selecting the appropriate methods can overcome the technical limitations of a low-level graphics library to produce a responsive and engaging game.

## Dependencies

- Graphics.H
- C++17 

## How to build and run Project

Follow these steps to build and run the project:

1. Ensure you have a compiler that supports C++17 and that the SFML library is installed.
2. Clone this repository to your local machine.
3. Use the following command in your terminal to link SFML and compile the project:
   ```bash
   g++ GhostofKeputih.cpp -o GhostofKeputih -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -lwinmm

   ```
4. Sesudah build dan compile projectnya, jalankan simulasinya:
   ```bash
   ./GhostofKeputih
   ```

##  Control

| Tombol | Fungsi |
|--------|--------|
| **Left Arrow** | Move Left |
| **Right Arrow** | Move Right |
| **W** | Normal Attack |
| **W** | Special Attack |
| **SPACE** | Jump |

## Result

- Running Program

https://github.com/user-attachments/assets/72539501-9e1d-4385-9467-7691f6726468

