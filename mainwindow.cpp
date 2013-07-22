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
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow),
m_selection(-1),
m_current(-1)
{
  ui->setupUi(this);
  ui->listWidget->setFocus();
  connect(ui->listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(DoSelect()));
  connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectionChanged()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::AddItem(const QString &text, const QString &iconPath, const QString &whatsThis)
{
  QListWidgetItem *item = new QListWidgetItem;
  QIcon *icon = new QIcon(iconPath);
  item->setText(text);
  item->setIcon(*icon);
  item->setWhatsThis(whatsThis);
  ui->listWidget->addItem(item);
  if (m_current < 0 && ui->listWidget->count() > 0)
  {
    ui->listWidget->setCurrentItem(item);
  }
}

int MainWindow::GetSelected()
{
  return m_selection;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  ui->listWidget->resize(width(), height());
  QWidget::resizeEvent(event);
}

void MainWindow::setFont(const QFont& font)
{
  ui->listWidget->setFont(font);
}

void MainWindow::setIconSize(const QSize& iconSize)
{
  ui->listWidget->setIconSize(iconSize);
}

void MainWindow::DoSelect()
{
  m_selection = ui->listWidget->currentRow();
  this->close();
}

void MainWindow::OnSelectionChanged()
{
  m_current = ui->listWidget->currentRow();
}
