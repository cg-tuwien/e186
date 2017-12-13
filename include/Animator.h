/*
---------------------------------------------------------------------------
Open Asset Import Library (ASSIMP)
---------------------------------------------------------------------------

Copyright (c) 2006-2010, ASSIMP Development Team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
contributors may be used to endorse or promote products
derived from this software without specific prior
written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/*
---------------------------------------------------------------------------
This file was modified for the AssimpOpenGLDemo by drivenbynostalgia.com.
---------------------------------------------------------------------------
*/

#ifndef ANIMATOR_H
#define ANIMATOR_H

#define DEFAULT_ANIMATION_TICKS_PER_SECOND 20.0

struct AnimationNode;

//---------------------------------------------------------------------------------
/** Calculates the animated node transformations for a given scene and timestamp.
*
*  Create an current for a aiScene you want to animate and set the current animation
*  to play. You can then have the current calculate the current pose for all nodes
*  by calling Calculate() for a given timestamp. After this you can retrieve the
*  present transformation for a given node by calling GetLocalTransform() or
*  GetGlobalTransform().
*/
class Animator
{
public:
	//----------------------------------------------------------------------------
	/** Constructor for a given scene.
	*
	* The object keeps a reference to the scene during its lifetime, but
	* ownership stays at the caller.
	* @param pScene The scene to animate.
	* @param pAnimIndex Index of the animation to play.
	*/
	Animator(const aiScene* pScene, unsigned int pAnimIndex);
	~Animator();

	//----------------------------------------------------------------------------
	/** Sets the animation to use for playback. This also recreates the internal
	* mapping structures, which might take a few cycles.
	* @param uAnimationIndex Index of the animation in the scene's animation array
	*/
	void SetAnimationIndex(unsigned int uAnimationIndex);

	//----------------------------------------------------------------------------
	/** Calculates the node transformations for the scene. Call this to get
	* up-to-date results before calling one of the getters.
	* Evaluates the animation tracks for a given time stamp. The calculated pose can be retrieved as a
	* array of transformation matrices afterwards by calling GetTransformations().
	* @param lElapsedTime Elapsed time since animation start in ms.
	*/
	void UpdateAnimation(double dElapsedTime, double dTicksPerSecond);

	//----------------------------------------------------------------------------
	/** Calculates the bone matrices for the given mesh.
	*
	* Each bone matrix transforms from mesh space in bind pose to mesh space in
	* skinned pose, it does not contain the mesh's world matrix. Thus the usual
	* matrix chain for using in the vertex shader is
	* @code
	* projMatrix * viewMatrix * worldMatrix * boneMatrix
	* @endcode
	* @param pNode The node carrying the mesh.
	* @param pMeshIndex Index of the mesh in the node's mesh array. The NODE's
	*   mesh array, not  the scene's mesh array! Leave out to use the first mesh
	*   of the node, which is usually also the only one.
	* @return A reference to a vector of bone matrices. Stays stable till the
	*   next call to GetBoneMatrices();
	*/
	const std::vector<aiMatrix4x4>& GetBoneMatrices(const aiNode* pNode, unsigned int pMeshIndex = 0);

	// ----------------------------------------------------------------------------
	/** Retrieves the most recent global transformation matrix for the given node.
	*
	* The returned matrix is in world space, which is the same coordinate space
	* as the transformation of the scene's root node. If the node is not animated,
	* the node's original transformation is returned so that you can safely use or
	* assign it to the node itsself. If there is no node with the given name, the
	* identity matrix is returned. All transformations are updated whenever
	* Calculate() is called.
	* @param pNodeName Name of the node
	* @return A reference to the node's most recently calculated global
	*   transformation matrix.
	*/
	const aiMatrix4x4& GetGlobalTransform(const aiNode* node) const;

private:
	/** Recursively creates an internal node structure matching the
	*  current scene and animation.
	*/
	AnimationNode* CreateNodeTree(aiNode* pNode, AnimationNode* pParent);

	/** Calculates the global transformation matrix for the given internal node */
	void CalculateGlobalTransform(AnimationNode* pInternalNode);

	/** Recursively updates the internal node transformations from the
	*  given matrix array
	*/
	void UpdateTransforms(AnimationNode* pNode, const std::vector<aiMatrix4x4>& vTransforms);

	/** Name to node map to quickly find nodes by their name */
	typedef std::map<const aiNode*, AnimationNode*> NodeMap;
	NodeMap m_mapNodesByName;

	/** Name to node map to quickly find nodes for given bones by their name */
	typedef std::map<const char*, const aiNode*> BoneMap;
	BoneMap m_mapBoneNodesByName;

	/** At which frame the last evaluation happened for each channel.
	* Useful to quickly find the corresponding frame for slightly increased time stamps
	*/
	double m_dLastTime;
	glm::uvec3* m_pLastFramePosition;

	/** Array to return transformations results inside. */
	std::vector<aiMatrix4x4> m_vTransforms;

	/** Identity matrix to return a reference to in case of error */
	aiMatrix4x4 m_mIdentityMatrix;

	const aiScene* m_pScene;
	AnimationNode* m_pRootNode;
	unsigned int m_uCurrentAnimationIndex;
	const aiAnimation* m_pCurrentAnimation;
};

#endif