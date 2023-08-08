#include "Moedel.h"
#include <sstream>

namespace Loki {
	bool Model::Load(const std::string& filename) {
		std::string buffer;
		Loki::readFile(filename, buffer);

		std::istringstream stream(buffer);

		// read color
		stream >> m_color;

		// read # of points
		std::string line;
		std::getline(stream, line);
		int numPoints = std::stoi(line);

		// read Vector2 points
		for (int i = 0; i < numPoints; i++) {
			vec2 point;
			stream >> point;

			m_points.push_back(point);
		}

		return true;
	}

	void Loki::Model::Draw(Renderer& renderer, const vec2& position, float roatation, float scale) {

		if (m_points.empty()) return;

		renderer.SetColor(Color::ToInt(m_color.r), Color::ToInt(m_color.g), Color::ToInt(m_color.b), Color::ToInt(m_color.a));
		for (int i = 0; i < m_points.size() - 1; i++) {
			vec2 p1 = (m_points[i] * scale).Rotate(roatation) + position;
			vec2 p2 = (m_points[i + 1] * scale).Rotate(roatation) + position;

			renderer.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}

	}

	void Model::Draw(Renderer& renderer, const Transform& transform) {
		Draw(renderer, transform.position, transform.rotation, transform.scale);
	}
	float Model::GetRadius() {
		if (m_radius != 0) return m_radius;

		for (auto point : m_points) {
			float length = point.Length();
			m_radius = Max(m_radius, length);
		}

		return m_radius;
	}

	bool Model::Create(std::string filename, ...) {
		return Load(filename);
	}
}
