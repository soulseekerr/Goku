
#include "shader.h"
#include "pathmanager.h"

#include <iostream>

using namespace soul;

bool Shader::isLoaded() const { 
    return isShaderLoaded; 
}

bool Shader::load(const std::string& shader_name) {

    auto shaderPath = PathManager::getInstance().getFolderPath(PathManager::FileType::Shader)->string();
    auto shader_frag_file = std::format("{}/{}.frag", shaderPath, shader_name);
        
    std::cout << "Loading shader " << shader_name << std::endl;

    if (!sf::Shader::isAvailable())
        throw ShaderException("Shader not supported on this machine.");

    std::cout << "Loading from file " << shader_frag_file << std::endl;
        
    if (!shader.loadFromFile(shader_frag_file, sf::Shader::Type::Fragment))
        throw ShaderException("Error while Shader.");

    isShaderLoaded = true;
    return isShaderLoaded;
}