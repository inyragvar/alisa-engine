#ifndef ALISA_INPUT_TYPE_H_
#define ALISA_INPUT_TYPE_H_

namespace alisa {
namespace input {

enum class EventType { touch, mouse, key };

enum class KeyMap {
    q,
    w,
    e,
    r,
    t,
    y,
    u,
    i,
    o,
    p,
    a,
    s,
    d,
    f,
    g,
    h,
    j,
    k,
    l,
    z,
    x,
    c,
    v,
    b,
    n,
    m,
    space,
    backspace,
    num_0,
    num_1,
    num_2,
    num_3,
    num_4,
    num_5,
    num_6,
    num_7,
    num_8,
    num_9,

    up,
    down,
    left,
    right,

    key_size = 38
};

enum class MouseMap {
    left,
    right,
    move,

    key_size = 3
};

}
}

#endif