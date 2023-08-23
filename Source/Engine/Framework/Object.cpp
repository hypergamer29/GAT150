#include "Object.h"


namespace Loki {
	void Object::Read(const json_t& value) {
		READ_DATA(value, name)
	}
}