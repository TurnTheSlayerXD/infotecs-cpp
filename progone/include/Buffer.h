

#ifndef BUFFER_H

#define BUFFER_H

#include <condition_variable>
#include <vector>
#include <thread>

template <typename T> class Buffer {
  std::condition_variable cv_;
  std::vector<T> buf_;

  mutable std::mutex mut_;

public:

  void add(const T &data);

  std::condition_variable &get_cv();

  void clear();
  T back() const;

  bool pop(T &ref);

  int size() const;
  bool empty() const;
};


template <typename T> void Buffer<T>::add(const T &data) {
  {
    std::lock_guard lk(mut_);
    buf_.push_back(data);
  }
  cv_.notify_one();
}

template <class T> std::condition_variable &Buffer<T>::get_cv() { return cv_; }

template <class T> void Buffer<T>::clear() {
  std::lock_guard lk{mut_};
  buf_.clear();
}

template <class T> T Buffer<T>::back() const {
  std::lock_guard lk{mut_};
  T data = buf_.back();
  return data;
}

template <class T> bool Buffer<T>::pop(T &ref) {
  std::lock_guard lk(mut_);
  if (buf_.empty()) {
    return false;
  }
  ref = std::move(buf_.back());
  buf_.pop_back();
  return true;
}


template <class T> bool Buffer<T>::empty() const {
    std::lock_guard lk{mut_};
    return buf_.empty();
  }

template <class T> int Buffer<T>::size() const {
  std::lock_guard lk{mut_};
  return buf_.size();
}



#endif
