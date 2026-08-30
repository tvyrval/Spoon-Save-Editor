# Spoon Save Editor

Spoon Save Editor is an application designed for editing Splatoon 1 `save.dat` files. Built entirely using the Qt libraries and C++. It offers a lightweight and direct approach to modifying player data, inventory, game progression, and meta-flags.

<img width="1040" height="746" alt="image" src="https://github.com/user-attachments/assets/352e64d5-8c0a-4e83-a74f-9c2464d65a6a" />


## Features

*   **Player Editor**: Modify player statistics (Level, Rank, Money, Super Sea Snails), appearance (Gender, Skin Color, Eye Color), Splatfest data, and control settings.
*   **Weapon Editor**: Manage your weapon inventory, edit turf inked per weapon, and unlock new weapons.
*   **Gear Editor**: Edit headgear, clothing, and shoes. Add new gear to your inventory or modify secondary abilities.
*   **Hero Mode**: Adjust single-player campaign progression, power eggs, hero suit upgrades, and meta-flags.
*   **Plaza Editor**: Modify the inklings that appear in your Inkopolis Plaza, including their names, gear, and stats. Supports filling the plaza with fake players.
*   **Amiibo Challenges**: Toggle completion states for Amiibo challenges across all figures (Inkling Girl, Inkling Boy, Inkling Squid).
*   **Minigames**: Unlock minigames (Squid Ball, Squid Racer, Squid Beatz) and edit high scores.
*   **Spyke Orders**: Manage and edit pending gear orders placed with Spyke.
*   **BitFlags**: Directly change raw game state flags or unlock all flags.

## System Requirements

*   **Operating System**: Windows, macOS or Linux
*   **Save File**: A valid Splatoon 1 save file (`.dat` format).

## Usage

1.  **Launch the Application**:
    *   **Windows**: Run `Spoon Save Editor.exe`.
    *   **Linux**: Make the `.AppImage` executable (`chmod +x Spoon_Save_Editor.AppImage`) and run it.
    *   **macOS**: When you run the application for the first time, you will get notified that macOS can't verify the application. This is because it isn't signed with a paid developer account. To use it, go to System Settings > Security & Privacy, scroll to the bottom and click Open Anyway.
2.  Click **Open File** and navigate to your Splatoon save file.
3.  Select your save file to load the data into the editor.
4.  Navigate through the tabs to modify your desired parameters.
5.  Click Save File to overwrite your changes, or use Save As... to save the changes to a new file.


## Settings

Preferences can be adjusted directly via the **Settings** menu:
*   **Show Internal IDs**: Displays the internal game ID numbers left to the item names.
*   **Sort Text Alphabetically**: Alphabetizes lists (such as gear and abilities) for easier navigation.
*   **Brick Save**: A joke option that replaces all save data bytes with zeros. Use with caution (or better, don't xD).

## Credits

* **Main Developer**: [@tvyrval](https://github.com/tvyrval) / vyrval
* **UI Developer**: [@JerrySM64](https://github.com/JerrySM64)
* **Contributor**: [@Winterberry1](https://github.com/c8ff)

## License

This project is licensed under the **[GNU General Public License v3.0 or later](LICENSE)**.
