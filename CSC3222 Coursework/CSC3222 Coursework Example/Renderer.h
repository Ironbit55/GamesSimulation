#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include <algorithm>
#include "Physics.h"


class Renderer : public OGLRenderer {
public:
	Renderer(Window &parent, Physics* p);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();
	void	DrawPath(vector<Node>& path, bool success = true);
	void	deletePath(Node& rootNode);
	void	deleteAllPaths();
	Physics* p;

protected:
	void	BuildNodeLists(SceneNode* from);
	void	SortNodeLists();
	void	ClearNodeLists();
	void	DrawNodes();
	void	DrawNode(SceneNode*n);


	SceneNode* root;
	Camera*	camera;
	Mesh*	map;
	Mesh*	raider;
	Mesh*	dragon;
	Mesh*	breathWeapon;
	Mesh*	terrainCollider;
	Mesh*	pathNode;

	SceneNode* pathRootNode;
	std::map<int, SceneNode*> pathRootNodeMap;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

