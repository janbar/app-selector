/*
 *      Copyright (C) 2013 Jean-Luc Barriere
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "item.h"
#include "mainwindow.h"
#include <assert.h>
#include <stdio.h>
#include <QApplication>
#include <QDesktopWidget>
#include <iostream>
#include <string>

#define WINDOW_DFLT_WIDTH                     800
#define WINDOW_DFLT_HEIGHT                    522
#define WINDOW_DFLT_FONT_FAMILY               ""
#define WINDOW_DFLT_FONT_POINT_SIZE           36
#define WINDOW_DFLT_FONT_WEIGHT               50
#define WINDOW_DFLT_ICON_SIZE                 102

using namespace std;

typedef vector<Item*> vitem;

// trim from start

static inline string &ltrim(string &s)
{
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}

// trim from end

static inline string &rtrim(string &s)
{
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}

// trim from both ends

static inline string &trim(string &s)
{
  return ltrim(rtrim(s));
}

static void readItemsFromStream(istream *file, vitem *v)
{
  bool bitem = false;
  Item *item;

  assert(v != NULL);
  v->clear();

  while (*file)
  {
    string sline;
    getline(*file, sline);
    string tline = trim(sline);

    if (tline.compare(0, 6, "<item>") == 0)
    {
      bitem = true;
      item = new Item;
    }
    else if (bitem)
    {
      if (tline.compare(0, 7, "</item>") == 0)
      {
        bitem = false;
        v->push_back(item);
      }
      if (tline.compare(0, 5, "name ") == 0)
      {
        string val = tline.substr(5, tline.length() - 5);
        item->name = trim(val);
      }
      if (tline.compare(0, 5, "exec ") == 0)
      {
        string val = tline.substr(5, tline.length() - 5);
        item->execCmd = trim(val);

      }
      if (tline.compare(0, 5, "icon ") == 0)
      {
        string val = tline.substr(5, tline.length() - 5);
        item->iconPath = trim(val);
      }
    }
  }
  if (bitem)
    delete item;
}

static char* getCmd(char **begin, char **end, const string &option)
{
  char **itr = std::find(begin, end, option);
  if (itr != end)
  {
    return *itr;
  }
  return NULL;
}

static char* getCmdOption(char **begin, char **end, const string &option)
{
  char **itr = std::find(begin, end, option);
  if (itr != end && ++itr != end)
  {
    return *itr;
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  int ret = -1;
  char *buf;
  int window_w, window_h; // width, height
  const char *window_font; // font family
  int window_fs, window_fw; // font size, weight
  int window_is; // icon size

  // Processing command line arguments
  buf = getCmd(argv, argv + argc, "--help");
  if (buf)
  {
    cout << "Usage : app-selector [OPTION]... < [ITEM FILE]\n"
            "\nLaunch applications selector GUI and return the 'exec' command of selected\n"
            "item.\n"
            "\nOptional arguments :\n"
            " --geometry\n"
            "             This option specifies the preferred size of window. [WxH]\n"
            " --font\n"
            "             This option specifies the prefered font family name.\n"
            " --font-size\n"
            "             This option specifies the font point size.\n"
            " --font-weight\n"
            "             This option specifies the font weight.\n"
            " --icon-size\n"
            "             This option specifies the icon size.\n"
            "\nStructure of items file :\n"
            "<item>\n"
            "    name    [Friendly name of application]\n"
            "    icon    [Icon path]\n"
            "    exec    [Command line or any data as output]\n"
            "</item>\n"
            "<item>\n"
            "...\n";
    return 0;
  }
  buf = getCmdOption(argv, argv + argc, "--geometry");
  if (!buf || sscanf(buf, "%ix%i", &window_w, &window_h) != 2)
  {
    window_w = WINDOW_DFLT_WIDTH;
    window_h = WINDOW_DFLT_HEIGHT;
  }
  buf = getCmdOption(argv, argv + argc, "--font");
  if (!buf)
    window_font = WINDOW_DFLT_FONT_FAMILY;
  else
    window_font = buf;
  buf = getCmdOption(argv, argv + argc, "--font-size");
  if (!buf || sscanf(buf, "%i", &window_fs) != 1)
  {
    window_fs = WINDOW_DFLT_FONT_POINT_SIZE;
  }
  buf = getCmdOption(argv, argv + argc, "--font-weight");
  if (!buf || sscanf(buf, "%i", &window_fw) != 1)
  {
    window_fw = WINDOW_DFLT_FONT_WEIGHT;
  }
  buf = getCmdOption(argv, argv + argc, "--icon-size");
  if (!buf || sscanf(buf, "%i", &window_is) != 1)
  {
    window_is = WINDOW_DFLT_ICON_SIZE;
  }

  vitem *v = new vitem;
  readItemsFromStream(&cin, v);

  MainWindow window;

  // Setup our window size
  window.resize(window_w, window_h);

  // Setup our font face, size and weight
  QFont font;
  font.setFamily(QString::fromUtf8(window_font));
  font.setPointSize(window_fs);
  font.setBold(false);
  font.setWeight(window_fw);
  window.setFont(font);

  // Setup our icon size
  window.setIconSize(QSize(window_is, window_is));

  // Main window is shown in the center of our desktop screen
  QRect screenGeometry = QApplication::desktop()->screenGeometry();
  int x = (screenGeometry.width() - window.width()) / 2;
  int y = (screenGeometry.height() - window.height()) / 2;
  window.move(x, y);

  for (vitem::iterator it = v->begin(); it != v->end(); ++it)
    window.AddItem(QString((*it)->name.c_str()),
          QString((*it)->iconPath.c_str()),
          QString((*it)->execCmd.c_str()));

  window.show();

  ret = a.exec();

  if (ret == 0)
  {
    int selection = window.GetSelected();
    if (selection >= 0)
    {
      Item *item = v->at(selection);
      cout << item->execCmd;
      cout << "\n";
    }
  }

  for (vitem::iterator it = v->begin(); it != v->end(); ++it)
    delete *it;
  delete v;

  return ret;
}
