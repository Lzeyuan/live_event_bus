#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace live_event_bus {

class noncopyable {
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

}  // namespace live_event_bus

#endif  // NONCOPYABLE_H