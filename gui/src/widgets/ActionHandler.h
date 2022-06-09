#pragma once

#include <src/models/ProjectNode.h>

#include <src/hkx/ResourceManager.h>
#include <src/models/ProjectTreeModel.h>
#include <src/models/ProjectTreeActions.h>

#include <QObject>
#include <QMenu>
#include <QTreeView>

namespace ckcmd {
	namespace HKX {

		class ActionHandler : public QObject
		{
			Q_OBJECT

			ProjectTreeActions& _treeActions;

			QAction* _save;

			QAction* _createProject;

			void buildSaveAction();
			void buildCreateProjectAction();

		public slots:
			void save();

			void createProject();

			void export_to(ProjectNode* project_node);

			void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

		public:
			ActionHandler(ProjectTreeActions& treeActions, QObject* parent = nullptr) :
				QObject(parent),
				_save(nullptr),
				_treeActions(treeActions)
			{
				buildSaveAction();


				//connect(_modelview.selectionModel(), &QItemSelectionModel::selectionChanged,
				//		this, &ActionHandler::selectionChanged);
			}



			QAction* saveAction() {	return _save;	}
			QAction* exportAction();
			QAction* saveAction(ProjectNode* node);
			QAction* exportAction(ProjectNode* node);

			QAction* createProjectAction() { return _createProject; }

		};
	}
}