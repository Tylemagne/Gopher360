Download
======

[![Github All Releases](https://img.shields.io/github/downloads/Tylemagne/Gopher360/total.svg?style=flat&label=Download%20Gopher360%20Standalone&logo=appveyor&colorA=00cc0a&colorB=000000)](https://github.com/Tylemagne/Gopher360/releases/download/v0.989/Gopher.exe)

Donate
======

[![Become a patron](https://c5.patreon.com/external/logo/become_a_patron_button@2x.png)](https://www.patreon.com/bePatron?u=1002457)

Compatible Controllers
======
### Xbox 360
* [Official Wired Xbox 360 Controller for Windows](https://www.amazon.com/Microsoft-Wired-Controller-Windows-Console/dp/B004QRKWLA/)
* [Official Wireless Xbox 360 Controller](https://www.amazon.com/Xbox-360-Wireless-Controller-Glossy-microsoft/dp/B003ZSP0WW/)
* [A compatible wireless adapter](https://www.amazon.com/Microsoft-Xbox-Wireless-Receiver-Windows/dp/B000HZFCT2/) is required to get a standard controller to connect to a Windows PC)

### Xbox One
* [Offical Xbox One Wireless Controller](https://www.amazon.com/Xbox-Wireless-Controller-Black-one/dp/B01LPZM7VI/)
* [A bluetooth adapter/antenna](https://www.amazon.com/Bluetooth-Wireless-Headphone-Controller-Keyboard/dp/B0774NZNGX/) is needed for XBone controller Bluetooth mode. Many laptops and tablets will *already* have this. Read the manual!
* [A wireless adapter](https://www.amazon.com/Microsoft-Xbox-Wireless-Adapter-Windows-one/dp/B00ZB7W4QU/) is needed if you want wireless mode.
* [A decently long MicroUSB cable](https://www.amazon.com/Charger-Durable-Charging-Smartphones-Motorola/dp/B06XZTK2JL/) will be needed for comfortable direct wired mode usage. This is the cheapest option if you already have the controller and no Bluetooth antenna!

### DualShock
DualShock controllers are great, but you NEED to emulate Xinput for Gopher to see and understand them. Fortunately, Xinput emulation is a very popular thing, as there are just as many people with DualShock controllers as there are Xbox controllers. Listings coming soon.

### Third party
SOME third party controllers will most likely work as well. I haven't seen one not work, but I'd imagine some wouldn't. Research before buying, Gopher expects native Xinput devices, so the controller should as well. I won't be listing any for now until I know what ones will work. If I can find one that does the job and saves you from Microsoft's extreme profit margins, I'll list it.

Feedback
======

[![Static](https://img.shields.io/badge/Feedback%20Survey-03-blue.svg?logo=eclipse)](https://docs.google.com/forms/d/e/1FAIpQLScbsasEPPPLY0utFSnowtZFAr_O60hAHvrLt7_wEHZ3pzY9AA/viewform)

Table of contents
=================

  * [About](#about-gopher)
  * [Default Controls](#default-controls)
  * [Requirements](#requirements)
  * [DualShock Controllers](#using-dualshock-controllers)
  * [Re-binding Controls](#config-file-instructions)
  * [Building](#build-instructions)
  * [License](#license)



About Gopher
======

Gopher is a utility for couch-oriented PC users that wish to entirely control their PC from the couch with a controller. Gopher works by transforming Xbox (or PlayStation, if using DS3Tool) controller input into traditional keyboard and mouse input that many applications and games still completely rely on with no controller-based alternative input options. The analog sticks move the mouse, the buttons click - it's very simple. Gopher completely skips this requirement and brings controller compatibility to ALL your applications and MOST of your games. Games like Runescape will be just fine. Crysis? Maybe not so much. It all depends on what amount of traditional input the game requires. Gopher is an excellent tool for PC gaming from the couch, as it's fully capable of web browsing, playing mouse-based games, controlling media players, and launching emulators. Don't stand up and waste calories, just download Gopher!

Gopher separates itself from the competition by being efficient, small, portable, free, and fully open. If you have something you'd like to see improved, added, or changed, please fill out the survey.

[![Github Releases (by Release)](https://img.shields.io/github/downloads/Tylemagne/Gopher360/v0.989/total.svg)]()

[![GitHub repo size in bytes](https://img.shields.io/github/repo-size/Tylemagne/Gopher360.svg)]()

[![GitHub last commit](https://img.shields.io/github/last-commit/Tylemagne/gopher360.svg)]()

[![GitHub top language](https://img.shields.io/github/languages/top/Tylemagne/gopher360.svg)]()

[![GitHub release](https://img.shields.io/github/release/Tylemagne/gopher360.svg)]()

[![Github commits (since latest release)](https://img.shields.io/github/commits-since/mike1084/GopherRenewed/latest.svg)]()

Requirements
======
Gopher is incredibly great at being a standalone program, but with one major exception: it absolutely needs Visual C++ 2015 Runtimes to be installed. If you have run Windows Updates at least once in the lifetime of your computer, this really won't be an issue.

Using DualShock Controllers
======
DualShock controllers don't use typical xinput libraries like the X360 and Xbone controllers do, so you'll need something like InputMapper, SCP, DS4Windows, or DS3Tool to "emulate" an xinput device in order to get xinput-using applications like Gopher360 to understand it. Gopher360 DOES NOT automatically offer these emulation layers ( yet ;) ), so you'll need to use something to emulate it before Gopher can understand it.

Video Demonstration
======

https://vine.co/v/MYadBgWXuWY

[![Gopher Video 1](http://img.youtube.com/vi/UWYUodeontM/0.jpg)](https://www.youtube.com/watch?v=UWYUodeontM)

[![Gopher Video 2](http://img.youtube.com/vi/8APmA1ohPdM/0.jpg)](https://www.youtube.com/watch?v=8APmA1ohPdM)


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
There is a configuration file (config.ini) that can be reconfigured for simple keybindings.

```diff
- IF YOU MESS SOMETHING UP, Gopher will automatically re-generate a config file.
```

You can set which controller buttons will activate the configuration events based on the official microsoft keys hexadecimal values.

Virtual Windows Keys:
https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731

XInput Controller Buttons:
https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad%28v=vs.85%29.aspx

More instruction in the configuration file.


```diff
+ If you make a config file you feel could benefit people with the same use scenario as you, feel free to make a pull request for it in the public configs directory.
```


Build Instructions
======
Building is pretty straightforward, but you may get a "missing win32 include" error due to the solution targetting. Simply follow the instructions the error provides (Project -> Retarget solution) to ensure your project has a working link to the libraries it needs.

License
======
Gopher free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see http://www.gnu.org/licenses/.




![Controller GIF](https://thumbs.gfycat.com/ElasticUnrulyBighorn-max-1mb.gif)
