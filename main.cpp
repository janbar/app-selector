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

#include "mainwindow.h"
#include "jsonparser.h"
#include <assert.h>
#include <stdio.h>
#include <QApplication>
#include <QScreen>
#include <QString>
#include <QDesktopWidget>

#include <iostream>
#include <string>

#define WINDOW_DFLT_FONT_FAMILY               ""
#define WINDOW_DFLT_FONT_WEIGHT               50
#define WINDOW_DFLT_ROW_COUNT                 24

using namespace std;

typedef vector<Item> vitem;

static void readJsonFromStream(istream *file, vitem &v)
{
  string doc;
  while (*file)
  {
    string line;
    getline(*file, line);
    doc.append(line);
  }

  JSON::Document json(doc);
  if (!json.IsValid())
    return;
  JSON::Node root = json.GetRoot();
  if (root.IsArray())
  {
    for (size_t i = 0; i < root.Size(); ++i)
    {
      JSON::Node elem = root.GetArrayElement(i);
      if (!elem.IsObject())
        continue;
      Item item;
      JSON::Node f1(elem.GetObjectValue("name"));
      if (f1.IsString())
        item.name = QString::fromUtf8(f1.GetStringValue().c_str());
      JSON::Node f2(elem.GetObjectValue("exec"));
      if (f2.IsString())
        item.execCmd = QString::fromUtf8(f2.GetStringValue().c_str());
      JSON::Node f3(elem.GetObjectValue("icon"));
      if (f3.IsString())
        item.iconPath = QString::fromUtf8(f3.GetStringValue().c_str());
      v.push_back(item);
    }
  }
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
  QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
  int ret = -1;
  char *buf;
  int window_x, window_y; // posX, posY
  int window_w, window_h; // width, height
  const char *font_name;
  int font_weight;
  int row_count;
  bool fs;

  // Processing command line arguments
  buf = getCmd(argv, argv + argc, "--help");
  if (buf)
  {
    cout << "Application selector usage:\n"
            "    app-selector [OPTION]... < [ITEM FILE]\n"
            "\nwhere options include:\n"
            "    --fullscreen                 use the entire screen.\n"
            "    --geometry WxH               size of window.\n"
            "    --font fontname              font family name.\n"
            "    --font-weight weight         font weight.\n"
            "    --rows count                 page size.\n"
            "\nStructure of JSON file :\n"
            "[\n  {\n"
            "    \"name\": <string>, /* Friendly name of application       */\n"
            "    \"exec\": <string>, /* Command line or any data as output */\n"
            "    \"icon\": <string>  /* Icon path                          */\n"
            "  }, ...\n"
            "]\n";
    return 0;
  }
  buf = getCmdOption(argv, argv + argc, "--fullscreen");
  if (buf)
    fs = true;
  else
    fs = false;
  buf = getCmdOption(argv, argv + argc, "--geometry");
  if (!buf || sscanf(buf, "%ix%i", &window_w, &window_h) != 2)
  {
    window_x = 0;
    window_y = 0;
    window_w = screenGeometry.width();
    window_h = screenGeometry.height();
  }
  else
  {
      window_x = (screenGeometry.width() - window_w) / 2;
      window_y = (screenGeometry.height() - window_h) / 2;
  }
  buf = getCmdOption(argv, argv + argc, "--font");
  if (!buf)
    font_name = WINDOW_DFLT_FONT_FAMILY;
  else
    font_name = buf;
  buf = getCmdOption(argv, argv + argc, "--font-weight");
  if (!buf || sscanf(buf, "%i", &font_weight) != 1)
  {
    font_weight = WINDOW_DFLT_FONT_WEIGHT;
  }
  buf = getCmdOption(argv, argv + argc, "--rows");
  if (!buf || sscanf(buf, "%i", &row_count) != 1)
  {
    row_count = WINDOW_DFLT_ROW_COUNT;
  }

  // fix limits
  row_count = (row_count < 5 ? 5 : (row_count > 25 ? 25 : row_count));
  int row_height = window_h / row_count;

  vitem v;
  readJsonFromStream(&cin, v);

  MainWindow window;

  // Setup our window size and position
  window.resize(window_w, window_h);
  window.move(window_x, window_y);

  // Setup our font face, size and weight
  QFont font;
  font.setFamily(QString::fromUtf8(font_name));
  double font_size = 0.70 * row_height;
  font.setPointSize(int(font_size));
  font.setBold(false);
  font.setWeight(font_weight);
  window.setFont(font);

  // Setup our icon size
  double icon_size = row_height;
  window.setIconSize(QSize(icon_size, icon_size));

  for (vitem::iterator it = v.begin(); it != v.end(); ++it)
    window.AddItem(*it);

  if (fs)
    window.showFullScreen();
  else
    window.show();
  window.activateWindow();

  ret = a.exec();

  if (ret == 0)
  {
    int selection = window.GetSelected();
    if (selection >= 0)
    {
      Item item = v.at(selection);
      cout << item.execCmd.toStdString();
      cout << "\n";
    }
  }

  return ret;
}
