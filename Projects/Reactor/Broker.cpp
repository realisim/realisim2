
#include "Broker.h"

#include "Core/FileInfo.h"
#include "Core/Path.h"

#include "DataStructures/Scene/ImageNode.h"
#include "DataStructures/Scene/MaterialNode.h"
#include "DataStructures/Scene/ModelNode.h"

#include "Geometry/Plane.h"
#include "Geometry/PlatonicSolid.h"
#include "Geometry/RectangularPrism.h"

using namespace Realisim;
using namespace Math;
using namespace Reactor;
using namespace ThreeD;

//---------------------------------------------------------------------------------------------------------------------
Broker::Broker()
{
    Core::FileInfo appPath(Core::Path::getApplicationFilePath());
    mAssetPath = Core::Path::join(appPath.getAbsolutePath(), "../Assets");

    mTimeSinceStartup.start();
}

//---------------------------------------------------------------------------------------------------------------------
void Broker::makeBasicScene()
{
    mScene.clear();

    using namespace Geometry;
    Plane plane(Vector3(0), Vector3(0, 1, 0));
    RectangularPrism prism0(Vector3(0, 0, 0), 2, 3, 4);
    RectangularPrism prism1(Vector3(0, 0, 0), 4, 5, 6);

    Matrix4 transfo;

    ImageNode* pGlowImageNode = new ImageNode();
    pGlowImageNode->setName("glowImage");
    Core::Image& glowImage = pGlowImageNode->getImageRef();
    glowImage.set(Core::Path::join(getAssetPath(), "Textures/glowTest1.png"));

    MaterialNode* pGlowMatNode = new MaterialNode();
    pGlowMatNode->setName("glowMat");
    pGlowMatNode->addImageNode(pGlowImageNode, Material::ilDiffuse);

    ModelNode *planeNode = new ModelNode();
    planeNode->setName("plane");
    Mesh m = plane.makeMesh(Vector2(50, 50));
    planeNode->addMesh(m);

    ModelNode* prism0Node = new ModelNode();
    prism0Node->setName("prism0");
    m = prism0.makeMesh();
    prism0Node->addMesh(m);
    prism0Node->setMaterialNode(pGlowMatNode);
    transfo.setAsTranslation(Vector3(0, 3, 0));
    prism0Node->setParentTransform(transfo);

    ModelNode* prism1Node = new ModelNode();
    prism1Node->setName("prism1");
    m = prism1.makeMesh();
    prism1Node->addMesh(m);
    prism1Node->setMaterialNode(pGlowMatNode);
    transfo.setAsTranslation(Vector3(8, 2, 4));
    prism1Node->setParentTransform(transfo);


    // parent all these nodes to the roots
    ThreeD::SceneNode& rootNode = mScene.getRootRef();
    rootNode.addChild(planeNode);
    rootNode.addChild(prism0Node);
    rootNode.addChild(prism1Node);

    ThreeD::SceneNode& matLibrary = mScene.getMaterialLibraryRef();
    matLibrary.addChild(pGlowMatNode);

    ThreeD::SceneNode& imageLibrary = mScene.getImageLibraryRef();
    imageLibrary.addChild(pGlowImageNode);
}