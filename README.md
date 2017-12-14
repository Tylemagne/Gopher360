#[Download Gopher 0.989](https://github.com/Tylemagne/Gopher360/releases/download/v0.989/Gopher.exe)

[![Github All Releases](https://img.shields.io/github/downloads/Tylemagne/Gopher360/total.svg)]()

About Gopher
======

Gopher is a utility for couch-oriented PC users that wish to entirely control their PC from the couch with a controller. Gopher works by transforming Xbox (or PlayStation, if using DS3Tool) controller input into traditional keyboard and mouse input that many applications and games still completely rely on with no controller-based alternative input options. The analog sticks move the mouse, the buttons click - it's very simple. Gopher completely skips this requirement and brings controller compatibility to ALL your applications and MOST of your games. Games like Runescape will be just fine. Crysis? Maybe not so much. It all depends on what amount of traditional input the game requires. Gopher is an excellent tool for PC gaming from the couch, as it's fully capable of web browsing, playing mouse-based games, controlling media players, and launching emulators. Don't stand up and waste calories, just download Gopher!

Gopher separates itself from the competition by being efficient, small, portable, free, and fully open.


Requirements
======
Gopher is incredibly great at being a standalone program, but with one major exception: it absolutely needs Visual C++ 2015 Runtimes to be installed. If you have run Windows Updates at least once in the lifetime of your computer, this really won't be an issue.

Using DualShock Controllers
======
DualShock controllers don't use typical xinput libraries like the X360 and Xbone controllers do, so you'll need something like InputMapper to "emulate" an xinput device in order to get xinput-using applications like Gopher360 to understand it. Gopher360 DOES NOT automatically offer these emulation layers, so you'll need to use something to emulate it.

Video Demonstration
======

https://vine.co/v/MYadBgWXuWY


Download Instructions
======
Download the latest release here: https://github.com/irungentoo/Gopher360/releases

I recommend that you copy it somewhere outside of the ZIP and make a shortcut to it. Adding it to your startup folder in your HTPC can make bootups a lot more convenient!

Default Controls
======
Gopher360 automatically generates a config file, which will contain documentation information on all input types and key bindings.

**A**: Left Mouse-Click.

**X**: Right Mouse-click.

**Y**: Hide terminal.

**B**: Enter.

**D-pad**: Arrow keys.

**Right Analog**: Scroll up/down.

**Right Analog Click**: F2.

**Left Analog**: Mouse.

**Left Analog Click**: Middle mouse click.

**Back**: Browser refresh

**Start**: Left Windows Key

**Start + Back**: Toggle. Useful for when you launch emulators or open Steam Big Picture mode. Press again to re-enable.

**Start + DPad Up**: Toggle gopher vibration setting.

**LBumper**: Browser previous

**RBumper**: Browser next

**LBumber + RBummper**: Cycle speed (x3)

**LTrigger**: Space

**RTrigger**: Backspace

Config file instructions
======
There is a configuration file config.ini for simple keybindings.

You can set which controller buttons will activate the configuration events based on the official microsoft keys hexadecimal values.

Virtual Windows Keys:
https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731

XInput Controller Buttons:
https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad%28v=vs.85%29.aspx

More instruction in the configuration file.

Port Progress
======
Linux: Not started.

Windows: Initial port complete.

OSX: Not started.



License
======
Gopher free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see http://www.gnu.org/licenses/.
