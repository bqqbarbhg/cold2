#ifndef _C2_RENDER_RENDER_H
#define _C2_RENDER_RENDER_H

#include <c2/core/enabled_event.h>
#include <string>

namespace c2 { namespace render {

typedef EnabledEvent<void(const std::string&), void(*)(bool)> ErrorEvent;
extern ErrorEvent on_error;

} }

#endif