/*
* @author orestescm76
* @brief main
* VERSION 0.6.0a2
* 
*/
#include "pch.h"
#include "Renderer.h"
#include "Log.h"
#include "Model.h"

//callback de redibujar
void window_refresh_callback(GLFWwindow* window)
{
	PAG::Renderer::getInstance()->refreshWindow();
	glfwSwapBuffers(window);
	//std::cout << "Ventana redibujada" << std::endl;
}
//callback al redimensionar ventana - C400, par�metro formal sin referencia
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	PAG::Renderer::getInstance()->configViewport(width, height);
	window;
	//std::cout << "Ventana redimensionada: " << width << "x" << height << std::endl;
}
//callback al pulsar una tecla
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	mods;
	scancode;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	bool drawing = PAG::Renderer::getInstance()->isDrawing();
	if (key == GLFW_KEY_A && !drawing)
		PAG::Renderer::getInstance()->draw();
	else if (key == GLFW_KEY_D && drawing)
		PAG::Renderer::getInstance()->erase();
	else if (key == GLFW_KEY_H)
	{
		std::cout << "'p' for panning right" << std::endl
			<< "'P' for panning left" << std::endl;
	}
	else if (key == GLFW_KEY_P)
	{
		//float angle = PAG::Renderer::getInstance()->getCamera().getPanAngle();
		PAG::Renderer::getInstance()->getCamera().pan(.1, PAG::Direction::RIGHT);

	}
	else
		std::cout << "Press 'h' for the keybinds" << std::endl;
	window_refresh_callback(window);
	//std::cout << "Key callback called" << std::endl;
}
//callback de botones del rat�n
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	mods;
	button;
	window;
	if (action == GLFW_PRESS)
	{
		//std::cout << "Se ha pulsado " << button << std::endl;
	}
	else if (action == GLFW_RELEASE)
	{
		//std::cout << "Se ha soltado " << button << std::endl;
	}
}
//callback rueda rat�n
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	xoffset;
	//std::cout << "Movida la rueda del raton " << xoffset
	//	<< " Unidades en horizontal y " << yoffset << " unidades en vertical"
	//	<< std::endl;
	PAG::Renderer::getInstance()->changeColor(yoffset);
	window_refresh_callback(window);
}

int main()
{
	// - Inicializa GLFW. Es un proceso que s�lo debe realizarse una vez en la aplicaci�n
	if (glfwInit() != GLFW_TRUE)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, "Failed to initialize GLFW");
		return -1;
	}

	// - Definimos las caracter�sticas que queremos que tenga el contexto gr�fico
	//   OpenGL de la ventana que vamos a crear. Por ejemplo, el n�mero de muestras o el
	//   modo Core Profile.
	glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialising con 4 muestras
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // - Esta y las 2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL Core Profile 4.1
	// - Definimos el puntero para guardar la direcci�n de la ventana de la aplicaci�n y la creamos
	GLFWwindow* window;
	// - Tama�o, t�tulo de la ventana, en ventana y no en pantalla completa, sin compartir recursos con otras ventanas.
	window = glfwCreateWindow(1280, 720, "PAG2122-[ColominaMonsalve-Orestes]", nullptr, nullptr);
	//�sali� bien?
	if (!window)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, "Failed to open GLFW window");
		glfwTerminate();
		return -2;
	}
	// - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    //   ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
 	glfwMakeContextCurrent(window);
	// - Ahora inicializamos GLEW.
    //   IMPORTANTE: GLEW debe inicializarse siempre DESPU�S de que se haya
    //               inicializado GLFW y creado la ventana
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, "Failed to initialize GLEW");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	//Start the renderer.
	try
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "Starting PAG 2122 " + PAG::Renderer::getInstance()->version);
	}
	catch (const std::exception& e)
	{
		PAG::Log::getInstance()->printMessage(PAG::msgType::ERROR, e.what());
		return -1;
	}
	PAG::Renderer::getInstance()->configViewport(1280, 720);
	PAG::Renderer::getInstance()->printInfo();
	//registrar callbacks
	glfwSetWindowRefreshCallback(window, window_refresh_callback);	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	//configurar OpenGL
	PAG::Renderer::getInstance()->start();

	//Ciclo de eventos
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
	delete PAG::Renderer::getInstance();
	PAG::Log::getInstance()->printMessage(PAG::msgType::INFO, "PAR2122 is finished!");
	std::cout << "Press enter to continue..." << std::endl;
	std::cin.get();
	return 0;
}
