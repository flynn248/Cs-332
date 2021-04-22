#include <iostream>
#include <memory>
#include <random>
#include <string>
#include "Organism.h"
const int GENERATION_SIZE = 100;
const int MAX_GENERATIONS = 1000;
const int PARENT_COUNT = 2;
const int FITNESS_THRESHOLD = 10; // can do a ratio of correct letters to incorrect ones
const int GENOTYPE_SIZE = 10;
const double MUTATION_RATE = 0.05;
char getRandomChar();
std::string getRandomGenotype();
void geneticAlgorithm();
void evaluate(Organism&);
Organism breed(std::unique_ptr<Organism[]>&);
void mutate(Organism&);

int main() {
	for (int i = 0; i < 10; i++) 		{
		std::cout << getRandomChar() << std::endl;
	}
	geneticAlgorithm();
	std::cin.get();
	return 0;
}

void geneticAlgorithm() {
	std::unique_ptr<Organism[]> generation = std::make_unique<Organism[]>(GENERATION_SIZE);

	for (int i = 0; i < GENERATION_SIZE; i++)
		generation[i] = Organism(getRandomGenotype(), GENOTYPE_SIZE);

	for (int g = 0; g < MAX_GENERATIONS; g++) 		{
		
		for (int i = 0; i < GENERATION_SIZE; i++) 		{
			evaluate(generation[i]);
		}

		//Sort decending
		//sortDecending(generation) by fitness

		//Print stuff

		if (generation[0].getFitness() >= FITNESS_THRESHOLD) 	{
			break;
		}

		std::unique_ptr<Organism[]> parents = std::make_unique<Organism[]>(PARENT_COUNT);
		for (int p = 0; p < PARENT_COUNT; p++) 		{
			parents[p] = std::move(generation[p]);
		}
		
		std::unique_ptr<Organism[]> nextGeneration = std::make_unique<Organism[]>(GENERATION_SIZE);
		for (int i = 0; i < GENERATION_SIZE; i++) 		{
			nextGeneration[i] = breed(parents);
		}

		for (int i = 0; i < GENERATION_SIZE; i++) 		{
			mutate(nextGeneration[i]);
		}
		generation.release
		generation = nextGeneration;
	}
}

void evaluate(Organism& organ) {
	std::string password = "HELLOWORLD";

	int fitness = 0;
	for (int i = 0; i < GENOTYPE_SIZE; i++) 		{
		if (password[i] == organ.getAlleleAt(i)) 	{
			fitness++;
		}
	}

	organ.setFitness(fitness);
}

Organism breed(std::unique_ptr<Organism[]>& parents) {
	Organism organ;
	return organ;
}

void mutate(Organism& organ) {
	for (int i = 0; i < GENOTYPE_SIZE; i++) 		{

	}
}

std::string getRandomGenotype() {
	std::string genotype = "";
	for (int i = 0; i < GENOTYPE_SIZE; i++)
		genotype += getRandomChar();
	
	return genotype;
}
char getRandomChar() { //get random number between 0 < ri <= r
	srand(time(0));
	thread_local std::mt19937 rng(rand()); //seed
	std::uniform_int_distribution<int> generator(65, 90); // min, max
	return static_cast<char>(generator(rng));
}