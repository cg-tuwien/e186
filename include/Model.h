#pragma once

#include "Animator.h"

namespace e186
{
	struct AnimationClip
	{
		unsigned int m_animation_index;
		double m_ticks_per_second;
		double m_start_ticks;
		double m_end_ticks;
		Animator* m_animator;

		AnimationClip(unsigned int animationIndex, double ticksPerSec, double startTicks, double endTicks, Animator* animator)
			: m_animation_index(animationIndex),
			m_ticks_per_second(ticksPerSec),
			m_start_ticks(startTicks),
			m_end_ticks(endTicks),
			m_animator(animator)
		{
		}
	};

	enum ModelLoaderFlags
	{
		// Assimp flags
		MOLF_triangulate = 0x000001,
		MOLF_smoothNormals = 0x000002,
		MOLF_faceNormals = 0x000004,
		MOLF_flipUVs = 0x000008,
		MOLF_limitBoneWeights = 0x000010,
		MOLF_calcTangentSpace = 0x000020,
		// vertex-data flags
		MOLF_loadNormals = 0x010000,
		MOLF_loadColors = 0x020000,
		MOLF_loadTexCoords = 0x040000,
		MOLF_loadTexCoords2 = 0x080000,
		MOLF_loadTexCoords3 = 0x100000,
		MOLF_loadBones = 0x200000,
		// additional feature flags
		MOLF_includeAdjacentEdge = 0x400000,
		// the default flags
		MOLF_default = MOLF_triangulate | MOLF_smoothNormals | MOLF_limitBoneWeights,
	};

	struct MeshUniformSettersForShader
	{
		GLuint m_shader_handle;
		std::vector<std::tuple<MeshRef, UniformSetter>> m_mesh_uniform_setters;
		MeshUniformSettersForShader() : m_shader_handle{0}, m_mesh_uniform_setters{} {}
	};
	
	struct MeshVaosForAttribConfig
	{
		VertexAttribData m_vertex_attrib_config;
		std::vector<std::tuple<MeshRef, VAOType>> m_mesh_vaos;
		MeshVaosForAttribConfig() : m_vertex_attrib_config{VertexAttribData::Nothing}, m_mesh_vaos{} {}
	};

	void Append(MeshUniformSettersForShader& unisetters, const MeshUniformSettersForShader& unisetters_to_append);
	void Append(MeshVaosForAttribConfig& vaos, const MeshVaosForAttribConfig& vaos_to_append);
	MeshUniformSettersForShader Concatenate(const MeshUniformSettersForShader& unisetters, const MeshUniformSettersForShader& unisetters_to_append);
	MeshVaosForAttribConfig Concatenate(const MeshVaosForAttribConfig& vaos, const MeshVaosForAttribConfig& vaos_to_append);

	class Model;
	class MeshEditor;
	class Mesh
	{
		friend class Model;
		friend class MeshEditor;

		MeshIdx m_index;
		std::string m_name;

		VertexAttribData m_vertex_data_layout;

		GLuint m_vertex_data_vbo_id;
		GLuint m_indices_vbo_id;

		/// stores different VAOs for different VertexData-combinations
		VAOMap m_vertex_array_objects;

		std::vector<char> m_vertex_data;
		std::vector<GLuint> m_indices;
		GLuint m_indices_len;

		size_t m_size_one_vertex;

		size_t m_position_offset;
		size_t m_normal_offset;
		size_t m_tex_coords_offset;
		size_t m_color_offset;
		size_t m_bone_incides_offset;
		size_t m_bone_weights_offset;
		size_t m_tangent_offset;
		size_t m_bitangent_offset;

		size_t m_position_size;
		size_t m_normal_size;
		size_t m_tex_coords_size;
		size_t m_color_size;
		size_t m_bone_indices_size;
		size_t m_bone_weights_size;
		size_t m_tangent_size;
		size_t m_bitangent_size;

		glm::mat4 m_scene_transformation_matrix;

		std::shared_ptr<MaterialData> m_material_data;

	public:
		// Constructor - initialize everything
		Mesh() :
			m_index(-1),
			m_vertex_data_layout(VertexAttribData::Nothing),
			m_vertex_data_vbo_id(0),
			m_indices_vbo_id(0),
			m_indices_len(0),
			m_size_one_vertex(0),
			m_position_offset(0),
			m_normal_offset(0),
			m_tex_coords_offset(0),
			m_color_offset(0),
			m_bone_incides_offset(0),
			m_bone_weights_offset(0),
			m_tangent_offset(0),
			m_bitangent_offset(0),
			m_position_size(0),
			m_normal_size(0),
			m_tex_coords_size(0),
			m_color_size(0),
			m_bone_indices_size(0),
			m_bone_weights_size(0),
			m_tangent_size(0),
			m_scene_transformation_matrix(glm::mat4(1.0f)),
			m_bitangent_size(0)
		{}

		MeshIdx index() const { return m_index; }
		const std::string& name() const { return m_name; }
		GLuint indices_length() const { return m_indices_len; } 
		const glm::mat4& transformation_matrix() const { return m_scene_transformation_matrix; }
		const std::shared_ptr<MaterialData>& material_data() const { return m_material_data; }
		std::shared_ptr<MaterialData>& material_data() { return m_material_data; }

		void set_material_data(std::shared_ptr<MaterialData> mat_data) { m_material_data = std::move(mat_data); }

		static VAOType GetOrCreateVAOForShader(Mesh& mesh, const Shader& shader);
		static VAOType GetOrCreateVAOForVertexAttribConfig(Mesh& mesh, VertexAttribData vertexDataConfig);
	};

	class MeshEditor
	{
	protected:
		unsigned int modelLoaderFlags;
		// helper functions
		GLuint GetIndex(Mesh &mesh, size_t listIdx) { return mesh.m_indices.at(listIdx); }
		glm::vec3 GetPosition(Mesh &mesh, GLuint listIdx) { return *reinterpret_cast<glm::vec3*>(&mesh.m_vertex_data[listIdx * mesh.m_size_one_vertex]); }
		size_t IndicesCount(Mesh &mesh) { return mesh.m_indices.size(); }
		void SetIndices(Mesh &mesh, std::vector<GLuint> newIndices) {
			mesh.m_indices = newIndices;
			mesh.m_indices_len = static_cast<GLuint>(newIndices.size());
		}
	public:
		void SetModelLoaderFlags(unsigned int modelLoaderFlags) { this->modelLoaderFlags = modelLoaderFlags; }
		virtual void EditIndices(Mesh &mesh) = 0;
	};
	
	class Model
	{
	private:
		static const int kNumFaceVertices = 3;
		static const int kVertexSize = 3;
		static const int kColorSize = 4;
		static const int kNormalSize = 3;
		static const char* kIndent;
		static const glm::vec4 kDefaultDiffuseColor;
		static const glm::vec4 kDefaultSpecularColor;
		static const glm::vec3 kDefaultAmbientColor;
		static const glm::vec3 kDefaultEmissiveColor;
		static const glm::vec4 kDefaultTransparentColor;
		static const float kDefaultSpecularPower;
		static const float kDefaultOpacity;
		static const float kDefaultBumpScaling;
		static const float kDefaultRefraction;
		static const float kDefaultReflectivity;

		std::vector<Mesh> m_meshes;

#ifdef false
		std::vector<Animator*> m_animators;
		std::map<unsigned int, AnimationClip> m_animation_clips;
#endif

		/// The transformation matrix specified while
		glm::mat4 m_load_transformation_matrix;

		static MeshEditor* meshEditor;

	public:
		Model(const glm::mat4& loadTransMatrix = glm::mat4(1.0f));
		Model(const Model& other) = delete;
		Model(Model&& other) noexcept;
		Model& operator=(const Model& other) = delete;
		Model& operator=(Model&& other) noexcept;
		~Model();

	public:
		static std::unique_ptr<Model> LoadFromFile(const std::string& path, const glm::mat4& transform_matrix, const unsigned int model_loader_flags = MOLF_default);

	private:
		bool Load(const std::string& path, const unsigned int modelLoaderFlags = MOLF_default);

		void Dispose();
		bool InitScene(const aiScene* scene, const std::string& path);
		bool InitMesh(const int index, const aiMesh* paiMesh);
		void InitTransformationMatrices(const aiNode* pNode, const aiMatrix4x4& accTrans);

		static void PrintIndent(std::ostream& stream, int indent);
		static void PrintMatrix(std::ostream& stream, const aiMatrix4x4& mat, int indent);
		static void PrintMesh(const aiScene* scene, std::ostream& stream, unsigned int meshIndex, int indent);
		static void PrintNodeRecursively(const aiScene* scene, std::ostream& stream, const aiNode* pNode, const aiMatrix4x4 accTranse, int indent);
		static void PrintVector3D(std::ostream& stream, const aiVector3D& vec);
		static void PrintQuaternion(std::ostream& stream, const aiQuaternion& quat);
		static void PrintVectorKey(std::ostream& stream, const aiVectorKey& key, int indent);
		static void PrintRotationKey(std::ostream& stream, const aiQuatKey& key, int indent);
		static void PrintAnimationChannel(std::ostream& stream, const aiNodeAnim* channel, int indent);
		static void PrintMeshKey(std::ostream& stream, const aiMeshKey& key, int indent);
		static void PrintMeshChannel(std::ostream& stream, const aiMeshAnim* channel, int indent);
		static void PrintBone(std::ostream& stream, const aiBone* bone, int indent);
		static void PrintAnimMesh(std::ostream& stream, const aiAnimMesh* am, int indent);

		static std::string GetTextureNameFromMaterials(const aiScene* scene, unsigned int meshIndex, aiTextureType type);

	public:
		bool BindVAOForMesh(const unsigned int meshIndex, const VertexAttribData vertexDataConfig) const;
		void UnbindVAO() const;

		bool GenerateVAOsWithVertexAttribConfig(VertexAttribData vertexDataConfig);
		bool GenerateVAOsForShader(MeshIdx mesh_index, const Shader& shader);
		VAOType GetOrCreateVAOForMeshForVertexAttribConfig(MeshIdx mesh_index, VertexAttribData vertexDataConfig);
		VAOType GetOrCreateVAOForMeshForShader(MeshIdx mesh_index, const Shader& shader);

		const glm::mat4& transformation_matrix() const;
		const glm::mat4 transformation_matrix(unsigned int meshIndex) const;

		static void PrintNodeTree(const aiScene* scene, std::ostream& stream);
		static void PrintAnimationTree(const aiScene* scene, std::ostream& stream);
		static void PrintMeshes(const aiScene* scene, std::ostream& stream);

		size_t num_meshes() const;
		size_t num_vertices(unsigned int meshIndex) const;
		size_t indices_length(unsigned int meshIndex) const;

		std::string GetMeshName(unsigned int meshIndex) const;
		static std::string GetMeshName(const aiScene* scene, unsigned int meshIndex);
		int Model::GetIndexOfMeshWithName(const aiScene* scene, const std::string& meshName);
		static std::string GetDiffuseTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetSpecularTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetAmbientTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetEmissiveTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetHeightTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetNormalsTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetShininessTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetOpacityTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetDisplacementTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetReflectionTextureName(const aiScene* scene, unsigned int meshIndex);
		static std::string GetLightmapTextureName(const aiScene* scene, unsigned int meshIndex);

		void GatherMaterialData(const aiScene* scene, const std::string& model_path);
		static aiMaterial* GetAssimpMaterialPtr(const aiScene* scene, unsigned int meshIndex);
		MaterialData GetMaterialData(unsigned int meshIndex);

		template<typename Func>
		std::vector<MeshRef> SelectMeshes(Func predicate)
		{
			std::vector<MeshRef> selection;
			size_t n = m_meshes.size();
			for (size_t i = 0; i < n; ++i)
			{
				if (predicate(m_meshes[i]))
				{
					selection.push_back(m_meshes[i]);
				}
			}
			return selection;
		}

		std::vector<MeshRef> SelectAllMeshes()
		{
			return SelectMeshes([](const Mesh& mesh) { return true; });
		}

		static MeshUniformSettersForShader CompileUniformSetters(const Shader& shader, const std::vector<MeshRef>& meshes);

		static MeshVaosForAttribConfig GetOrCreateVAOs(const Shader& shader, const std::vector<MeshRef>& meshes);

		Mesh& mesh_at(unsigned int meshIndex);

#ifdef false
		bool HasAnimationWithIndex(const unsigned int animationIndex);
		double GetTicksPerSecondOfAnimation(const unsigned int animationIndex);
		double GetDurationOfAnimationTicks(const unsigned int animationIndex);
		bool AddAnimationClip(const unsigned int name, const unsigned int animationIndex, const double defaultTicksPerSecond = 0.0);
		bool AddAnimationClip(const unsigned int name, const unsigned int animationIndex, const double startTimeTicks, const double endTimeTicks, const double defaultTicksPerSecond = 0.0);
		bool HasAnimationClips() const;
		const AnimationClip* GetAnimationClip(const unsigned int name);
		Animator* GetAnimatorForAnimationIndex(const unsigned int animationIndex);
#endif

		static bool GetBoneWeightsAndIndicesForMeshVertex(const aiScene* scene, const unsigned int meshIndex, const unsigned int vertexId, glm::uvec4& outBoneIndices, glm::vec4& outBoneWeights);
		glm::vec3 GetVertexPosition(const unsigned meshIndex, const unsigned int vertexId) const;
		glm::vec3 GetVertexNormal(const unsigned meshIndex, const unsigned int vertexId) const;
		static void SetMeshEditor(MeshEditor *meshEditor) { Model::meshEditor = meshEditor; }
	};

}
