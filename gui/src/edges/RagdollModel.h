#pragma once

#include <src/edges/SupportEnhancedEdge.h>

#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h>

namespace ckcmd {
	namespace HKX {

		class RagdollModel : public SupportEnhancedEdge
		{

			hkaRagdollInstance* variant(const ModelEdge& edge) const;
			virtual std::vector<std::function<hkVariant* (const ModelEdge&, ResourceManager& manager, hkVariant*)>> additional_variants() const override { return {}; }

		protected:

			virtual int supports() const override;
			virtual const char* supportName(int support_index) const override;
			virtual NodeType supportType(int support_index) const override;

		public:

			virtual int rows(const ModelEdge& edge, ResourceManager& manager) const override;
			virtual int columns(int row, const ModelEdge& edge, ResourceManager& manager) const override;
			virtual int childCount(const ModelEdge& edge, ResourceManager& manager) const override;
			virtual std::pair<int, int> child(int index, const ModelEdge& edge, ResourceManager& manager) const override;
			virtual int childIndex(int row, int column, const ModelEdge& edge, ResourceManager& manager) const override;
			virtual ModelEdge child(int row, int column, const ModelEdge& edge, ResourceManager& manager) const override;
			virtual QVariant data(int row, int column, const ModelEdge& edge, ResourceManager& manager) const override;
		};
	}
}
