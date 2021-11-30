
#include "Rendering/Gpu/OpenGlHeaders.h"

#include <fstream>
#include <algorithm>
#include <regex>
#include "Shader.h"
#include <sstream>

using namespace std;
using namespace Realisim;
using namespace Rendering;

//-----------------------------------------------------------------------------
Shader::Shader() :
    mIsValid(false),
    mName("Unnamed"),
    mProgramId(0),
    mErrors()
{
}

//-----------------------------------------------------------------------------
Shader::Shader(const std::string& iName) :
    mIsValid(false),
    mName(iName),
    mProgramId(0),
    mErrors()
{
}

//-----------------------------------------------------------------------------
Shader::~Shader()
{
    clear();
}

std::string Shader::ShaderSource::getShaderSource() const
{
    std::string source = mSource;

    std::string regexStart = "(#define{1})";

    for (auto& defines : mDefinesAndValues)
    {
        std::string define = "(" + defines.first + "{1})";
        std::string value = "(" + defines.second.mOriginal + ")";

        std::string finalReg = regexStart + "\\s+" + define + "\\s+" + value + "\\n{1}";
        // apply the defines
        regex defineRe(finalReg);

        std::string remplace = "$1 $2 " + defines.second.mModified + "\n";
        source = std::regex_replace(source, defineRe, remplace);
    }
    return source;
}

void Shader::ShaderSource::parseDefines()
{
    // will capture (#define)(name)(value)
    //      group:      1       2     3
    //
    regex defineRe("(#define{1})\\s+(\\w+)\\s+(\\w+)\\n{1}");
    smatch m;

    // for each uniform
    //      if not in map, get location and insert.
    //      if in map, do nothing....
    //
    string sourcePars = mSource;
    while (std::regex_search(sourcePars, m, defineRe))
    {
        string define = m[2].str();
        string value = m[3].str();

        int i = findDefine(define);
        if (i == -1)
        {
            DefineValue val;
            val.mOriginal = value;
            val.mModified = value;
            mDefinesAndValues.push_back(std::make_pair(define, val));
        }

        sourcePars = m.suffix().str();
    }
}

void Shader::ShaderSource::setDefine(const std::string& define, const std::string& value)
{
    int i = findDefine(define);

    if (i != -1)
        mDefinesAndValues[i].second.mModified = value;
}

void Shader::ShaderSource::releaseSourceAndDefine()
{
    mDefinesAndValues.clear();
    mSource.clear();
}

Shader::ShaderSource::ShaderSource(const Shader::ShaderSource& s)
    : mSource(s.mSource)
    , mDefinesAndValues(s.mDefinesAndValues)
{
}

Shader::ShaderSource& Shader::ShaderSource::operator = (const Shader::ShaderSource& s)
{
    mSource = s.mSource;
    mDefinesAndValues = s.mDefinesAndValues;
    return *this;
}

std::string Shader::ShaderSource::getDefineValue(const std::string& define)
{
    std::string value = "";
    int i = findDefine(define);

    if (i != -1)
        value = mDefinesAndValues[i].second.mModified;

    return value;
}

void Shader::ShaderSource::setSource(std::string source)
{
    mDefinesAndValues.clear();

    mSource = source;
    this->parseDefines();
}

int Shader::ShaderSource::findDefine(const std::string& define)
{
    int n = -1;
    for (int i = 0; i < mDefinesAndValues.size() && (n == -1); ++i)
    {
        if (mDefinesAndValues[i].first == define)
        {
            n = i;
        }
    }

    return n;
}

//-----------------------------------------------------------------------------
void Shader::addError(const std::string& iError) const
{
    mErrors += hasErrors() ? string("\n") + iError : iError;
}

void Shader::setDefine(std::string define, std::string value)
{
    for (int i = 0; i < stCount; ++i)
    {
        for (auto& shader : mShaderIdToSources[i])
        {
            shader.second.setDefine(define, value);
        }
    }
}

void Shader::releaseSourceCode()
{
    for (int i = 0; i < stCount; ++i)
    {
        for (auto& shader : mShaderIdToSources[i])
        {
            shader.second.releaseSourceAndDefine();
        }
    }
}

//-----------------------------------------------------------------------------
int Shader::addSource(ShaderType iSt, const std::string& iSource)
{
    int r = 0;

    ShaderSource shader;
    shader.setSource(iSource);

    GLenum shaderType = toGlShaderType(iSt);
    int shaderId = (int)glCreateShader(shaderType);

    mShaderIdToSources[iSt].push_back(std::make_pair(shaderId, shader));
    r = (int)mShaderIdToSources[iSt].size() - 1;

    return r;
}

//-----------------------------------------------------------------------------
int Shader::addSourceFromFile(ShaderType iSt, const std::string& iFilepath)
{
    int r = 0;

    //openfile...
    string source = readShaderFile(iFilepath);
    if (!source.empty())
    {
        r = addSource(iSt, source);
    }

    return r;
}

//-----------------------------------------------------------------------------
// flush all gl resources
//
void Shader::clear()
{
    mIsValid = false;

    releaseOpenGlRessources();

    mUniformNameToLocation.clear();
    mUniformBlockNameToLocation.clear();

    mErrors.clear();
}

//-----------------------------------------------------------------------------
// Creates the gl shader resources. 
// If compilation fails, the compilation log will be added to errors and
// the gl resources is released.
//
int Shader::compileShader(int shaderId, const ShaderSource& shader)
{
    // https://software.intel.com/en-us/blogs/2012/03/26/using-ifdef-in-opengl-es-20-shaders
    std::string source = shader.getShaderSource();
    const GLchar* sources[1] = { (const GLchar*)source.c_str() };
    GLint sourcesLength[1] = { (GLint)source.size() };
    glShaderSource(shaderId, 1, sources, sourcesLength);

    // Compile the fragment shader
    glCompileShader(shaderId);

    GLint didCompile;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didCompile);
    if (didCompile == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        // maxlenght containe the length of the info log + 1 to
        // account for a \0.
        //
        std::string infoLog(maxLength, '\0');
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(shaderId);

        ostringstream oss;
        oss << "------------------------------------------\n"
            << "While compiling shader for program:\n"
            << getName() << "\n"
            << source << "\n\n"
            << "The following errors occured:\n"
            << infoLog << "\n";
        addError(oss.str());

        shaderId = 0;
    }
    return (int)shaderId;
}

//-----------------------------------------------------------------------------
// Returns all accumulated errors since last called. As a side effect, it
// clears all accumulated errors.
//
std::string Shader::getAndClearLastErrors() const
{
    string r = mErrors;
    mErrors = string();
    return r;
}

//-----------------------------------------------------------------------------
const std::string& Shader::getName() const
{
    return mName;
}

//-----------------------------------------------------------------------------
// Returns the number of shader of ShaderType
//
int Shader::getNumberOfShaders(ShaderType iSt) const
{
    return (int)mShaderIdToSources[iSt].size();
}

//-----------------------------------------------------------------------------
int Shader::getNumberOfUniforms() const
{
    return (int)mUniformNameToLocation.size();
}

//-----------------------------------------------------------------------------
int Shader::getShaderId(ShaderType iSt, int iIndex) const
{
    int r = 0;
    if (iIndex >= 0 && iIndex < getNumberOfShaders(iSt))
    {
        r = mShaderIdToSources[iSt][iIndex].first;
    }
    return r;
}

//-----------------------------------------------------------------------------
// Returns the shader source as it is on the card.
// Note on performance: 
//  calling this method will hinder performance as the glGet requires to flush
//  the gl command queue.
//
std::string Shader::getShaderSource(ShaderType iSt, int iIndex) const
{
    string r;
    const int id = getShaderId(iSt, iIndex);

    if (id > 0)
    {
        int maxLength = 0;
        glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &maxLength);

        std::string source(maxLength, '\0');
        glGetShaderSource(id, maxLength, &maxLength, &source[0]);

        r = string(source.begin(), source.end());
    }
    return r;
}

//-----------------------------------------------------------------------------
int Shader::getUniformBlockLocation(const std::string& iName) const
{
    int r = -1;
    auto it = mUniformBlockNameToLocation.find(iName);
    if (it != mUniformBlockNameToLocation.end())
    {
        r = it->second;
    }
    return r;
}

//-----------------------------------------------------------------------------
// Returns the location of the uniform iName
// 
//  -1 is returned if the uniform does not exists or if the uniform is not in
//  usage in the shader (the shader compiler can optimize them out...)
//
int Shader::getUniformLocation(const std::string& iName) const
{
    int r = -1;
    auto it = mUniformNameToLocation.find(iName);
    if (it != mUniformNameToLocation.end())
    {
        r = it->second;
    }
    //else
    //{
    //    ostringstream oss;
    //    oss << "Uniform named '" << iName << "' could not be found.";
    //    addError(oss.str());
    //}
    return r;
}

//-----------------------------------------------------------------------------
// Return the name of the uniform at index iIndex.
// an empty string is returned if iIndex is out of bound.
//
std::string Shader::getUniformName(int iIndex) const
{
    string r = "";
    if (iIndex >= 0 && iIndex < getNumberOfUniforms())
    {
        auto it = mUniformNameToLocation.begin();
        std::advance(it, iIndex);
        r = it->first;
    }
    return r;
}

//-----------------------------------------------------------------------------
bool Shader::hasErrors() const
{
    return !mErrors.empty();
}

//-----------------------------------------------------------------------------
bool Shader::isValid() const
{
    return mIsValid;
}

void Shader::compile()
{
    for (int i = 0; i < stCount; ++i)
    {
        for (auto& shader : mShaderIdToSources[i])
        {
            int shaderId = shader.first;
            if (compileShader(shaderId, shader.second) != shaderId)
            {
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Create the program gl resources and performs linkage on all attached shaders
// (via addSource or addSourceFromFile). 
//
// When the linkage succeeds, all shader sources will be parsed to gather the
// uniforms (see getNumberOfUniforms, getUniformLocation, getUniformName).
// Other type of shader introspection could be done here.
//
// Optimization opportunity: Since we do not keep a copy of the shader sources
//  in memory, we query the card when parsing the uniforms, it would be simple
//  to keep the source in memory until sources are parsed.
//
// In case of error: If linkage fails, all gl resources
//  are released and link log is added to errors.
//
void Shader::link()
{
    // early out
    if (hasErrors())
    {
        return;
    }

    mProgramId = (int)glCreateProgram();

    // attach all shaders
    for (int i = 0; i < stCount; ++i)
    {
        for (auto it : mShaderIdToSources[i])
        {
            glAttachShader(mProgramId, it.first);
        }
    }

    // Link our program
    glLinkProgram(mProgramId);

    // check for linkage error
    GLint isLinked = 0;
    glGetProgramiv(mProgramId, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &maxLength);

        string infoLog(maxLength, '\0');
        glGetProgramInfoLog(mProgramId, maxLength, &maxLength, &infoLog[0]);

        ostringstream oss;
        oss << "While linking program:\n"
            << getName() << "\n\n"
            << "The following errors occured:\n"
            << infoLog << "\n";
        addError(oss.str());
    }

    // if errors were encountered while linking, lets
    // release everything. Since we already detach all
    // shader, we are quite safe.
    //
    if (hasErrors())
    {
        releaseOpenGlRessources();
    }
    else
    {
        parseUniforms();

        mIsValid = true;
    }
}

//-----------------------------------------------------------------------------
// This method will parse all shader source and populate a map containing the
// uniform name and its location (glProgramUniformLocation).
//
// Note: this method needs to be called after a program has successfully link
//      or else the location won't be available.
//
void Shader::parseUniforms()
{
    for (int j = 0; j < ShaderType::stCount; ++j)
    {
        for (int i = 0; i < getNumberOfShaders((ShaderType)j); ++i)
        {
            parseUniforms(getShaderSource((ShaderType)j, i));
            parseUniformsBlock(getShaderSource((ShaderType)j, i));
        }
    }
}

//-----------------------------------------------------------------------------
// see method parseUniforms()
//
void Shader::parseUniforms(const std::string& iSource)
{
    // will capture (uniform)(type)(name)([number])
    //      group:      1       2     3     4
    //
    regex uniformRe("(uniform)\\s+(\\w+)\\s+(\\w+)(\\[\\d+\\])?");
    smatch m;

    // for each uniform
    //      if not in map, get location and insert.
    //      if in map, do nothing....
    //
    string source = iSource;
    while (std::regex_search(source, m, uniformRe))
    {
        string name = m[3].str();

        // check if name is an array, if yes resolve it
        int parStart = (int)m[4].str().find('[');
        int parEnd = (int)m[4].str().find(']');
        if ((m.size() == 5) && (parStart != std::string::npos))
        {
            string val = m[4].str().substr(parStart + 1, parEnd - parStart - 1);
            int arraySize = std::stoi(val);

            for (int i = 0; i < arraySize; ++i)
            {
                string valueName = name;
                valueName += "[";
                valueName += std::to_string(i);
                valueName += "]";

                auto foundIt = mUniformNameToLocation.find(valueName);
                if (foundIt == mUniformNameToLocation.end())
                {
                    int l = glGetUniformLocation(getProgramId(), valueName.c_str());
                    mUniformNameToLocation.insert(make_pair(valueName, l));
                }
            }
        }
        else
        {
            auto foundIt = mUniformNameToLocation.find(name);
            if (foundIt == mUniformNameToLocation.end())
            {
                int l = glGetUniformLocation(getProgramId(), name.c_str());
                mUniformNameToLocation.insert(make_pair(name, l));
            }
        }

        source = m.suffix().str();
    }
}

void Shader::parseUniformsBlock(const std::string& iSource)
{
    // will capture (layout) (std410,....) (uniform) (name)
    //      group:      1       2				3		4
    //
    regex uniformRe("(layout)(\\(\\w+\\))\\s+(uniform)\\s+(\\w+)");
    smatch m;

    // for each uniform
    //      if not in map, get location and insert.
    //      if in map, do nothing....
    //
    string source = iSource;
    while (std::regex_search(source, m, uniformRe))
    {
        string name = m[4].str();

        auto foundIt = mUniformBlockNameToLocation.find(name);
        if (foundIt == mUniformBlockNameToLocation.end())
        {
            int l = glGetUniformBlockIndex(getProgramId(), name.c_str());
            mUniformBlockNameToLocation.insert(make_pair(name, l));
        }

        source = m.suffix().str();
    }
}

//-----------------------------------------------------------------------------
std::string Shader::readShaderFile(const std::string& iFilepath)
{
    string source;
    ifstream ifs;
    ifs.open(iFilepath, ios::in | ios::ate);
    if (!ifs.fail())
    {
        auto size = ifs.tellg();

        // create string at the right size
        source.resize(size, '\0');
        ifs.seekg(0, ios::beg);
        ifs.read(&source[0], size);
        ifs.close();

        //remove all trailling \0
        source.erase(std::remove(source.begin(), source.end(), '\0'), source.end());
    }
    else
    {
        ostringstream oss;
        oss << "Could not read file " << iFilepath;
        addError(oss.str());
    }
    return source;
}

//-----------------------------------------------------------------------------
void Shader::releaseOpenGlRessources()
{
    if (getProgramId())
    {
        //detach all shaders
        for (int i = 0; i < stCount; ++i)
        {
            for (auto it : mShaderIdToSources[i])
            {
                glDetachShader(getProgramId(), it.first);
            }
        }

        glDeleteProgram(getProgramId());
        mProgramId = 0;
    }

    //delete all shader
    for (int i = 0; i < stCount; ++i)
    {
        for (auto it : mShaderIdToSources[i])
        {
            glDeleteShader(it.first);
        }
        mShaderIdToSources[i].clear();
    }

    mUniformNameToLocation.clear();
    mUniformBlockNameToLocation.clear();
}

//-----------------------------------------------------------------------------
void Shader::setName(const std::string& iName)
{
    mName = iName;
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const string& iUniformName, bool iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const string& iUniformName, int iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const string& iUniformName, float iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const string& iUniformName, double iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const std::string& iUniformName, const Core::ColorRgbUint8& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const std::string& iUniformName, const Core::ColorRgbaUint8& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const std::string& iUniformName, const Core::ColorRgbF32& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const std::string& iUniformName, const Core::ColorRgbaF32& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const string& iUniformName, const Math::Vector2& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

void Shader::setUniform(const string& iUniformName, const Math::Vector2i& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const string& iUniformName, const Math::Vector3& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

void Shader::setUniform(const string& iUniformName, const Math::Vector3i& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const string& iUniformName, const Math::Vector4& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniform(const string& iUniformName, const Math::Matrix4& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniform(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(const string& iUniformName, const double& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformAsFloat(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(const string& iUniformName, const Math::Vector2& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformAsFloat(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(const string& iUniformName, const Math::Vector3& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformAsFloat(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(const string& iUniformName, const Math::Vector4& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformAsFloat(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(const string& iUniformName, const Math::Matrix4& iV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformAsFloat(l, iV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(const string& iUniformName, const Math::Vector2* iV, const int& n)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformAsFloat(l, iV, n);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(const string& iUniformName, const Math::Vector3* iV, const int& n)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformAsFloat(l, iV, n);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(const string& iUniformName, const Math::Vector4* iV, const int& n)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformAsFloat(l, iV, n);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformArray(const string& iUniformName, int iCount, const int* ipV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformArray(l, iCount, ipV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformArray(const string& iUniformName, int iCount, const float* ipV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformArray(l, iCount, ipV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformArray(const string& iUniformName, int iCount, const double* ipV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformArray(l, iCount, ipV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformArrayAsFloat(const string& iUniformName, int iCount, const double* ipV)
{
    const int l = getUniformLocation(iUniformName);
    if (l != -1)
    {
        setUniformArrayAsFloat(l, iCount, ipV);
    }
}

//-----------------------------------------------------------------------------
void Shader::setUniformBlockBinding(const std::string& iUniformName, unsigned int uboIndex)
{
    const int l = getUniformBlockLocation(iUniformName);
    if (l != -1)
    {
        setUniformBlockBinding(l, uboIndex);
    }
}

//-----------------------------------------------------------------------------------------------------------------
//--- Actual OpenGl calls
//-----------------------------------------------------------------------------
int Shader::getProgramId() const
{
    return mProgramId;
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, bool iV)
{
    glProgramUniform1i(getProgramId(), l, (int)iV);
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, int iV)
{
    glProgramUniform1i(getProgramId(), l, iV);
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, float iV)
{
    glProgramUniform1f(getProgramId(), l, iV);
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, double iV)
{
    glProgramUniform1d(getProgramId(), l, iV);
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Core::ColorRgbUint8& iV)
{
    static const float oneOver255 = 1.f / 255.0f;

    glProgramUniform3f(getProgramId(), l, oneOver255 * iV.getRed(), oneOver255 * iV.getGreen(), oneOver255 * iV.getBlue());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Core::ColorRgbaUint8& iV)
{
    static const float oneOver255 = 1.0f / 255.0f;

    glProgramUniform4f(getProgramId(), l, oneOver255 * iV.getRed(), oneOver255 * iV.getGreen(),
        oneOver255 * iV.getBlue(), oneOver255 * iV.getAlpha());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Core::ColorRgbF32& iV)
{
    glProgramUniform3f(getProgramId(), l, iV.getRed(), iV.getGreen(), iV.getBlue());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Core::ColorRgbaF32& iV)
{
    glProgramUniform4f(getProgramId(), l, iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Math::Vector2& iV)
{
    glProgramUniform2d(getProgramId(), l, iV.x(), iV.y());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Math::Vector2i& iV)
{
    glProgramUniform2i(getProgramId(), l, iV.x(), iV.y());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Math::Vector3& iV)
{
    glProgramUniform3d(getProgramId(), l, iV.x(), iV.y(), iV.z());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Math::Vector3i& iV)
{
    glProgramUniform3i(getProgramId(), l, iV.x(), iV.y(), iV.z());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Math::Vector4& iV)
{
    glProgramUniform4d(getProgramId(), l, iV.x(), iV.y(), iV.z(), iV.w());
}

//-----------------------------------------------------------------------------
void Shader::setUniform(int l, const Math::Matrix4& iV)
{
    glProgramUniformMatrix4dv(getProgramId(), l, 1, false, iV.getDataPointer()); // no transposision
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(int l, const double& iV)
{
    glProgramUniform1f(getProgramId(), l, (float)iV);
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(int l, const Math::Vector2& iV)
{
    glProgramUniform2f(getProgramId(), l, (float)iV.x(), (float)iV.y());
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(int l, const Math::Vector3& iV)
{
    glProgramUniform3f(getProgramId(), l, (float)iV.x(), (float)iV.y(), (float)iV.z());
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(int l, const Math::Vector4& iV)
{
    glProgramUniform4f(getProgramId(), l, (float)iV.x(), (float)iV.y(), (float)iV.z(), (float)iV.w());
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(int l, const Math::Matrix4& iV)
{

    // Here, we fill the float matrix in column major format needed by 
    // openGL. Since Matrix4 presents a row major interface, the
    // indices [i][j] are swapped to [j][i].
    //

    // we cast values to float
    float f[16] = { 0.f };
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            f[i * 4 + j] = (float)iV(j, i);
        }
    }

    glProgramUniformMatrix4fv(getProgramId(), l, 1, false, f);  // no transposision
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(int l, const Math::Vector2* iV, const int& n)
{
    float* temp = new float[2 * n];
    for (int i = 0; i < n; ++i)
    {
        temp[2 * i + 0] = (float)iV[i].x();
        temp[2 * i + 1] = (float)iV[i].y();
    }

    glProgramUniform2fv(getProgramId(), l, n, (const float*)temp);

    delete[] temp;
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(int l, const Math::Vector3* iV, const int& n)
{
    float* temp = new float[3 * n];
    for (int i = 0; i < n; ++i)
    {
        temp[3 * i + 0] = (float)iV[i].x();
        temp[3 * i + 1] = (float)iV[i].y();
        temp[3 * i + 2] = (float)iV[i].z();
    }

    glProgramUniform3fv(getProgramId(), l, n, (const float*)temp);

    delete[] temp;
}

//-----------------------------------------------------------------------------
void Shader::setUniformAsFloat(int l, const Math::Vector4* iV, const int& n)
{
    float* temp = new float[4 * n];
    for (int i = 0; i < n; ++i)
    {
        temp[4 * i + 0] = (float)iV[i].x();
        temp[4 * i + 1] = (float)iV[i].y();
        temp[4 * i + 2] = (float)iV[i].z();
        temp[4 * i + 3] = (float)iV[i].w();
    }

    glProgramUniform4fv(getProgramId(), l, n, (const float*)temp);

    delete[] temp;
}

//-----------------------------------------------------------------------------
void Shader::setUniformArray(int l, int iCount, const int* ipV)
{
    glProgramUniform1iv(getProgramId(), l, iCount, (GLint*)ipV);
}

//-----------------------------------------------------------------------------
void Shader::setUniformArray(int l, int iCount, const float* ipV)
{
    glProgramUniform1fv(getProgramId(), l, iCount, (GLfloat*)ipV);
}

//-----------------------------------------------------------------------------
void Shader::setUniformArray(int l, int iCount, const double* ipV)
{
    glProgramUniform1dv(getProgramId(), l, iCount, ipV);
}

//-----------------------------------------------------------------------------
void Shader::setUniformArrayAsFloat(int l, int iCount, const double* ipV)
{

    // do a copy as float
    std::unique_ptr<float[]>    temp(new float[iCount]);
    for (int i = 0; i < iCount; ++i)
    {
        temp[i] = (float)ipV[i];
    }
    glProgramUniform1fv(getProgramId(), l, iCount, temp.get());
}

//-----------------------------------------------------------------------------
void Shader::setUniformBlockBinding(int l, unsigned int uboIndex)
{
    glUniformBlockBinding(getProgramId(), l, uboIndex);
}


//-----------------------------------------------------------------------------
void    Shader::use()
{
    glUseProgram(getProgramId());
}

//-----------------------------------------------------------------------------
void    Shader::unuse()
{
    glUseProgram(0);
}

//-----------------------------------------------------------------------------
/*void Shader::setUniformAsFloat(const string& iUniformName, const Math::Matrix4* iV, const int& n)
{
const int l = getUniformLocation(iUniformName);
if (l != -1)
{
// Here, we fill the float matrix in column major format needed by
// openGL. Since Matrix4 presents a row major interface, the
// indices [i][j] are swapped to [j][i].
//

// we cast values to float
float f[16] = { 0.f };
for (int i = 0; i < 4; ++i)
{
for (int j = 0; j < 4; ++j)
{
f[i * 4 + j] = (float)iV(j, i);
}
}

glProgramUniformMatrix4fv(getProgramId(), l, 1, false, f);  // no transposision
}
}*/
