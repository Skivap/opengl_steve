#include "Shape.h"

Shape::Shape(std::string path) {

    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec2> _uvs;
    std::vector<glm::vec3> _normals;

    std::vector<GLuint> _vertexIndices;
    std::vector<GLuint> _uvIndices;
    std::vector<GLuint> _normalIndices;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            _vertices.push_back(vertex);
        }
        else if (prefix == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            _uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            _normals.push_back(normal);
        }
        else if (prefix == "f") {
            for (int i = 0; i < 3; i++) {
                GLuint vertexIndexTmp, uvIndexTmp, normalIndexTmp;
                char slash;
                ss >> vertexIndexTmp >> slash >> uvIndexTmp >> slash >> normalIndexTmp;
                _vertexIndices.push_back(vertexIndexTmp - 1);
                _uvIndices.push_back(uvIndexTmp - 1);
                _normalIndices.push_back(normalIndexTmp - 1);
            }
        }
    }

    file.close();

    for (size_t i = 0; i < _vertices.size(); i++) {
        glm::vec3 vertex = _vertices[i];
        glm::vec2 uv = _uvs[i];
        glm::vec3 normal = _normals[i];

        this->vertices.push_back(vertex.x);
        this->vertices.push_back(vertex.y);
        this->vertices.push_back(vertex.z);
        this->vertices.push_back(uv.x);
        this->vertices.push_back(uv.y);
        this->vertices.push_back(normal.x);
        this->vertices.push_back(normal.y);
        this->vertices.push_back(normal.z);
    }
    
    this->indices = _vertexIndices;
    init();
}

void Shape::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // ====================================================================================================================
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // ====================================================================================================================
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ====================================================================================================================
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Shape::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Shape::terminate() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}