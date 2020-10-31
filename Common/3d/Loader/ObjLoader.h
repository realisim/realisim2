
#include "3d/Material.h"
#include "Geometry/Mesh.h"
#include <string>
#include <vector>

#include "tinyObjLoader/v2.0.0rc7/src/tiny_obj_loader.h"

namespace Realisim
{
namespace ThreeD
{
    class ObjLoader
    {
    public:
        ObjLoader();
        ~ObjLoader();
        
        struct Asset
        {
            std::vector<std::string> mName;
            std::vector<Geometry::Mesh*> mMeshes;
            std::map<int, Material> mMeshIndexToMaterial;
        };

        const std::string getAndClearLastErrors() const;
        bool hasErrors() const;
        Asset load(const std::string &iFilePath);

    protected:
        void addError(const std::string& iE) const;
        void createAsset(const tinyobj::attrib_t &iAttrib, const std::vector<tinyobj::shape_t>& iShapes, const std::vector<tinyobj::material_t> &iMaterials, Asset *opAsset);
        void createMaterials(const std::vector<tinyobj::material_t> &iMaterials, std::vector<Material> *opMaterials);

        mutable std::string mErrors;
    };
}

}