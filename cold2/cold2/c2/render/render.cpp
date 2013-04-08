#include "render.h"
#include <c2/render/gl.h>

namespace c2 { namespace render {

namespace {
	void error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, void* user)
	{
		on_error(std::string(message, length));
	}
	void error_event_enabled(bool b)
	{
		if (b)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallbackARB(error_callback, nullptr);
		}
		else
		{
			glDisable(GL_DEBUG_OUTPUT);
		}
	}
}

ErrorEvent on_error = ErrorEvent(error_event_enabled);

} }