#pragma once

#include <core/HKXWrangler.h>
#include <src/Log.h>
#include <src/models/ProjectNode.h>
#include <src/hkx/ITreeBuilderClassHandler.h>
#include <src/hkx/ISpecialFieldsHandler.h>

#include <map>

typedef std::pair< hkVariant, hkArray<hkVariant>> hkx_file_t;

namespace ckcmd {
	namespace HKX {

		typedef std::vector<std::pair<int, const hkVariant*>> hk_object_list_t;

		class ResourceManager {

			std::vector<fs::path> _files;
			std::map<size_t, hkx_file_t> _contents;
			std::map<size_t, ITreeBuilderClassHandler*> _class_handlers;
			std::map<size_t, ISpecialFieldsHandler*> _field_handlers;
			std::map<size_t, std::vector<ProjectNode*>> _nodes;
			const fs::path _workspace_folder;

		public:

			ResourceManager(const fs::path& workspace_folder);

			hkx_file_t& get(const fs::path& file);

			//fs::path ResourceManager::open(const std::string& project);

			size_t index(const fs::path& file) const;

			int findIndex(int file_index, const void* object) const;
			int findIndex(const fs::path& file, const void* object) const;

			bool isHavokProject(const fs::path& file);

			hkVariant* at(const fs::path& file, size_t _index);
			const hkVariant* at(const fs::path& file, size_t _index) const;
			hkVariant* ResourceManager::at(size_t file_index, size_t _index);
			const hkVariant* ResourceManager::at(size_t file_index, size_t _index) const;


			void setClassHandler(size_t index, ITreeBuilderClassHandler* handler);
			void setFieldHandler(size_t index, ISpecialFieldsHandler* handler);

			ITreeBuilderClassHandler* classHandler(size_t index) const;
			ISpecialFieldsHandler* fieldsHandler(size_t index) const;

			hk_object_list_t findCompatibleNodes(size_t file_index, const hkClassMember* member_class) const;

			ProjectNode* createStatic(const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createStatic(const QVector<QVariant>& data, ProjectNode* parentItem, ProjectNode::NodeType type);
			ProjectNode* createSupport(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createProject(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createCharacter(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createBehavior(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createSkeleton(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItemm = nullptr);
			ProjectNode* createAnimation(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createMisc(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createHkxProject(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createHkxCharacter(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createHkxNode(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createEventNode(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createVariableNode(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);
			ProjectNode* createPropertyNode(size_t file_index, const QVector<QVariant>& data, ProjectNode* parentItem = nullptr);

		};
	}
}