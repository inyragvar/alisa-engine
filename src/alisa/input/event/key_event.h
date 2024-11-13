#ifndef ALISA_INPUT_EVENT_KEY_EVENT_H_
#define ALISA_INPUT_EVENT_KEY_EVENT_H_

#include <functional>

namespace alisa {
namespace input {

struct KeyData {
    bool is_pressed;
};

typedef std::function<void(KeyData)> key_event_lambda;

class KeyEvent {
public:
    KeyEvent(const key_event_lambda lambda);

    void call(KeyData data);

private:
    key_event_lambda lambda_;
};


}
}

#endif