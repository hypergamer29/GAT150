#include "Font.h"
#include <SDL2-2.28.0/include/SDL_ttf.h>

namespace Loki {
	Font::Font(const std::string& filename, int fontSize) {
		Load(filename, fontSize);
	}

	Font::~Font() {
		if (m_ttfFont != NULL) {
			Font(TTF_closefont);
		}

	}

	bool Font::Load(const std::string& filename, int fontSize) {
		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);
	}

	bool Font::Create(std::string filename, ...) {
		return false;
	}
}