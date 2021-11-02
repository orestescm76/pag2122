/*
* @author orestescm76
* @brief renderer cpp
*/

#include "pch.h"
#include "Renderer.h"

PAG::Renderer* PAG::Renderer::instance = nullptr;
const std::string PAG::Renderer::version = "0.7.0";

PAG::Renderer::Renderer()
{
	camera = Camera();
	try
	{
		sp = std::make_shared<ShaderProgram>("pag07-vs.glsl", "pag07-fs.glsl");
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::Renderer -> " + (std::string)e.what());
	}
	mat = Material({ 0,0,0 }, glm::vec3(0.263, 0.149, 0.596), { 0,0,0 }, 0.0f);
	triangle = std::make_unique <Model>(sp, PAG::ModelType::TRIANGLE, mat);
	tetrahedron = std::make_unique<Model>(sp, PAG::ModelType::TETRAHEDRON, mat);
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;
}


PAG::Renderer::~Renderer()
{
	//std::unique_ptr and std::shared_ptr destroys on its own and we do not need to delete it manually
}

PAG::Renderer* PAG::Renderer::getInstance()
{
	if (!instance)
	{
		try
		{
			instance = new Renderer();
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Renderer::getInstance() -> " + (std::string)e.what());
		}
	}
	return instance;
}

void PAG::Renderer::refreshWindow()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	draw();
}

void PAG::Renderer::changeColor(double yoffset)
{
	if (yoffset > 0)
		addColor();
	else minusColor();
	configBackColor(r, g, b, a);
}

void PAG::Renderer::start()
{
	configBackColor(r,g,b,a);
	activeZBuffer();
	//Polil�nea, s�lidos de revoluci�n
	//glPrimitiveRestartIndex(0xFFFF);
	//glEnable(GL_PRIMITIVE_RESTART);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_MULTISAMPLE);
}

void PAG::Renderer::activeZBuffer()
{
	// - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
	//   No tiene por qu� ejecutarse en cada paso por el ciclo de eventos.
	glEnable(GL_DEPTH_TEST);
}

void PAG::Renderer::addColor()
{
	r += .05f;
	g += .05f;
	b += .05f;
	if (r >= 1.0f)
		r = 1.0f;
	if (g >= 1.0f)
		g = 1.0f;
	if (b >= 1.0f)
		b = 1.0f;
}

void PAG::Renderer::minusColor()
{
	r -= .05f;
	g -= .05f;
	b -= .05f;
	if (r <= 0.0f)
		r = 0.0f;
	if (g <= 0.0f)
		g = 0.0f;
	if (b <= 0.0f)
		b = 0.0f;
}

void PAG::Renderer::configBackColor(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a)
{
	glClearColor(_r, _g, _b, _a);
}

void PAG::Renderer::printInfo()
{
	std::string renderer((const char*)glGetString(GL_RENDERER));
	std::string vendor = (const char*)glGetString(GL_VENDOR);
	std::string openglversion = (const char*)glGetString(GL_VERSION);
	std::string shadingVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "RENDERER: " + renderer);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "VENDOR: " + vendor);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "VERSION: " + openglversion);
	Log::getInstance()->printMessage(PAG::msgType::INFO, "SHADING LANGUAGE VERSION: " + shadingVersion);
}

void PAG::Renderer::draw()
{
	if (!triangle.get() && drawingTriangle) //If it's destroyed and we need to draw
		triangle = std::make_unique<Model>(sp, PAG::ModelType::TRIANGLE, mat);

	if (!tetrahedron.get() && !drawingTriangle) //If it's destroyed and we need to draw
	{
		tetrahedron = std::make_unique<Model>(sp, PAG::ModelType::TETRAHEDRON, mat);
	}
		

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = camera.getProjMatrix();
	//Multiply the matrices
	glm::mat4 projview = proj * view;
	try
	{
		sp->getVertexShader().setUniformMat4("matProjView", projview);
		if (drawingTriangle)
		{
			sp->getFragmentShader().setUniformVec3("diffuse", triangle->getMaterial().diffuse);
			//We need to use the shaderprogram beforehand
			triangle->useProgram();
			triangle->setDrawingMode(renderType);
			triangle->draw();
		}
		else
		{
			sp->getFragmentShader().setUniformVec3("diffuse", tetrahedron->getMaterial().diffuse);
			tetrahedron->useProgram();
			tetrahedron->setDrawingMode(renderType);
			tetrahedron->draw();

		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("PAG::Renderer::draw() -> " + (std::string)e.what());
	}


}

void PAG::Renderer::erase()
{
	if (triangle.get()) //if there is a triangle
		triangle.reset(); //Destroy the triangle, but do it once!
	
	if (tetrahedron.get())
		tetrahedron.reset();
}

void PAG::Renderer::configViewport(int width, int height)
{
	wViewport = width;
	hViewport = height;
	camera.setViewport(width, height);
	glViewport(0, 0, width, height);
}

bool PAG::Renderer::isDrawingTriangle()
{
	return drawingTriangle;
}

void PAG::Renderer::setDrawingTriangle(bool draw)
{
	drawingTriangle = draw;
}

void PAG::Renderer::setRenderType(RenderType rt)
{
	renderType = rt;
}

PAG::Camera& PAG::Renderer::getCamera()
{
	return camera;
}
