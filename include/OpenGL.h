#pragma once

#include "Types.cpp"
#include "File.cpp"
#include "List.cpp"

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

uint32 load_shaders(const char* vertexPath, const char* fragmentPath) {
  uint32 vertexShader   = glCreateShader(GL_VERTEX_SHADER);
  uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  uint32 program = glCreateProgram();

  int result;
  int info;

  std::string vSourceStr = load_file_as_string(vertexPath);
  std::string fSourceStr = load_file_as_string(fragmentPath);

  const char* vertexSource = vSourceStr.c_str();
  const char* fragmentSource = fSourceStr.c_str(); 

  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &info);
  if(info > 0) {
    char error[info + 1];
    glGetShaderInfoLog(vertexShader, info, NULL, error);
    printf("VERTEX: %s\n", error);
  }
  
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &info);
  if(info > 0) {
    char error[info + 1];
    glGetShaderInfoLog(fragmentShader, info, NULL, error);
    printf("FRAGMENT: %s\n", error);
  }  

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program; 
}

struct Blueprint {
  uint32 vertexBuffer;
  uint32 program;
  uint32 vertexCount;
  glm::vec3 color;
};

static List<Blueprint*> blueprints;

Blueprint* create_blueprint(float* vertices, int32 size, const glm::vec3& color, int32 vertexCount) {
  Blueprint* bp = (Blueprint*)malloc(sizeof(Blueprint));
  bp->color = color;
  bp->vertexCount = vertexCount;

  bp->program = load_shaders("res/shaders/defaultV.shader", "res/shaders/defaultF.shader");
  glGenBuffers(1, &bp->vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, bp->vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  return bp;
}

void triangle(float x, float y, float scale, const glm::vec3& color) {
  scale *= 50;

  float vertices[] = {
    x - scale, y - scale,
    x + scale, y - scale,
    x,         y + scale,
  };

  Blueprint* bp = create_blueprint(vertices, sizeof(vertices), color, 3);
  blueprints.insert(bp);
}

void rect(float x, float y, float scale, const glm::vec3& color) {
  scale *= 50;

  float vertices[] = {
    x - scale, y - scale,
    x + scale, y - scale,
    x + scale, y + scale,
    x - scale, y - scale,
    x - scale, y + scale,
    x + scale, y + scale,
  };

  Blueprint* bp = create_blueprint(vertices, sizeof(vertices), color, 6);
  blueprints.insert(bp);
}

void opengl_init() {
  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    printf("glew is bogged!");
  }

  //for alpha when we get to it
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void opengl_end() {
  blueprints.free_list();
}

void opengl_render(int32 w, int32 h, const glm::vec3& color) {
  glClearColor(color.x, color.y, color.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glViewport(0.0f, 0.0f, w, h);
  glm::mat4 proj = glm::ortho(0.0f, (float)w, 0.0f, (float)h);

  Node<Blueprint*>* current = blueprints.head;
  while(current) {
    Blueprint* obj = current->data;

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glUseProgram(obj->program);
    
    float r = obj->color.x;
    float g = obj->color.y;
    float b = obj->color.z;

    glUniform3f(glGetUniformLocation(obj->program, "objColor"), r, g, b);
    glUniformMatrix4fv(glGetUniformLocation(obj->program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glDrawArrays(GL_TRIANGLES, 0, obj->vertexCount);
  
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    
    current = current->next;
  }

  glFlush();
}
