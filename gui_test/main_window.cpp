#include "main_window.h"

#include <QThread>
#include <QTimer>
#include <QtConcurrent>
#include <string>

class QLiveEventBus : public live_event_bus::LiveEventBus {
 private:
  void Process(CallBack callback) override {
    QMetaObject::invokeMethod(
        &obj, [callback]() { callback(); }, Qt::AutoConnection);
  }

  void Log(const std::string& msg) {
    qDebug() << "[QLiveEventBus] " << QString::fromStdString(msg);
  }

 private:
  QObject obj;
#pragma region Singleton
 public:
  static QLiveEventBus& GetInstance() {
    static QLiveEventBus instance;
    return instance;
  }

 private:
  QLiveEventBus() = default;
  ~QLiveEventBus() = default;
  QLiveEventBus(const QLiveEventBus&) = delete;
  QLiveEventBus& operator=(const QLiveEventBus&) = delete;
  QLiveEventBus(const QLiveEventBus&&) = delete;
  QLiveEventBus& operator=(const QLiveEventBus&&) = delete;
#pragma endregion
};

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  auto& liveEventBus = QLiveEventBus::GetInstance();
  t_ = liveEventBus.Subscribe<int>([=](int param) {
    qDebug() << "hello" << param;
    t_->UnObserve();
  });
  void_t_ = liveEventBus.Subscribe<QString>(
      [](QString param) { qDebug() << "oh no!" << param; });

  QTimer::singleShot(1000, this, [=]() {
    QtConcurrent::run([]() { QLiveEventBus::GetInstance().Publish<int>(11); });
  });

  QTimer::singleShot(1500, this, [=]() {
    QtConcurrent::run(
        []() { QLiveEventBus::GetInstance().Publish<QString>("asdasdas"); });
  });

  QTimer::singleShot(1000, this, [=]() {
    QtConcurrent::run(
        []() { QLiveEventBus::GetInstance().Publish<int>(10086); });
  });
}
