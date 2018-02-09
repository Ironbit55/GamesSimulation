#include "Renderer.h"

Renderer::Renderer(Window &parent, Physics* physics) : OGLRenderer(parent) {

	p = physics;

	/* We have extended the SceneNode class to include a pointer to a SceneNode's PhysicsData.
	We have then extended the SceneNode's Update function to update to the latest position and 	rotation values, which are
	updated in Physics::UpdatePhysics. */

	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
	
	camera = new Camera(0.0f, 0.0f, Vector3(0, 100, 750.0f));

	currentShader = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");

	// The Map

	map = Mesh::GenerateQuad();

	map->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"map.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	if (!currentShader->LinkProgram() || !map->GetTexture()) {
		return;
	}

	// The dragon

	dragon = Mesh::GenerateQuad();

	dragon->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"dragon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	if (!currentShader->LinkProgram() || !dragon->GetTexture())
	{
		return;
	}

	// The dragon's breath weapon

	breathWeapon = Mesh::GenerateQuad();

	breathWeapon->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"breath.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	if (!currentShader->LinkProgram() || !breathWeapon->GetTexture())
	{
		return;
	}


	// Raider graphics - feel free to extend and edit

	raider = Mesh::GenerateQuad();

	raider->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"raider.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	if (!currentShader->LinkProgram() || !raider->GetTexture()) {
		return;
	}

	/* Declaring our SceneNodes places objects in the environment. We begin with a root note. It you look at the
	constructor, you'll see it defaults to position 0,0,0 - as such, all SceneNodes which are children of the root note are
	translated relative to 0,0,0. This framework means we don't really have to worry about this overmuch - so long as our
	updated PhysicsData makes sense, objects should appear where we want them to appear, and pointing in the right
	direction. */

	root = new SceneNode();

	//SceneNode * mapNode = new SceneNode();
	//mapNode->SetColour(Vector4(1, 1, 1, 1));
	//mapNode->SetTransform(Matrix4::Translation(p->map.getPosition3d())*Matrix4::Rotation(p->map.getRotation(), Vector3(0,0,1)));
	//mapNode->SetModelScale(p->map.getScale());
	//mapNode->SetMesh(map);
	//mapNode->SetBoundingRadius(5.0f);
	//mapNode->SetPhysicsData(&(p->map));
	//root->AddChild(mapNode);

	SceneNode * mapNode = new SceneNode();
	mapNode->SetColour(Vector4(1, 1, 1, 1));
	mapNode->SetTransform(Matrix4::Translation(p->map.position)*Matrix4::Rotation(p->map.rotation, Vector3(0, 0, 1)));
	mapNode->SetModelScale(p->map.scale);
	mapNode->SetMesh(map);
	mapNode->SetBoundingRadius(5.0f);
	mapNode->SetPhysicsData(&(p->map));
	root->AddChild(mapNode);
	//SceneNode * dragonNode = new SceneNode();
	
	//dragonNode->SetColour(Vector4(1, 1, 1, 0.99));
	//dragonNode->SetTransform(Matrix4::Translation(p->dragon.physicsNode.getPosition3d())*Matrix4::Rotation(p->dragon.physicsNode.getRotation(), Vector3(0,0,1)));
	//dragonNode->SetModelScale(p->dragon.physicsNode.getScale());
	//dragonNode->SetMesh(dragon);
	//dragonNode->SetBoundingRadius(5.0f);
	//dragonNode->SetPhysicsData(&(p->dragon.physicsNode));
	//root->AddChild(dragonNode);

	///* Note that breathNode below is an example of a child of dragonNode. This means that its position is translated relative
	//to the dragon (notice how 'out of sync' its coordinates appear to be in Physics::Physics() - only slightly moved down
	//(negative y). Also notice how it moves when the dragon does, without being explicitly told to. Its rotation is initially set purely
	//to make sure it points down. Note that if we rotate the dragon (you can experiment with that) the breathNode moves as though it were
	//bolted to the bottom of the dragon. */

	//SceneNode* breathNode = new SceneNode();
	//breathNode->SetColour(Vector4(1, 1, 1, 0.999));
	//breathNode->SetTransform(Matrix4::Translation(p->breath.getPosition3d())*Matrix4::Rotation(p->breath.getRotation(), Vector3(0,0,1)));
	//breathNode->SetModelScale(p->breath.getScale());
	//breathNode->SetMesh(breathWeapon);
	//breathNode->SetBoundingRadius(5.0f);
	//breathNode->SetPhysicsData(&(p->dragon.physicsNode));
	//dragonNode->AddChild(breathNode);

	//for (int i = 0; i < p->raiders.size(); ++i) {
	//	SceneNode * s = new SceneNode();
	//	s->SetColour(Vector4(1, 1, 1, 0.999));
	//	s->SetTransform(Matrix4::Translation(p->raiders.at(i).physicsNode.getPosition3d())*Matrix4::Rotation(p->raiders.at(i).physicsNode.getRotation(), Vector3(0,0,1)));
	//	s->SetModelScale(p->raiders.at(i).physicsNode.getScale());
	//	s->SetMesh(raider);
	//	s->SetBoundingRadius(5.0f);
	//	s->SetPhysicsData(&(p->raiders.at(i).physicsNode));
	//	root->AddChild(s);
	//}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init = true;
}

Renderer::~Renderer(void) {
	delete root;
	delete raider;
	delete dragon;
	delete breathWeapon;
	delete map;
	delete camera;
}

void Renderer::UpdateScene(float msec) {
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);

	root->Update(msec);
}

void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	DrawNodes();

	glUseProgram(0);
	SwapBuffers();

	ClearNodeLists();
}

void	Renderer::DrawNode(SceneNode*n) {
	if (n->GetMesh()) {
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&(n->GetWorldTransform()*Matrix4::Scale(n->GetModelScale())));
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());
		GL_BREAKPOINT

			n->GetMesh()->Draw();
	}
}

void	Renderer::BuildNodeLists(SceneNode* from) {
	Vector3 direction = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
	from->SetCameraDistance(Vector3::Dot(direction, direction));

	if (frameFrustum.InsideFrustum(*from)) {
		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void	Renderer::DrawNodes() {
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		DrawNode((*i));
	}

	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));
	}
}

void	Renderer::SortNodeLists() {
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void	Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}