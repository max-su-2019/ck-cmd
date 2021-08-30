#pragma once

#include <src/config.h>
#include <src/workspace.h>
#include <src/models/ProjectNode.h>
#include <src/hkx/ResourceManager.h>

#include <core/AnimationCache.h>


#include <set>

namespace ckcmd {
	namespace HKX {

		class AnimationManager {

			WorkspaceConfig& _workspace;
			ResourceManager& _resource_manager;
			AnimationCache _cache;

			void scanWorkspace();

		public:
			AnimationManager(WorkspaceConfig& _workspace, ResourceManager& resource_manager);

			bool empty();

			void buildProjectTree(ProjectNode* root);
		};
	}
}