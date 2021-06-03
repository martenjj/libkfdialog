
LibKFDialog - A helper for implementing QtWidgets dialogues using KDE Frameworks
================================================================================

Qt provides QDialog and QDialogButtonBox for implementing dialogues.
However, there is still some code needed to properly implement a
dialogue which these standard classes do not provide, and a number of
cautions that are needed to make the dialogue appear and work
properly.

This library provides an interface similar to that provided by KDialog
in KDE4.  It manages the button box and its layout, handles saving and
restoring the dialogue size, provides spacing and layout functions
conforming to the application style, and some other useful functions.
The classes and namespaces provided are:

| Class              | Purpose                                            |
| :----------------- | :------------------------------------------------- |
| DialogBase         | Base class for a dialogue (instead of QDialog).    |
|                    | Manages the button box and the top level layout.   |
|                    | The caller provides a central widget which         |
|                    | implements the dialogue UI.                        |
| DialogStateSaver   | Manages saving and restoring the dialogue's        |
|                    | window size to the application configuration file. |
|                    | The default saver saves the window size only, but  |
|                    | it can be subclassed to save other information     |
|                    | (e.g. the column states of a list view).           |
| DialogStateWatcher | Monitors when a dialogue is being shown or closed, |
|                    | and calls the DialogStateSaver to restore or save  |
|                    | the state.  This is a separate class, so that it   |
|                    | can be used on a dialogue which is not derived     |
|                    | from DialogBase (e.g. a KPageDialog).              |
| RecentSaver        | A wrapper around KRecentDirs to easily save a      |
|                    | recently used file dialogue location, and obtain   |
|                    | a URL or path incorporating a suggested file name. |
| ImageFilter        | Generate an image filter string for all of the     |
|                    | image types that QImageReader or QImageWriter      |
|                    | supports.                                          |

More detailed API and programming information can be found in the
header files.

Building and using the library requires Qt <http://qt.io> and
KDE Frameworks <http://kde.org>.  Building the library requires
CMake <http://cmake.org>.  It is built in the same way as any
standard CMake-based project:

```
  git clone https://github.com/martenjj/libkfdialog
  cd libkfdialog
  mkdir BUILD
  cd BUILD
  cmake ..
  make
  make install
```
For an example of the library in use, see KRepton
<https://github.com/martenjj/krepton>.

Jonathan Marten, http://github.com/martenjj
