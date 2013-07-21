Create Makefile
===============

qmake QMAKE_CC=/usr/bin/gcc QMAKE_CXX=/usr/bin/g++ QMAKE_CFLAGS=-g3\ -gdwarf-2 \
QMAKE_CXXFLAGS=-g3\ -gdwarf-2 -o Makefile app-selector.pro

Build from source
=================

make clean
make

Usage
=====

Usage : app-selector [OPTION]... < [ITEM FILE]

Launch applications selector GUI and return the 'exec' command of selecteditem.

Optional arguments :
 --geometry
             This option specifies the preferred size of window. [WxH]
 --font
             This option specifies the prefered font family name.
 --font-size
             This option specifies the font point size.
 --font-weight
             This option specifies the font weight.
 --icon-size
             This option specifies the icon size.

Structure of items file :
<item>
    name    [Friendly name of application]
    icon    [Icon path]
    exec    [Command line or any data as output]
</item>
<item>
...

Sample
======

1 - Launch selector GUI from shell and execute selected application

$(./app-selector --gemometry 800x522 --font-size 36 --icon-size 102 <<EOF
<item>
  name XTerm - X11 terminal
  exec /usr/bin/xterm -fn 9x15
  icon /usr/share/pixmaps/gnome-xterm.png
</item>
<item>
  name Firefox - Internet browser
  exec /usr/bin/firefox
  icon /usr/share/icons/Faenza/apps/scalable/firefox-original.svg
</item>
EOF
)
