
#include "Geometry/Mesh.h"
#include <string>
#include <vector>

#include "tinyObjLoader/v2.0.0rc7/src/tiny_obj_loader.h"

namespace Realisim
{
namespace Geometry
{
    class ObjLoader
    {
    public:
        ObjLoader();
        ~ObjLoader();
        
        struct Asset
        {
            std::vector<std::string> mName;
            std::vector<Mesh*> mMeshes;
            //std::vector<Material*> mMaterials;
        };

        const std::string getAndClearLastErrors() const;
        bool hasErrors() const;
        Asset load(const std::string &iFilePath);

    protected:
        void addError(const std::string& iE) const;
        void createAsset(const tinyobj::attrib_t &iAttrib, const std::vector<tinyobj::shape_t>& iShapes, Asset *opAsset);

        mutable std::string mErrors;
    };
}

}