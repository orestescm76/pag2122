#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace PAG
{
	class Renderer
	{
	private:
		static Renderer* instancia;
		GLfloat r, g, b, a;
		//IDENTIFICADORES
		GLuint idVS = 0;
		GLuint idFS = 0;
		GLuint idSP = 0;
		GLuint idVAO = 0;
		GLuint idVBO = 0;
		GLuint idVBOColor = 0;
		GLuint idIBO = 0;
		Renderer();
		virtual ~Renderer();
		void configBackColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	public:
		static const std::string version;
		// Lo �nico que necesito est�tico de la clase Renderer es la instancia y el m�todo de la instancia
		// ya que as� o creo una instancia y hago render->getInstancia() o PAG::Renderer::getInstancia(), lo mismo da.
		static Renderer* getInstance(); 
		void refreshWindow();
		void changeColor(double yoffset);
		void start();
		void activeZBuffer();
		void addColor();
		void minusColor();
		void configViewport(int width, int height);
		void createModel();
		void createShader(std::string vsfile, std::string fsfile);
		std::string loadShader(std::string file);
		//Checks and returns the error log messages if there's anything wrong.
		void checkErrors(GLint status, GLint id, std::string msg, bool isShader);
		void printInfo();
	};
}
