#include "MapRenderer.h"

MapRenderer::MapRenderer(Rectangle viewport) : Renderer(viewport) {
}

MapRenderer::~MapRenderer() {

}

Renderers MapRenderer::update(float elapsedTime) {
	return KEEP;
}

void MapRenderer::render(float elapsedTime) {
}