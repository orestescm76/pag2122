#pragma once
//Includes model and Shader Program
#include "Model.h"
#include "Log.h"
#include "Camera.h"
#include "Light.h"
namespace PAG
{
	class Renderer
	{
	private:

		static Renderer* instance;
		glm::vec4 backColor;
		std::unique_ptr<Model> triangle;
		std::unique_ptr<Model> tetrahedron;
		std::unique_ptr<Model> lightCube;
		std::shared_ptr<ShaderProgram> sp;
		std::shared_ptr<ShaderProgram> spLightCube;
		Camera camera;
		Renderer();
		void configBackColor(glm::vec4 color);
		bool drawingTriangle = true;
		float wViewport, hViewport;
		RenderType renderType = RenderType::SOLID;
		Material copper;
		Light ambL, point, dir;// spot;
		void loadUniforms();
		void activateLight(Light& l);
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
		void printInfo();
		void draw();
		void erase();
		bool isDrawingTriangle();
		void setDrawingTriangle(bool draw);
		void setRenderType(RenderType rt);
		//Camera& getCamera();
		void moveCamera(int key);
		void moveCamera(float xoffset, float yoffset);
		void setCamera(MovType mov);
		void resetCamera();
		void zoomCamera(float yoffset);
		virtual ~Renderer();
	};
}
