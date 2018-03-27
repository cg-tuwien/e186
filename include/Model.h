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
		// the default flags
		MOLF_default = MOLF_triangulate | MOLF_smoothNormals | MOLF_limitBoneWeights,
	};

	struct MeshUniformSettersForShader
	{
		GLuint m_shader_handle;
		std::vector<std::tuple<MeshRef, UniformSetter>> m_mesh_uniform_setters;
		MeshUniformSettersForShader() : m_shader_handle{0}, m_mesh_uniform_setters{} {}
		auto num_meshes() const { return m_mesh_uniform_setters.size(); }
	};
	
	struct MeshRenderData
	{
		VertexAttribData m_vertex_attrib_config;
		std::vector<std::tuple<MeshRef, RenderConfig>> m_mesh_render_configs;
		MeshRenderData() : m_vertex_attrib_config{VertexAttribData::Nothing}, m_mesh_render_configs{} {}
		auto num_meshes() const { return m_mesh_render_configs.size(); }
	};

	void Append(MeshUniformSettersForShader& unisetters, const MeshUniformSettersForShader& unisetters_to_append);
	void Append(MeshRenderData& vaos, const MeshRenderData& vaos_to_append);
	MeshUniformSettersForShader Concatenate(const MeshUniformSettersForShader& unisetters, const MeshUniformSettersForShader& unisetters_to_append);
	MeshRenderData Concatenate(const MeshRenderData& vaos, const MeshRenderData& vaos_to_append);

	class Model;
	class Mesh
	{
		friend class Model;

		MeshIdx m_index;
		std::string m_name;

		VertexAttribData m_vertex_data_layout;

		GLuint m_vertex_data_vbo_id;
		GLuint m_indices_vbo_id;

		/// stores different VAOs for different VertexData-combinations
		VAOMap m_vertex_array_objects;

		std::vector<uint8_t> m_vertex_data;
		
		std::vector<GLuint> m_indices;
		/*! Topology of the indices, e.g. GL_TRIANGLES, GL_TRIANGLE_STRIP, etc. */
		GLenum m_topology;
		/*! If indices are intended to be used with GL_PATCHES, this holds the patch size */
		int m_patch_size;

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
		VertexAttribData vertex_data_layout() const { return m_vertex_data_layout; }
		GLuint vertex_data_vbo_handle() const { return m_vertex_data_vbo_id; }
		GLuint index_data_vbo_handle() const { return m_indices_vbo_id; }
		bool has_vertex_data_vbo() const { return 0 != m_vertex_data_vbo_id; }
		bool has_index_data_vbo() const { return 0 != m_indices_vbo_id; }
		bool has_been_uploaded_to_gpu() const { return has_vertex_data_vbo() && has_index_data_vbo(); }
		const std::vector<uint8_t>& vertex_data() const { return m_vertex_data; }
		const std::vector<GLuint>& indices() const { return m_indices; }
		GLuint indices_length() const { return static_cast<GLuint>(m_indices.size()); }
		GLenum topology() const { return m_topology; }
		int patch_size() const { return m_patch_size; }
		const glm::mat4& transformation_matrix() const { return m_scene_transformation_matrix; }
		const std::shared_ptr<MaterialData>& material_data() const { return m_material_data; }
		std::shared_ptr<MaterialData>& material_data() { return m_material_data; }

		glm::vec3 vertex_position_at(size_t index) const;
		glm::vec3 vertex_normal_at(size_t index) const;
		GLuint index_at(size_t index) const;

		void SetVertexData(
			std::vector<uint8_t>&& vertex_data,
			VertexAttribData vertex_data_layout,
			size_t position_offset,
			size_t normal_offset,
			size_t tex_coords_offset,
			size_t color_offset,
			size_t bone_incides_offset,
			size_t bone_weights_offset,
			size_t tangent_offset,
			size_t bitangent_offset,
			size_t position_size,
			size_t normal_size,
			size_t tex_coords_size,
			size_t color_size,
			size_t bone_indices_size,
			size_t bone_weights_size,
			size_t tangent_size,
			size_t bitangent_size);
		void SetVertexData(
			const std::vector<uint8_t>& vertex_data,
			VertexAttribData vertex_data_layout,
			size_t position_offset,
			size_t normal_offset,
			size_t tex_coords_offset,
			size_t color_offset,
			size_t bone_incides_offset,
			size_t bone_weights_offset,
			size_t tangent_offset,
			size_t bitangent_offset,
			size_t position_size,
			size_t normal_size,
			size_t tex_coords_size,
			size_t color_size,
			size_t bone_indices_size,
			size_t bone_weights_size,
			size_t tangent_size,
			size_t bitangent_size);

		void SetIndices(std::vector<GLuint>&& indices, GLenum topology, int patch_size);
		void SetIndices(const std::vector<GLuint>& indices, GLenum topology, int patch_size);

		 /**
		 * \brief Creates VBOs for the vertex data and the index data
		 * \param vertex_data_usage	How the vetex data is intended to be used
		 * \param indices_usage		How the index data is intended to be used
		 * \param force				Force (re-)upload, even if they have already been uploaded to the gpu
		 */
		void CreateAndUploadGpuData(GLenum vertex_data_usage = GL_STATIC_DRAW, GLenum indices_usage = GL_STATIC_DRAW, bool force = false);

		void set_material_data(std::shared_ptr<MaterialData> mat_data) { m_material_data = std::move(mat_data); }
		void set_topology(const GLenum value) { m_topology = value; }
		void set_patch_size(const int value) { m_patch_size = value; }

		static VAOType GetOrCreateVAOForShader(Mesh& mesh, const Shader& shader);
		static VAOType GetOrCreateVAOForVertexAttribConfig(Mesh& mesh, VertexAttribData vertexDataConfig);
		static RenderConfig GetOrCreateRenderConfigForShader(Mesh& mesh, const Shader& shader);
		static RenderConfig GetOrCreateRenderConfigForVertexAttribConfig(Mesh& mesh, VertexAttribData vertexDataConfig);
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

	public:
		Model(const glm::mat4& loadTransMatrix = glm::mat4(1.0f));
		Model(const Model& other) = delete;
		Model(Model&& other) noexcept;
		Model& operator=(const Model& other) = delete;
		Model& operator=(Model&& other) noexcept;
		~Model();

	private:
		static std::unique_ptr<Model> g_full_screen_quad;
	public:
		static std::unique_ptr<Model> LoadFromFile(const std::string& path, const glm::mat4& transform_matrix, const unsigned int model_loader_flags = MOLF_default);
		static std::unique_ptr<Model> LoadFromMemory(const std::string& memory, const glm::mat4& transform_matrix, const unsigned int model_loader_flags = MOLF_default);

	private:
		unsigned static int CompileAssimpImportFlags(const unsigned int modelLoaderFlags);
		bool LoadFromFile(const std::string& path, const unsigned int modelLoaderFlags = MOLF_default);
		bool LoadFromMemory(const std::string& data, const unsigned int modelLoaderFlags = MOLF_default);
		bool PostLoadProcessing(Assimp::Importer& importer, const aiScene* scene, const std::string* file_path_or_null);

		void Dispose();
		bool InitScene(const aiScene* scene);
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

		/*! Uploads all meshes to the GPU which haven't already been uploaded */
		void CreateAndUploadGpuData(GLenum vertex_data_usage = GL_STATIC_DRAW, GLenum indices_usage = GL_STATIC_DRAW);

		bool GenerateVAOsWithVertexAttribConfig(VertexAttribData vertexDataConfig);
		bool GenerateVAOsForShader(MeshIdx mesh_index, const Shader& shader);
		VAOType GetOrCreateVAOForMeshForVertexAttribConfig(MeshIdx mesh_index, VertexAttribData vertexDataConfig);
		VAOType GetOrCreateVAOForMeshForShader(MeshIdx mesh_index, const Shader& shader);
		RenderConfig GetOrCreateRenderConfigForMeshForVertexAttribConfig(MeshIdx mesh_index, VertexAttribData vertexDataConfig);
		RenderConfig GetOrCreateRenderConfigForMeshForShader(MeshIdx mesh_index, const Shader& shader);

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

		static MeshRenderData GetOrCreateRenderData(const Shader& shader, const std::vector<MeshRef>& meshes);

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

		static Mesh& GetFullScreenQuadMesh();
	};

}
