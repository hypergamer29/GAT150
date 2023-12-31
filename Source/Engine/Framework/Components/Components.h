#pragma once
#include "FrameWork/Object.h"

namespace Loki {
	class Components : public Object{
	public:
		virtual void Update(float dt) = 0;

		friend class Actor;

	public:
		class Actor* m_owner = nullptr;

	};
}