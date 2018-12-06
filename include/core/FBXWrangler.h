/*
BodySlide and Outfit Studio
Copyright (C) 2018  Caliente & ousnius
See the included LICENSE file
*/

#pragma once

#include <stdafx.h>

#include <core\NifFile.h>
#include <core/HKXWrangler.h>

#include <fbxsdk.h>
#include <unordered_map>

#include <obj\NiNode.h>
#include <nif_math.h>
#include <set>

namespace ckcmd {
namespace FBX {

	struct FBXImportOptions {
		bool InvertU = false;
		bool InvertV = false;
	};


	using namespace std;
	using namespace Niflib;
	using namespace ckcmd::NIF;
	using namespace ckcmd::HKX;

	class FBXShape {
	public:
		class FBXSkin {
		private:
			unordered_map<uint16_t, float> vertWeights;

		public:
			void SetWeight(uint16_t vert, float wt) {
				vertWeights[vert] = wt;
			}

			float GetWeight(uint16_t vert) {
				auto it = vertWeights.find(vert);
				if (it == vertWeights.end())
					return 0.0f;

				return vertWeights[vert];
			}

			std::unordered_map<uint16_t, float>& GetWeights() {
				return vertWeights;
			}
		};

		string name;
		vector<Vector3> verts;
		vector<Triangle> tris;
		vector<TexCoord> uvs;
		vector<Vector3> normals;

		unordered_map<std::string, FBXSkin> boneSkin;
		set<string> boneNames;

	};

	class FBXWrangler {
	private:
		FbxManager * sdkManager = nullptr;
		FbxScene* scene = nullptr;
		HKXWrapper hkxWrapper;

		string comName;
		map<string, FBXShape> shapes;
		NiTriShapeRef importShape(FbxNode* child, const FBXImportOptions& options);
		vector<NiTriShapeRef> importMultipleShape(FbxNode* child, const FBXImportOptions& options);

		map<NiAVObjectRef, NiAVObjectRef> conversion_parent_Map;
		map<FbxNode*, NiObjectRef> conversion_Map;
		map<FbxMesh*, NiTriShapeRef> skins_Map;

		set<FbxNode*> skinned_bones;
		set<FbxNode*> unskinned_bones;
		set<FbxNode*> animated_nodes;
		set<FbxProperty> annotated;

		set<FbxAnimStack*> skinned_animations;
		set<FbxAnimStack*> unskinned_animations;

		NiNodeRef conversion_root;
		set<string> sequences_names;
		set<string> havok_sequences;

		string external_skeleton_path = "";
			
		void checkAnimatedNodes();
		void buildKF();
		double convert(FbxAnimLayer* pAnimLayer, NiControllerSequenceRef sequence, set<NiObjectRef>& targets, NiControllerManagerRef manager, NiMultiTargetTransformControllerRef multiController, string accum_name, double last_start);
		void convertSkins(FbxMesh* m, NiTriShapeRef shape);
	public:
		FBXWrangler();
		~FBXWrangler();

		void setExternalSkeletonPath(const string& external_skeleton_path);
		bool SaveAnimation(const string& fileName);

		void NewScene();
		void CloseScene();

		//void GetShapeNames(vector<string>& outNames) {
		//	for (auto &s : shapes)
		//		outNames.push_back(s.first);
		//}

		//FBXShape* GetShape(const string& shapeName) {
		//	return &(shapes[shapeName]);
		//}

		//void AddSkeleton(NifFile* nif, bool onlyNonSkeleton = false);

		//// Recursively add bones to the skeleton in a depth-first manner
		//FbxNode* AddLimb(NifFile* nif, NiNode* nifBone);
		//void AddLimbChildren(FbxNode* node, NifFile* nif, NiNode* nifBone);

		void AddNif(NifFile& nif);
		void AddExternalSkinnedMeshes(NifFile& skeleton, set<NifFile*> meshes);
		//void AddSkinning(AnimInfo* anim, const std::string& shapeName = "");
		//void AddGeometry(const std::string& shapeName,
		//	const std::vector<Vector3>& verts,
		//	const std::vector<Vector3>& norms,
		//	const std::vector<Triangle>& tris,
		//	const std::vector<TexCoord>& uvs);

		bool ExportScene(const std::string& fileName);
		bool ImportScene(const std::string& fileName, const FBXImportOptions& options = FBXImportOptions());

		bool LoadMeshes(const FBXImportOptions& options);
		bool SaveNif(const string& fileName);
	};	
	
} 
}
