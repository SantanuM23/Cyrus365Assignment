# Game Demo - Unreal Engine 5.5

## Overview
This assignment contains a game demo developed in Unreal Engine 5.5. The game dynamically spawns boxes of different colors, sizes, and rotations based on data retrieved from an online-hosted JSON file. The player earns points by shooting the spawned boxes.

## Features
- Boxes are spawned dynamically with varying properties (color, size, and rotation) based on JSON data.
- The game fetches JSON data from an online source using HTTP requests.
- Shooting the boxes reduces their health, and they are destroyed upon reaching zero health.
- Player scores points based on successful hits.

## Code Structure

### `BoxActor.cpp`
- Handles the spawning of boxes.
- Initializes each box with properties retrieved from the JSON file.
- Sets box color and other attributes.
- Manages damage handling and destruction when health reaches zero.

### `GameMode.cpp`
- Sends an HTTP request to retrieve JSON data from an online source.
- Parses and formats the JSON data into a usable structure.
- Spawns the boxes in the game world based on the formatted data.

### `Projectile.cpp`
- Handles the behavior of projectiles fired from the player's gun.
- Applies damage to the boxes upon impact.

## Setup Instructions
1. Clone or download the project repository.
2. Open the project in Unreal Engine 5.5.
3. Ensure the internet connection is active for fetching JSON data.
4. Compile and run the project.
5. Shoot the boxes to score points.

## Dependencies
- Unreal Engine 5.5
- C++ (Unreal Engine API)
- Online JSON data source
