
#include <cassert>
#include "Core/FileInfo.h"
#include "Core/Path.h"
#include <memory>
#include <regex>
#include "Scene.h"
#include "3d/Scene/SceneNode.h"

// temporary until we can read scene from files...
#include "DataStructure/Scene/GeometryNodes.h"
#include "DataStructure/Scene/LightNode.h"
#include "DataStructure/Scene/MaterialNode.h"
#include "3d/Loader/ObjLoader.h"
#include "Geometry/PlatonicSolid.h"

using namespace Realisim;
    using namespace Core;
    using namespace LightBeam;
    using namespace Math;
    using namespace ThreeD;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
Scene::Scene() :
    mCamera()
{}

//---------------------------------------------------------------------------------------------------------------------
Scene::~Scene()
{
}

//-----------------------------------------------------------------------------
void Scene::addNode(std::shared_ptr<ISceneNode> ipNode)
{
    mNodes.push_back(ipNode);

    switch (ipNode->getNodeType())
    {
    case ISceneNode::ntSceneNode: break;
    case ISceneNode::ntRenderable:
    {
        shared_ptr<IRenderable> c = dynamic_pointer_cast<IRenderable>(ipNode);
        if (c)
        {
            mRenderables.push_back(c);
        }
    } break;
    case ISceneNode::ntLight:
    {
        shared_ptr<ILightNode> c = dynamic_pointer_cast<ILightNode>(ipNode);
        if (c)
        {
            mLights.push_back(c);
        }
    }break;
    case ISceneNode::ntMaterial:
    {
        shared_ptr<MaterialNode> c = dynamic_pointer_cast<MaterialNode>(ipNode);
        if (c)
        {
            mMaterials.push_back(c);
        }
    }break;
    default: assert(0); break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
void Scene::createDummyScene()
{
    //------------------------------------
    // add some crap to the scene...
    Rendering::Camera &cam = getCameraRef();
    cam.set(Vector3(10.0, 20.0, 30.0),
        Vector3(0.0, 0.0, 0.0),
        Vector3(0.0, 1.0, 0.0));

    // add a plane
    Geometry::Plane p(Vector3(0.0, -5, 0.0), Vector3(0.0, 1.0, 0.0));
    shared_ptr<PlaneNode> pn = make_shared<PlaneNode>();
    pn->setPlane(p);
    shared_ptr<Material> mat0 = make_shared<Material>();
    mat0->setDiffuseColor(ColorRgbF32(1, 0, 0));
    //pn->setMaterial(mat0);

    // add a sphere
    Geometry::Sphere sphere(Vector3(6.0, 0.0, 0.0), 4);
    shared_ptr<SphereNode> sn = make_shared<SphereNode>();
    sn->setSphere(sphere);
    shared_ptr<Material> mat1 = make_shared<Material>();
    mat1->setDiffuseColor(ColorRgbF32(0, 1, 0));
    //sn->setMaterial(mat1);

    // add another sphere
    sphere.setCenter(Vector3(-6, 0.0, 0));
    sphere.setRadius(4);
    shared_ptr<SphereNode> sn2 = make_shared<SphereNode>();
    sn2->setSphere(sphere);
    shared_ptr<Material> mat2 = make_shared<Material>();
    mat2->setDiffuseColor(ColorRgbF32(0, 0, 1));
    //sn2->setMaterial(mat2);

    // add another sphere
    sphere.setCenter(Vector3(70, 0, -50));
    sphere.setRadius(50);
    shared_ptr<SphereNode> sn3 = make_shared<SphereNode>();
    sn3->setSphere(sphere);
    shared_ptr<Material> mat3 = make_shared<Material>();
    mat3->setDiffuseColor(ColorRgbF32(0.9f, 0.9f, 0.9f));
    mat3->setSpecularColor(ColorRgbF32(0.1f, 0.1f, 0.1f));
    mat3->setShininess(1024);
    // mat3->setReflectanceModel(mat3->getReflectanceModel() | Material::rmPerfectSpecular);
    //sn3->setMaterial(mat3);

    // add a light
    Light l;
    l.setType(Light::tDirectionnal);
    l.setDirection(Vector3(0.3, 1.0, 2.0));
    shared_ptr<LightNode> directionalLightNode = make_shared<LightNode>();
    directionalLightNode->setLight(l);

    addNode(pn);
    addNode(sn);
    addNode(sn2);
    addNode(sn3);
    addNode(directionalLightNode);
    //scene.addNode(meshNode);
    //------------------------------------

}

//---------------------------------------------------------------------------------------------------------------------
void Scene::clear()
{
    mNodes.clear();
    mRenderables.clear();
    mLights.clear();
    mMaterials.clear();

    mKeyToImage.clear();
}

//---------------------------------------------------------------------------------------------------------------------
const Rendering::Camera& Scene::getCamera() const
{
    return mCamera;
}

//---------------------------------------------------------------------------------------------------------------------
Rendering::Camera& Scene::getCameraRef()
{
    return mCamera;
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::importObj(const std::string &iFilenamePath)
{
    ThreeD::ObjLoader objLoader;
    ThreeD::ObjLoader::Asset objAsset;
    objAsset = objLoader.load(iFilenamePath);
   
    if (!objLoader.hasErrors())
    {
        map<int, shared_ptr<MaterialNode> > meshIndexToMaterialNode;

        // in obj, image file path are relative to the obj...
        // so lets get the path to the obj.
        FileInfo fi;
        fi.setCached(false);
        fi.setFile(iFilenamePath);
        const string folderPath = fi.getCanonicalPath();

        // add materials
        auto itMeshIndexToMaterial = objAsset.mMeshIndexToMaterial.begin();
        for (; itMeshIndexToMaterial != objAsset.mMeshIndexToMaterial.end(); ++itMeshIndexToMaterial)
        {
            const Material &mat = itMeshIndexToMaterial->second;
            for (int i = 0; i < (int)Material::ImageLayer::ilNumberOfLayers; ++i)
            {
                // load and store images if necessary...
                Material::ImageLayer imageLayer = (Material::ImageLayer)i;
                if (mat.hasImageLayer(imageLayer))
                {
                    const std::string filePath = Path::join(folderPath, mat.getImagePath(imageLayer) );

                    //check if already in store...
                    if (mKeyToImage.find(filePath) == mKeyToImage.end())
                    {
                        Core::Image im;

                        im.setFilenamePath(filePath);
                        im.load();
                        im.setWrapType(Image::wtRepeat);

                        if (im.isValid())
                        {
                            mKeyToImage[filePath] = im;
                        }
                    }
                }
            }

            //create the materialNode.
            shared_ptr<MaterialNode> matNode = make_shared<MaterialNode>();
            matNode->setMaterial(mat);

            // grab the diffuse image from the store.
            //
            if (mat.hasImageLayer(Material::ImageLayer::ilDiffuse))
            {
                const std::string filePath = Path::join(folderPath, mat.getImagePath(Material::ImageLayer::ilDiffuse));
                matNode->setDiffuse(mKeyToImage[filePath]);
            }
                

            // add node to cache and to scene
            //
            meshIndexToMaterialNode[itMeshIndexToMaterial->first] = matNode;
            addNode(matNode);
        }

        // add all meshes
        // add a mesh
        for (int i = 0; i < objAsset.mMeshes.size(); ++i)
        {
            string name = objAsset.mName[i];
            regex matchRe("lightPoint.*");
            if (regex_match(name, matchRe))
            {
                Geometry::Mesh *pMeshToDiscard = objAsset.mMeshes[i];

                Light lightFromObj;
                lightFromObj.setType(Light::tPoint);
                lightFromObj.setPosition(pMeshToDiscard->getVertex(0).mVertex);
                lightFromObj.setAttenuationType(Light::atQuadratic);
                shared_ptr<LightNode> lightNodeFromObj = make_shared<LightNode>();
                lightNodeFromObj->setLight(lightFromObj);
                addNode(lightNodeFromObj);

                delete pMeshToDiscard;

                printf("lightPoint %s added to scene.\n", name.c_str());
            }
            else
            {
                shared_ptr<MeshNode> meshNode = make_shared<MeshNode>();
                
                
                //shared_ptr<ThreeD::Material> mat4 = make_shared<ThreeD::Material>(objAsset.mMeshIndexToMaterial[i]);
                
                meshNode->setMaterialNode(meshIndexToMaterialNode[i]);
                meshNode->setName(name);
                meshNode->setMeshAndTakeOwnership(objAsset.mMeshes[i]);
                addNode(meshNode);

                printf("obj %s added to scene.\n", name.c_str());
            }
        }

        
    }

    // add an ambient light
    Light l;
    l.setType(Light::tAmbient);
    l.setColor( Color(0.2, 0.2, 0.2, 0.0) );
    shared_ptr<LightNode> directionalLightNode = make_shared<LightNode>();
    directionalLightNode->setLight(l);
    
    addNode(directionalLightNode);
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::setCamera(const Rendering::Camera &iCamera)
{
    mCamera = iCamera;
}
//-----------------------------------------------------------------------------
const std::vector< std::shared_ptr<ILightNode> >& Scene::getLights() const
{
    return mLights;
}

//-----------------------------------------------------------------------------
const std::vector< std::shared_ptr<IRenderable> >& Scene::getRenderables() const
{
    return mRenderables;
}
