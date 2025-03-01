#include "Model/Factorial.h"

#include <iostream>
#include <spdlog/spdlog.h>
#include <Eigen/Eigen>
#include <thread>

void doSomethingParallel(const int nbThreads)
{
	// Vecteur pour stocker les objets de thread
	std::vector<std::thread> threads;

	// Cr�er un pool de threads
	for (int i = 0; i < nbThreads; ++i) {
		// Instancier un thread et lui assigner une fonction
		threads.push_back(std::thread(somethingWithEigen, i));
	}

	// Attendre que tous les threads aient termin� leur ex�cution
	for (auto& thread : threads) {
		thread.join();
	}

	spdlog::info("All threads completed\n");

	return;
}

void somethingWithEigen(int threadId)
{
	// D�claration et initialisation de deux matrices
	Eigen::MatrixXd matA(200, 200);
	Eigen::MatrixXd matB(200, 200);

	//matA << 1, 2, 3, 4, 5, 6;
	//matB << 7, 8, 9, 10, 11, 12;

	// Multiplication de matrices
	Eigen::MatrixXd matC = matA * matB;

	spdlog::info("Thread {} completed", threadId ," completed\n");
}
