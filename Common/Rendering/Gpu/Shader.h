#pragma once

#include "Core/Color.h"
#include "DataType.h"
#include "Math/VectorI.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>


namespace Realisim
{
namespace Rendering
{
    // This class handles shader program and shader ressource creation/deletion.
    //  
    // Upon creation, a shader is invalid (see isValid()) and has no program id
    // nor shader attached to it.
    //
    // Shader are created by adding sources via addSource() or addSourceFromFile().
    // From there, shaders are compiled. If compilation errors occured, they will
    // be available via hasErrors() and getAndClearLastErrors();
    //
    // Once all shader sources have been added, a call to link() will create the
    // program, attach all shaders and perform linkage. If compilation errors
    // occured, they would be available via hasErrors() and getAndClearLastErrors();
    //
    // clear() will flush all gl resources and isValid() will be false.
    //
    // Introspection: 
    //      It is possible to query the Shader object.
    //          - program id is available via programId()
    //          - shader's id are available via shaderId(ShaderType iSt, int iIndex)
    //              The index is an integer between 0 and getNumberOfShader(ShaderType iSt)
    //              The integer is also returned by addSource() and addSourceFromFile(), so it
    //              can be kept by client code to introspect a specific shader object.
    //          - shader source code can be queried via shaderSource(ShaderType iSt, int iIndex)
    //          - uniforms:
    //              The number of uniform for the program can be queried via getNumberOfUniforms()
    //              The name of a uniform can be queried by index: getUniformName(int iIndex);
    //              The location of a uniform can be queried by name: getUniformLocation(string name)
    //
    // Errors:
    //      Contrary to openGL, errors are kept in a cumulative manner. It is
    //      possible to perform all operation and check for errors only at the end.
    //
    //      Once the Shader object has an error, either at shader compile or linkage,
    //      the object will be invalid and all gl resources will be released. A call
    //      to clear will be necessary to reuse that same Shader object.
    //
    // Ex:
    //      Shader lightShader("Super Lighting");
    //      lightShader.addSourceFromFile(stVertex, "../commonFunctions.vert");
    //      lightShader.addSourceFromFile(stVertex, "../light.vert");
    //      lightShader.addSourceFromFile(stFragment, "../light.frag");
    //      
    //      lightShader.setDefine("SUPER_SHADER","UBER_MAX_COOL");
    //
    //      lightShader.compile();
    //      // Check for errors and handle problems...
    //      if(lightShader.hasErrors())
    //      {
    //          doSomethingWithShaderErrors( lightShader.getAndClearLastErrors() );
    //      }   
    //
    //      lightShader.link();
    //      // Check for errors and handle problems...
    //      if(lightShader.hasErrors())
    //      {
    //          doSomethingWithShaderErrors( lightShader.getAndClearLastErrors() );
    //      }
    //
    //      ... shader is valid...
    //      lightShader.useProgram();
    //      lightShader.setUniform("myUniform", someValue);
    //
    // Note:
    //      quand il y a des erreurs, aucune ressources OpenGL n'est allouées.
    //
    class Shader
    {
    public:
        Shader();
        Shader(const std::string& iName);
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        ~Shader();
        
        int addSource(ShaderType iSt, const std::string& iSource);
        int addSourceFromFile(ShaderType iSt, const std::string& iFilePath);

        void    use();
        void    unuse();

        void clear();
        void compile();

        std::string getAndClearLastErrors() const;
        const std::string& getName() const;
        int getNumberOfShaders(ShaderType) const;
        int getNumberOfUniforms() const;
        inline int getProgramId() const;
        int getShaderId(ShaderType iSt, int iIndex) const;
        std::string getShaderSource(ShaderType iSt, int iIndex) const;
        int getUniformLocation(const std::string& iName) const;
		int getUniformBlockLocation(const std::string& iName) const;
        std::string getUniformName(int iIndex) const;
        bool hasErrors() const;
        bool isValid() const;
        void link();

        void releaseSourceCode();

        void setDefine(std::string define, std::string value);

        void setName(const std::string& iName);                

        // set uniform with string
        //
        void setUniform(const std::string& iUniformName, bool iV);
        void setUniform(const std::string& iUniformName, int iV);
        void setUniform(const std::string& iUniformName, float iV);
        void setUniform(const std::string& iUniformName, double iV);
        void setUniform(const std::string& iUniformName, const Core::ColorRgbUint8& iV);
        void setUniform(const std::string& iUniformName, const Core::ColorRgbaUint8& iV);
        void setUniform(const std::string& iUniformName, const Core::ColorRgbF32& iV);
        void setUniform(const std::string& iUniformName, const Core::ColorRgbaF32& iV);
        void setUniform(const std::string& iUniformName, const Math::Vector2& iV);        
        void setUniform(const std::string& iUniformName, const Math::Vector2i& iV);
        void setUniform(const std::string& iUniformName, const Math::Vector3& iV);
        void setUniform(const std::string& iUniformName, const Math::Vector3i& iV);
        void setUniform(const std::string& iUniformName, const Math::Vector4& iV);
        void setUniform(const std::string& iUniformName, const Math::Matrix4& iV);
        void setUniformAsFloat(const std::string& iUniformName, const double& iV);
        void setUniformAsFloat(const std::string& iUniformName, const Math::Vector2& iV);
        void setUniformAsFloat(const std::string& iUniformName, const Math::Vector3& iV);
        void setUniformAsFloat(const std::string& iUniformName, const Math::Vector4& iV);
        void setUniformAsFloat(const std::string& iUniformName, const Math::Matrix4& iV);
        void setUniformAsFloat(const std::string& iUniformName, const Math::Vector2* iV, const int& n);
        void setUniformAsFloat(const std::string& iUniformName, const Math::Vector3* iV, const int& n);
        void setUniformAsFloat(const std::string& iUniformName, const Math::Vector4* iV, const int& n);
        void setUniformArray(const std::string& iUniformName, int iCount, const int* ipV);
        void setUniformArray(const std::string& iUniformName, int iCount, const float* ipV);
        void setUniformArray(const std::string& iUniformName,int iCount,const double* ipV);
        void setUniformArrayAsFloat(const std::string& iUniformName,int iCount,const double* ipV);
        void setUniformBlockBinding(const std::string& iUniformName,unsigned int uboIndex);

        // set uniform with location
        //
        inline void setUniform(int l, bool iV);
        inline void setUniform(int l, int iV);
        inline void setUniform(int l, float iV);
        inline void setUniform(int l, double iV);
        inline void setUniform(int l, const Core::ColorRgbUint8& iV);
        inline void setUniform(int l, const Core::ColorRgbaUint8& iV);
        inline void setUniform(int l, const Core::ColorRgbF32& iV);
        inline void setUniform(int l, const Core::ColorRgbaF32& iV);
        inline void setUniform(int l, const Math::Vector2& iV);
        inline void setUniform(int l, const Math::Vector2i& iV);
        inline void setUniform(int l, const Math::Vector3& iV);
        inline void setUniform(int l, const Math::Vector3i& iV);
        inline void setUniform(int l, const Math::Vector4& iV);
        inline void setUniform(int l, const Math::Matrix4& iV);
        inline void setUniformAsFloat(int l, const double& iV);
        inline void setUniformAsFloat(int l, const Math::Vector2& iV);
        inline void setUniformAsFloat(int l, const Math::Vector3& iV);
        inline void setUniformAsFloat(int l, const Math::Vector4& iV);
        inline void setUniformAsFloat(int l, const Math::Matrix4& iV);
        inline void setUniformAsFloat(int l, const Math::Vector2* iV, const int& n);
        inline void setUniformAsFloat(int l, const Math::Vector3* iV, const int& n);
        inline void setUniformAsFloat(int l, const Math::Vector4* iV, const int& n);
        inline void setUniformArray(int l, int iCount, const int* ipV);
        inline void setUniformArray(int l, int iCount, const float* ipV);
        inline void setUniformArray(int l,int iCount,const double* ipV);
        inline void setUniformArrayAsFloat(int l,int iCount,const double* ipV);
        inline void setUniformBlockBinding(int l,unsigned int uboIndex);

    protected:
        class ShaderSource
        {
            struct DefineValue{ std::string mOriginal; std::string mModified; };

        protected:
            std::string mSource;
            std::vector<std::pair<std::string,DefineValue>>    mDefinesAndValues;

            int findDefine(const std::string& define);
            void parseDefines();
        public:
            ShaderSource() = default;
            ~ShaderSource() = default;

            ShaderSource(const ShaderSource& s);
            ShaderSource& operator = (const ShaderSource& s);

            std::string getDefineValue(const std::string& define);
            // return source code with defines set
            std::string getShaderSource() const;

            void releaseSourceAndDefine();

            void setDefine(const std::string& define,const std::string& value);
            void setSource(std::string source);
        };

        void addError(const std::string&) const;
        int compileShader(int shaderId,const ShaderSource& shader);

        void parseUniforms();
        void parseUniforms(const std::string& iSource);
        void parseUniformsBlock(const std::string& iSource);
        std::string readShaderFile(const std::string& iFilepath);
        void releaseOpenGlRessources();

        //--- data
        bool mIsValid;
        std::string mName;

        int mProgramId;
        std::vector<std::pair<int,ShaderSource>> mShaderIdToSources[stCount];

        std::unordered_map<std::string,int> mUniformNameToLocation;
        std::unordered_map<std::string,int> mUniformBlockNameToLocation;

        mutable std::string mErrors;
    };

}
}