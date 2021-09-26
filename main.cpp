/*
* @author orestescm76
* @brief main
* VERSION 0.3.1
*/

#include <iostream>
#include "Renderer.h"
//en este orden que si no la jodemos
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//callback de redibujar
void window_refresh_callback(GLFWwindow* window)
{
	PAG::Renderer::getInstancia()->refrescar_ventana();
	glfwSwapBuffers(window);
	//std::cout << "Ventana redibujada" << std::endl;
}
//callback al redimensionar ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	PAG::Renderer::getInstancia()->configurar_viewport(width, height);
	//std::cout << "Ventana redimensionada: " << width << "x" << height << std::endl;
}
//callback al pulsar una tecla
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	//std::cout << "Key callback called" << std::endl;
}
//callback de botones del rat�n
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
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
	//std::cout << "Movida la rueda del raton " << xoffset
	//	<< " Unidades en horizontal y " << yoffset << " unidades en vertical"
	//	<< std::endl;
	PAG::Renderer::getInstancia()->cambiar_color(yoffset);
	window_refresh_callback(window);
}

int main()
{
	// - Inicializa GLFW. Es un proceso que s�lo debe realizarse una vez en la aplicaci�n
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
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
	window = glfwCreateWindow(1024, 576, "PAG2122-[ColominaMonsalve-Orestes]", nullptr, nullptr);
	//�sali� bien?
	if (!window)
	{
		std::cout << "Failed to open GLFW window" << std::endl;
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
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	std::cout << "Iniciando PAG2122 " << PAG::Renderer::getInstancia()->version << std::endl;
	PAG::Renderer::getInstancia()->imprimirInformacion();
	//registrar callbacks
	glfwSetWindowRefreshCallback(window, window_refresh_callback);	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	//establecer un gris
	PAG::Renderer::getInstancia()->configurar_color();
	//configurar zbuffer y tal
	PAG::Renderer::getInstancia()->inicializar();

	//Ciclo de eventos
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	window = nullptr;
	glfwTerminate();
	std::cout << "PAR2122 is finished!" << std::endl;
	std::cin.get();
}
