#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Public/Ingredients/defines.h>

#include <map>

using namespace RTX;
using namespace CppUtil::Basic;

namespace RTX
{
	using namespace std;
	struct Vertex
	{
		Vertex(const RayVec3 pos = RayVec3(0.0f), const RayVec2 uv = RayVec2(0.0f)) : pos(pos), uv(uv) {}
		RayVec3 pos = RayVec3(0.0f);
		//RayVec3 normal = RayVec3(0.0f, 0.0f, 1.0f);
		RayVec2 uv = RayVec2(0.0f);
	};

	class Mesh : public Hitable
	{
	public:
		Mesh(const string& path, CppUtil::Basic::Ptr<Material> material = nullptr, float scale = 1.0f) : scale(scale), material(material)
		{
			// read file via ASSIMP
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
			// check for errors
			if (!(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)) // if is Not Zero
			{
				Load(scene->mRootNode, scene);
			}
		}
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, RayPrecision t_min, RayPrecision t_max, HitRecord& rec) const override
		{
			rec.material = material;
			return model->hit(ray, t_min, t_max, rec);
		}
		virtual bool boundingBox(RayPrecision t0, RayPrecision t1, AABB& output_box) const override
		{
			output_box = model->box;
			return true;
		}
	private:
		void Load(const aiMesh* mesh, const aiScene* scene)
		{
			vector<Vertex> vertices;
			// compute normal
			map<size_t, vector<uvec3>> idx2fV;
			map<size_t, vec3> idx2n;
			if (mesh->mNormals == NULL)
			{
				// collect faces
				for (size_t i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace face = mesh->mFaces[i];
					uvec3 uvec3_face(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
					idx2fV[uvec3_face[0]].push_back(uvec3_face);
					idx2fV[uvec3_face[1]].push_back(uvec3_face);
					idx2fV[uvec3_face[2]].push_back(uvec3_face);
				}
				// compute average normal
				//for (auto const pair : idx2fV)
				//{
				//	vec3 normal(0);
				//	for (auto const face : pair.second)
				//	{
				//		vec3 p[3];
				//		for (size_t i = 0; i < 3; i++)
				//		{
				//			p[i].x = mesh->mVertices[face[i]].x * scale;
				//			p[i].y = mesh->mVertices[face[i]].y * scale;
				//			p[i].z = mesh->mVertices[face[i]].z * scale;
				//		}
				//		normal += normalize(cross(p[1] - p[0], p[2] - p[0]));
				//	}
				//	idx2n[pair.first] = normal / (float)pair.second.size();
				//}
			}

			// walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
			for (size_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				// retrieve all indices of the face and store them in the indices vector
				for (size_t j = 0; j < face.mNumIndices; j++)// mNumIndices == 3
				{
					size_t idx = face.mIndices[j];
					//indices.push_back(face.mIndices[j]);
					Vertex vertex;
					glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

					// positions
					vector.x = mesh->mVertices[idx].x * scale;
					vector.y = mesh->mVertices[idx].y * scale;
					vector.z = mesh->mVertices[idx].z * scale;
					vertex.pos = vector;

					// normals
					//if (mesh->mNormals != NULL) {
					//	vector.x = mesh->mNormals[idx].x;
					//	vector.y = mesh->mNormals[idx].y;
					//	vector.z = mesh->mNormals[idx].z;
					//}
					//else
					//{
					//	vector = idx2n[idx];
					//}

					//vertex.normal = vector;
					// texture coordinates
					if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
					{
						// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
						// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
						vertex.uv.x = mesh->mTextureCoords[0][idx].x;
						vertex.uv.y = mesh->mTextureCoords[0][idx].y;
					}
					vertices.push_back(vertex);
				}
			}
			buildTree(vertices);
		}
		void Load(const aiNode* node, const aiScene* scene)
		{
			// process each mesh located at the current node
			for (size_t i = 0; i < node->mNumMeshes; i++)
			{
				// the node object only contains indices to index the actual objects in the scene. 
				// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				Load(mesh, scene);
			}
			// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
			for (size_t i = 0; i < node->mNumChildren; i++)
			{
				Load(node->mChildren[i], scene);
			}
		}
		void buildTree(const vector<Vertex>& vertices)
		{
			HittableList obj;
			for (int i = 0; i < vertices.size(); i += 3)
			{
				obj.add(ToPtr(new Triangle(vertices[i].pos, vertices[i + 1].pos, vertices[i + 2].pos, vertices[i].uv, vertices[i + 1].uv, vertices[i + 2].uv)));
			}
			model = ToPtr(new BVH(obj, 0.0f, 0.01f));
		}
	private:
		float scale;
		CppUtil::Basic::Ptr<Material> material = nullptr;
		CppUtil::Basic::Ptr<BVH> model;
	};
}
