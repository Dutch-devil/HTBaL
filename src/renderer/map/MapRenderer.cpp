#include "MapRenderer.h"

MapRenderer::MapRenderer(Rectangle viewport) : Renderer(viewport) {
}

MapRenderer::~MapRenderer() {

}

void MapRenderer::update(float elapsedTime) {
	
}

Renderers MapRenderer::getNextRenderer() {
	return KEEP;
}

void MapRenderer::render(float elapsedTime) {
}