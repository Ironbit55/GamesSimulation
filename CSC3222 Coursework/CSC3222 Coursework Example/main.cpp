#include "../../nclgl/window.h"
#include "Renderer.h"
#include "Physics.h"

#pragma comment(lib, "nclgl.lib")

int main() {
	Window w("CSC3222 Coursework", 1920, 1080, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Physics physics;
	Renderer renderer(w, &physics);
	physics.initialse(&renderer);
	
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(false);
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float msec = w.GetTimer()->GetTimedMS();
		physics.UpdatePhysics(msec);
		renderer.UpdateScene(msec);
		renderer.RenderScene();


	}

	return 0;
}