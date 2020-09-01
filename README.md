Heroes 3.5: In the Wake of Gods 3.59 alpha.
===

WoG Legacy docs
---
Most important features: https://drive.google.com/file/d/1QxOKFQ6G0NerG8xEb-2iGE-DhmcAEtQO/view

Building from source tutorial: https://drive.google.com/open?id=1gpGqdj4BOo-crjxbtmoPp9r81cZDBBEZ&fbclid=IwAR0P7ZD2XBeMHBLDx41OMqfhkhBqC-8bxE02asWiw_z0dnfyXvCKCOJgqOc

Working with ERM Parser: https://drive.google.com/file/d/1sGtK0E1DeOZMMu59FtbJzi_TIYMhgUAT/view?usp=sharing

Map objects, game memory: https://drive.google.com/file/d/1T0sYWGM__jq1Hx8ZMoL8iPjJcE5vTwUa/view?usp=sharing

Always up-to-date ERM/Lua Help: https://azethmeron.github.io

(Polish) ERM tutorial: https://drive.google.com/open?id=1L8I4NZUse8CTkazf7BCal9WEhZPrGJPQ&fbclid=IwAR10yjwbRjQhgGfTDaNN34BVghvbe1zJlyVLDTRrKCsG-jMrE-t1bEIb1Ls

About
---

This is Alpha, so anything may change in the future.  
If you want to contact me, you can do it via Discord group: https://discord.gg/5HMkZDr

WoG 3.59 is going to be a platform hosting different mods, somewhat like Era II in that sense, but different.  
Alpha version executable is called WogT1.exe for historic reasons. Executable of final version will be called h3wog.exe as usual.
WogDialogs.dll source code can be found in MMExtension source code archive: https://www.dropbox.com/s/91xvgzbchx3h1bc/MMExtensionSrc.rar?dl=1

Installing a release
---

Extract contents to WoG home folder. Using a folder with clean WoG 3.58 install is preferable. Make sure Data folder doesn't contain these files:  
ARTEVENT.TXT  
ARTRAITS.TXT  
CRANIM.txt  
CRTRAITS.TXT  
SpTraits.txt  
ZCRTRAIT.TXT  
ZELP.TXT  

Note: If you're building WoG from sources, the only things you'll need from a release is Data\p\ folder and dbghelp.dll file, plus you must get rid of text files mentioned above.

Testing
---

'info' folder contains descriptions of new 3.59 stuff (and t1.map file for my own debugging convenience). Mods folder contains all scripts, Data\s\ isn't used anymore.  
An intermediate version of ERM help is available at https://www.dropbox.com/s/uxoqylpuyh96iei/erm_help_1_Mar_2006.zip?dl=1  
Currently no-one is taking care of it.

Saved games from 3.58 are incompatible. From one version of 3.59a to another saves compatibility may break as well.

If WoG happens to hang, don't kill the process. Instead, use this program: https://www.dropbox.com/s/zhf5sdq2e0e7m9t/WindowMan.rar?dl=1  
Run it, then switch to WoG window and switch back to the program. It should pick up the window name of WoG. When you see it's correct, press the "Crash" button. This will make WoG create WOGCRASHDUMP.DMP, WOGCRASHLOG.TXT and WOGERMLOG.TXT files. The program itself will create CrashEIP.txt file in its own folder. Send me all 4 files.

In addition to sending WOGCRASHDUMP.DMP, WOGCRASHLOG.TXT and WOGERMLOG.TXT don't forget to describe what exactly you did that lead to the issue and send the saved game and map if they're relevant.

Legacy note: all LOG files have been moved to woglogs/ directory. It's best to send whole folder once you face any errors, then remove this directory, 

At some point you may find UnclosedStackLevels.txt file in your WoG folder. Send it to me if you do.

Also, setting No32Bit=0 in [Common] of WoG.ini will let you run WoG windowed in 32-bit screen mode. However, 10-60 minutes into the game it will hang. That's ok, 32 bit mode code is temporary. Baratorch's HD Mod does it much better, so it should be the base of future proper 32 bit mode support.

Building
---

- The project is for Visual Studio 2005, Express edition should work fine as well. I haven't tried Express edition, but here are some links:  
http://go.microsoft.com/fwlink/?linkid=57034 (Visual C++ Express)  
http://cplus.about.com/od/learnc/ss/vcpppins.htm (About installation)  
http://www.microsoft.com/en-us/download/details.aspx?id=804 (Service Pack 1, I don't know if the first link includes it or not)

- Add %HOMMWOGDIR% to environment variables, set it to the path where WoG is installed. Restart VS if it's running.

- In WoG.ini add these lines:

```ini
[Common]
DeveloperPath=(path to the repository)
AllowMultipleCopies=1
```

Done! Now you can build and run the project. Use "Dll" configugation for debugging. "Release" configuratin is used for deployment. The t1.map file found in Build folder can help you debug it.

About Code
---

To find code of a receiver (let's say, 'UX'), search in erm.cpp for its name like this: 'UX'.

Internal numbers of triggers are listed after ERM_Triggers table in erm.cpp. To find code that calls a trigger you need to search for its number (use "Find in Files" in VS).

All new code hooks are in global_hooks.h and global.cpp files as well as Lua files. Old hooks are in _B1.cpp. Don't do hooks the old way!

Note the WogNew project. The reason it's there is because t1b project must be built with optimization turned off, due to functions intermixing C code with Asm. This is also one of the reasons the solution is for VS 2005.

One thing absolutely necessary in coding style is use of tabs, not spaces (set up in Tools -> Options -> Text Editor -> C/C++ -> Tabs -> Keep Tabs).

IDA Database
---

My database of executable can be found at https://www.dropbox.com/s/wl84icfyxz92ns0/Heroes3f.rar?dl=1  
I update it very rarely.

About Git
---

You may want to hide a local folder from Git. E.g. I have a mod with my test scripts. To do so, create .gitignore file in it containing just this:

```
/*
```
