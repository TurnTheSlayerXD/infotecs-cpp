
#ifndef IPC_INTERACTION_H
#define IPC_INTERACTION_H
#include <iostream>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <cstring>

namespace ipc {

constexpr size_t text_size_ = 200;

constexpr key_t key_ = 10;

constexpr long message_type_ = 1;

struct message {
  long mtype = message_type_;
  char text[text_size_];
};

std::pair<std::string, bool> receive_message() {

  const auto false_return = std::make_pair(std::string(""), false);

  int msg_id;
  if ((msg_id = msgget(key_, 0666)) < 0) {
    return false_return;
  }

  message msg;
  if (msgrcv(msg_id, &msg, text_size_, message_type_, 0) < 0) {
    std::cerr << "Error: msgrcv\n";
    return false_return;
  }

  return std::make_pair(std::string(msg.text), true);
}

bool send_message(const std::string &text) {
  int msg_id;

  if ((msg_id = msgget(key_, IPC_CREAT | 0666)) < 0) {
    std::cerr << "Error: msgget returned code" << msg_id << "\n";
    return false;
  }

  message msg;

  std::strncpy(msg.text, text.c_str(), text_size_);

  int res;
  if ((res = msgsnd(msg_id, &msg, std::strlen(msg.text) + 1, IPC_NOWAIT)) < 0) {
    std::cerr << "Error: msgsnd returned code" << res << "\n";
    return false;
  }

  return true;
}

} // namespace ipc
#endif