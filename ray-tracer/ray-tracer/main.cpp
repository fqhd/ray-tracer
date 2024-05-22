#include "camera.hpp"
#include "workbench.hpp"
#include "photorealistic.hpp"

int main()
{
	Scene scene;
	std::cout << "Which type of renderer do you want to start?" << std::endl;
	std::cout << "1) Workbench" << std::endl;
	std::cout << "2) Photorealistic" << std::endl;
	std::cout << "Input: ";
	std::string userinput;
	while (true) {
		std::getline(std::cin, userinput);
		if (userinput == "1") {
			Workbench workbench(&scene);
			workbench.finish();
			return 0;
		}
		else if (userinput == "2") {
			Photorealistic photorealistic;
			photorealistic.finish(&scene);
			return 0;
		}
		else {
			std::cout << "That is not a valid option, please type either 1 or 2." << std::endl;
			std::cout << "Input: ";
		}
	}
}