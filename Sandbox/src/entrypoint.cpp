#include <elm.h>
#include <elm/core/entypoiny.h>

#include "sandbox_app.h"

namespace elm {

	application* application::create(void)
	{
		return new sandbox({
			.name = "Sandbox",
		});
	}
}
