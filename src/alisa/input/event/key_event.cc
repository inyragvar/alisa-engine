#include "key_event.h"

namespace alisa {
namespace input {

KeyEvent::KeyEvent(const key_event_lambda lambda)
    : lambda_(std::move(lambda)) {}

void KeyEvent::call(KeyData data) { lambda_(data); }

}
} 