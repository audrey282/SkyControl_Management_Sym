# SkyControl – Multi-Runway Management Simulator

## Overview

SkyControl is a comprehensive airport runway management simulator developed in C that models the operations of a major international airport with two runways. The system manages flight landings and takeoffs, handles emergency situations, adapts to traffic congestion, and maintains historical records for safety audits.

###  Key Features

- **Dual Runway System**: Runway Alpha and Bravo with dynamic assignment
- **Three Operational Modes**: Normal, Emergency, and Congestion modes
- **Priority-Based Queueing**: Emergency and low-fuel flights get priority
- **Undo Functionality**: Stack-based command history for error correction
- **Flight Archive**: BST-based storage with O(log n) search capability
- **Real-time Fuel Management**: Fuel decreases over time affecting priority
- **Interactive CLI**: User-friendly command-line interface

##  Architecture

├── flight.h/c # Flight data structure and operations
├── queue.h/c # Queue implementation (Landing/Takeoff queues)
├── stack.h/c # Stack implementation (Command history)
├── bst.h/c # Binary Search Tree (Flight archive)
├── runway.h/c # Runway management system
├── simulator.h/c # Core simulation logic
├── main.c # User interface and program entry

##  Usage Guide

========================================
✈️  SKYCONTROL - RUNWAY MANAGEMENT SYSTEM
========================================
1. Add New Flight
2. Process Next Time Step
3. Display Queues
4. Display Runway Status
5. Undo Last Assignment
6. Declare Emergency (Mayday)
7. Search Completed Flight
8. Display Statistics & Archive
9. Generate Random Flight
0. Exit System
----------------------------------------
