#include "Obstacle.h"

using namespace Orz;
using namespace Ogre;

Obstacle::Obstacle(const std::string &name, Ogre::Vector3 pos, double scale, const std::string &materialName): Actor(name), _pos(pos),
	_scale(scale), _materialName(materialName), _node(NULL), _entity(NULL)
{
}

Obstacle::~Obstacle(void)
{

}

void Obstacle::doEnable()
{
	Ogre::SceneManager* sm = OgreGraphicsManager::getSingleton().getSceneManager();
	_node = sm->getRootSceneNode()->createChildSceneNode(this->getName()+"node", _pos);
	createSphere(this->getName()+"mesh", 200, 64, 64);
	_entity = sm->createEntity(this->getName()+"entity", this->getName()+"mesh");
	_entity->setQueryFlags(0x0);
	//std::cout << _materialName << std::endl;
	_entity->setMaterialName(_materialName);
	_node->attachObject(_entity);
	_node->setScale(Ogre::Vector3(_scale, _scale, _scale));

	enableUpdate();
}

void Obstacle::doDisable()
{
	Ogre::SceneManager* sm = OgreGraphicsManager::getSingleton().getSceneManager();
	_node->detachObject(_entity);
	sm->destroySceneNode(_node);
	sm->destroyEntity(_entity);
}

void Obstacle::doFrame()
{

}

void Obstacle::createSphere(const std::string& strName, const float r, const int nRings, const int nSegments)
{
	MeshPtr pSphere = MeshManager::getSingleton().createManual(strName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh *pSphereVertex = pSphere->createSubMesh();
 
     pSphere->sharedVertexData = new VertexData();
     VertexData* vertexData = pSphere->sharedVertexData;
 
     // define the vertex format
     VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
     size_t currOffset = 0;
     // positions
     vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
     currOffset += VertexElement::getTypeSize(VET_FLOAT3);
     // normals
     vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
     currOffset += VertexElement::getTypeSize(VET_FLOAT3);
     // two dimensional texture coordinates
     vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
     currOffset += VertexElement::getTypeSize(VET_FLOAT2);
 
     // allocate the vertex buffer
     vertexData->vertexCount = (nRings + 1) * (nSegments+1);
     HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
     VertexBufferBinding* binding = vertexData->vertexBufferBinding;
     binding->setBinding(0, vBuf);
     float* pVertex = static_cast<float*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));
 
     // allocate index buffer
     pSphereVertex->indexData->indexCount = 6 * nRings * (nSegments + 1);
     pSphereVertex->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, pSphereVertex->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
     HardwareIndexBufferSharedPtr iBuf = pSphereVertex->indexData->indexBuffer;
     unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));
 
     float fDeltaRingAngle = (Math::PI / nRings);
     float fDeltaSegAngle = (2 * Math::PI / nSegments);
     unsigned short wVerticeIndex = 0 ;
 
     // Generate the group of rings for the sphere
     for( int ring = 0; ring <= nRings; ring++ ) {
         float r0 = r * sinf (ring * fDeltaRingAngle);
         float y0 = r * cosf (ring * fDeltaRingAngle);
 
         // Generate the group of segments for the current ring
         for(int seg = 0; seg <= nSegments; seg++) {
             float x0 = r0 * sinf(seg * fDeltaSegAngle);
             float z0 = r0 * cosf(seg * fDeltaSegAngle);
 
             // Add one vertex to the strip which makes up the sphere
             *pVertex++ = x0;
             *pVertex++ = y0;
             *pVertex++ = z0;
 
             Vector3 vNormal = Vector3(x0, y0, z0).normalisedCopy();
             *pVertex++ = vNormal.x;
             *pVertex++ = vNormal.y;
             *pVertex++ = vNormal.z;
 
             *pVertex++ = (float) seg / (float) nSegments;
             *pVertex++ = (float) ring / (float) nRings;
 
             if (ring != nRings) {
                                // each vertex (except the last) has six indices pointing to it
                 *pIndices++ = wVerticeIndex + nSegments + 1;
                 *pIndices++ = wVerticeIndex;               
                 *pIndices++ = wVerticeIndex + nSegments;
                 *pIndices++ = wVerticeIndex + nSegments + 1;
                 *pIndices++ = wVerticeIndex + 1;
                 *pIndices++ = wVerticeIndex;
                 wVerticeIndex ++;
             }
         }; // end for seg
     } // end for ring
 
     // Unlock
     vBuf->unlock();
     iBuf->unlock();
     // Generate face list
     pSphereVertex->useSharedVertices = true;
 
     // the original code was missing this line:
	 pSphere->_setBounds( AxisAlignedBox( Vector3(-r, -r, -r), Vector3(r, r, r) ), false );
	 pSphere->_setBoundingSphereRadius(r);
         // this line makes clear the mesh is loaded (avoids memory leaks)
	 pSphere->load();
}

const std::string & ObstacleFactory::getTypeName() const
{
	static const std::string typeName("Obstacle");
	return typeName;
}

ObstacleFactory::pointer_type ObstacleFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	Ogre::Vector3 pos = Ogre::Vector3(0.0, 0.0, 0.0);
	double scale = 0.0;
	std::string materialName;
	if(parameter)
	{

		parameter_iterator it = parameter->find("pos");
		if(it!= parameter->end())
			pos = VariantData<Ogre::Vector3>::get(it->second);

		it = parameter->find("scale");
		if(it != parameter->end())
			scale = VariantData<double>::get(it->second);

		it = parameter->find("materialName");
		if(it != parameter->end()){
			materialName = VariantData<std::string>::get(it->second);
		}	

	}

	return pointer_type(new Obstacle(instanceName, pos, scale, materialName));
}