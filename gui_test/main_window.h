#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "live_event_bus.h"

QT_BEGIN_NAMESPACE
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() = default;

 private:
  live_event_bus::Slot slot_;
  std::shared_ptr<live_event_bus::Observer> t_;
  std::shared_ptr<live_event_bus::Observer> void_t_;
};



#endif  // MAINWINDOW_H
