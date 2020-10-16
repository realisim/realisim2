
#include "args/args.hxx"
#include "Core/Unused.h"
#include <iostream>
#include "Geometry/Loader/ObjLoader.h"
#include "Geometry/Mesh.h"
#include "TextureUnwrapper.h"

using namespace Realisim;
using namespace std;


int main(int argc, char** argv) {
    args::ArgumentParser parser("Texture unwrapper. Will unwrap the 3d object files and create a packed texture.", 
        "Examples:\n");
    args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
    args::ValueFlag<string> filePathFlag(parser, "filePath", "Specifies the file path to the .obj file.", { 'f', "filePath" });

    try
    {
        parser.ParseCLI(argc, argv);

        if (filePathFlag)
        {
            Geometry::ObjLoader objLoader;
            Geometry::Mesh *pMesh = objLoader.load(args::get(filePathFlag));

            if (!objLoader.hasErrors())
            {

            }
            else
            {
                cout << "objLoader errors: " + objLoader.getAndClearLastErrors() << endl;
            }

            UNUSED(pMesh);
            //printf("%d", (int)pMesh);
            TextureUnwrapper tu;
            //tu.unwrap(*pMesh);

        }
    }
    catch (args::Help)
    {
        cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        cerr << e.what() << endl;
        cerr << parser;
        return 1;
    }
    return 0;
}