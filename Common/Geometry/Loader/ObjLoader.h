
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

        const std::string getAndClearLastErrors() const;
        bool hasErrors() const;
        std::vector<Mesh*> load(const std::string &iFilePath);

    protected:
        void addError(const std::string& iE) const;
        std::vector<Mesh*> createMeshes(const tinyobj::attrib_t &iAttrib, const std::vector<tinyobj::shape_t>& iShapes);

        mutable std::string mErrors;
    };
}

}