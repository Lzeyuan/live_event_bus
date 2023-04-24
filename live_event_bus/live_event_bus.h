#ifndef LIVE_EVENT_BUS_H
#define LIVE_EVENT_BUS_H

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include "signal_slot.h"

namespace live_event_bus {

class Observer {
 public:
  void SetSlot(Slot slot) { slot_ = slot; }
  void UnObserve() { slot_.reset(); };

 private:
  Slot slot_;
};

class LiveEventBus {
 public:
  using CallBack = std::function<void(void)>;
  using ObserverFunctionPtr = std::function<void(std::any)>;
  using SignalAny = Signal<void(std::any)>;
  using SignalPtr = std::shared_ptr<Signal<void(std::any)>>;
  using EventMap = std::map<std::string, SignalPtr>;

  template <typename TParam>
  std::shared_ptr<Observer> Subscribe(std::function<void(TParam)> handler) {
    return Subscribe<TParam>(typeid(TParam).name(), handler);
  }

  template <typename TParam>
  void Publish(TParam message) {
    Publish(typeid(TParam).name(), message);
  }

  // 自定义同步处理方法
  virtual void Process(CallBack callback) = 0;
  // 自定义日志
  virtual void Log(const std::string& msg) = 0;

 protected:
  template <typename TParam>
  std::shared_ptr<Observer> Subscribe(const std::string& token,
                                      std::function<void(TParam)> handler) {
    auto func = [this, token, handler](const auto& message) {
      Log("Receive event:" + token + ", TParam = " + typeid(TParam).name());
      handler(std::any_cast<TParam>(message));
    };
    auto ret = std::make_shared<Observer>();
    std::weak_ptr<Observer> weak_ret = ret;
    Process([weak_ret, token, func, this]() {
      Log("Subscribe event:" + token);
      auto signal_it = map_.find(token);
      SignalPtr signal_ptr;
      if (map_.end() != signal_it) {
        signal_ptr = signal_it->second;
      } else {
        signal_ptr = std::make_shared<SignalAny>();
        map_.emplace(token, signal_ptr);
      }
      auto slot = signal_ptr->connect(func);
      if (!weak_ret.expired()) {
        auto ret = weak_ret.lock();
        ret->SetSlot(slot);
      }
    });
    return ret;
  }

  template <typename TParam>
  void Publish(const std::string& token, TParam& message) {
    Process([token, message, this]() {
      Log("Publish event: " + token);
      auto signal_it = map_.find(token);
      if (map_.end() != signal_it) {
        (*signal_it).second->call(message);
      }
    });
  }

  EventMap map_;
};

}  // namespace live_event_bus
#endif  // LIVE_EVENT_BUS_H