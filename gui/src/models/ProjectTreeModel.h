#pragma once

#include <QAbstractItemModel>
#include <src/models/ProjectNode.h>
#include <src/hkx/CommandManager.h>
#include <src/hkx/ResourceManager.h>
#include <src/models/ProjectTreeActions.h>

//Proof of concept, but unuseful. look for table/tree models instead

namespace ckcmd {
    namespace HKX {

        class ProjectTreeModel : public QAbstractItemModel {
            Q_OBJECT
                friend class TreeBuilder;

            enum class NodeType
            {
                Invalid = 0,
                ProjectNode = 1,
                HavokNative = 2
            };

            class ModelEdge
            {
                friend class ProjectTreeModel;

                NodeType _parentType = NodeType::Invalid;
                void* _parentItem = nullptr;
                QModelIndex _parent;
                int _file = -1;
                int _row = -1;
                int _column = -1;
                NodeType _childType = NodeType::Invalid;
                void* _childItem = nullptr;
                QModelIndex _child;

                auto as_tuple() const { return std::tie(_parentItem, _file, _row, _column, _childItem); }

            public:
                bool operator < (const ModelEdge& rhs) const {
                    return 
                        as_tuple() < rhs.as_tuple();
                }

                ModelEdge() {}

                ModelEdge(ProjectNode*, int file, int row, int column, ProjectNode*);
                ModelEdge(ProjectNode*, int file, int row, int column, hkVariant*);
                ModelEdge(hkVariant*, int file, int row, int column, hkVariant*);

                QVariant data(int row, int column) const;

                ModelEdge childEdge(int index, ResourceManager& manager) const;
                int childCount() const;
            };

            struct ModelEdgeComparator {
                bool operator()(const ModelEdge* a, const ModelEdge* b) const {
                    return *a < *b;
                }
            };

            std::map<ModelEdge*, qintptr, ModelEdgeComparator> _reverse_find;
            std::map<qintptr, ModelEdge> _direct_find;

            qintptr modelEdgeIndex(const ModelEdge& edge) const;
            const ModelEdge& modelEdge(const QModelIndex& index) const;
            ModelEdge& modelEdge(const QModelIndex& index);

            bool hasModelEdgeIndex(const ModelEdge& edge) const;
            qintptr createModelEdgeIndex(const ModelEdge& edge);

            CommandManager& _commandManager;
            ResourceManager& _resourceManager;
            ProjectTreeActions _actionsManager;

        public:

            ProjectTreeModel(CommandManager& commandManager, ResourceManager& resourceManager, QObject* parent = 0);
            ~ProjectTreeModel() 
            {
            }

            void select(const QModelIndex& index);
            void activate(const QModelIndex& index);

            //void notifyBeginInsertRows(const QModelIndex& index, int first, int last) {
            //    emit beginInsertRows(index, first, last);
            //}

            //void notifyEndInsertRows() {
            //    emit endInsertRows();
            //}

            //void notifyBeginRemoveRows(const QModelIndex& index, int first, int last) {
            //    emit beginRemoveRows(index, first, last);
            //}

            //void notifyEndRemoveRows() {
            //    emit endRemoveRows();
            //}

            //void notifyBeginMoveRows(const QModelIndex& sourceParent, int sourceFirst, int sourceLast, const QModelIndex& destinationParent, int destinationRow) {
            //    emit beginMoveRows(sourceParent, sourceFirst, sourceLast, destinationParent, destinationRow);
            //}

            //void notifyEndMoveRows() {
            //    emit endMoveRows();
            //}

            //void notifyElementChanged(const QModelIndex& index) {
            //    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
            //}


            /*
            ** AbstractItemModel(required methods)
            */
            QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
            QModelIndex index(int row, int col, const QModelIndex& parent = QModelIndex()) const override;
            QModelIndex parent(const QModelIndex& index) const override;
            int rowCount(const QModelIndex& parent = QModelIndex()) const override;
            int columnCount(const QModelIndex& parent = QModelIndex()) const override;
            QVariant headerData(int section, Qt::Orientation orientation,
                int role = Qt::DisplayRole) const override;

            Qt::ItemFlags flags(const QModelIndex& index) const override;
            bool setData(const QModelIndex& index, const QVariant& value,
                int role = Qt::EditRole) override;

            //QModelIndex getIndex(ProjectNode* node) const;

            ProjectNode* _rootNode = NULL;

        };
    }
}