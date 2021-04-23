#include <iostream>
#include <memory>
#include <random>
#include <string>
#include "Organism.h"
const int GENERATION_SIZE = 1000;
const int MAX_GENERATIONS = 1000;
const int PARENT_COUNT = 20;
const int FITNESS_THRESHOLD = 10; // can do a ratio of correct letters to incorrect ones
const int GENOTYPE_SIZE = 10;
const double MUTATION_RATE = 0.1;
char getRandomChar();
std::string getRandomGenotype();
void geneticAlgorithm();
void evaluate(Organism&);
Organism breed(std::unique_ptr<Organism[]>&);
void mutate(Organism&);
void insertionSort(std::unique_ptr<Organism[]>&);
template <typename T>
T getRandomNumber(int, int);

int main() {
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


		insertionSort(generation);
		for (int i = 0; i < PARENT_COUNT; i++) 		{
			std::cout << generation[i].getFitness() << " ";
		}
		std::cout << std::endl;
		//Sort decending
		//sortDecending(generation) by fitness

		//Print stuff

		if (generation[0].getFitness() >= FITNESS_THRESHOLD) 	{
			break;
		}

		std::unique_ptr<Organism[]> parents = std::make_unique<Organism[]>(PARENT_COUNT);
		for (int p = 0; p < PARENT_COUNT; p++) 		{
			parents[p] = generation[p];
		}
		
		std::unique_ptr<Organism[]> nextGeneration = std::make_unique<Organism[]>(GENERATION_SIZE);
		for (int i = 0; i < GENERATION_SIZE; i++) 		{
			nextGeneration[i] = breed(parents);
		}

		for (int i = 0; i < GENERATION_SIZE; i++) 		{
			mutate(nextGeneration[i]);
		}
		generation.reset();
		generation = std::move(nextGeneration);
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
	std::unique_ptr<char[]> genotype = std::make_unique<char[]>(GENOTYPE_SIZE);

	for (int i = 0; i < GENOTYPE_SIZE; i++) 		{
		Organism parentToInheritFrom = parents[getRandomNumber<int>(0, PARENT_COUNT-1)];
		genotype[i] = parentToInheritFrom.getAlleleAt(i);
	}
	
	return Organism((std::string)genotype.get(), GENOTYPE_SIZE);
}

void mutate(Organism& organ) {
	for (int i = 0; i < GENOTYPE_SIZE; i++) 		{
		if ((getRandomNumber<int>(0,100) / 100.f)  < MUTATION_RATE) 	{
			organ.mutateAlleleAt(i, getRandomChar());
		}
	}
}

std::string getRandomGenotype() {
	thread_local std::string genotype = "";
	for (thread_local int i = 0; i < GENOTYPE_SIZE; i++)
		genotype += getRandomChar();
	
	return genotype;
}
char getRandomChar() { //get random number between 0 < ri <= r
	srand(time(0));
	thread_local std::mt19937 rng(rand()); //seed
	std::uniform_int_distribution<int> generator(65, 90); // min, max
	return static_cast<char>(generator(rng));
}

template<typename T>
T getRandomNumber(int min, int max) {
	srand(time(0));
	thread_local std::mt19937 rng(rand()); //seed
	std::uniform_int_distribution<T> generator(min, max); // min, max
	return generator(rng);
}

void insertionSort(std::unique_ptr<Organism []>& generation) {

	for (int i = 1; i < GENERATION_SIZE; i++) 		{
		int j = i;
		Organism temp = generation[j];

		while (j > 0 && temp.getFitness() > generation[j-1].getFitness()) 	{
			generation[j] = generation[j - 1];
			j--;
			generation[j] = temp;
		}
	}
}