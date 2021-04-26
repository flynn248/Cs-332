#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include "Organism.h"
#include "Timer.h"
const int GENERATION_SIZE = 1000;
const int MAX_GENERATIONS = 1000;
const int PARENT_COUNT = 20;
const int FITNESS_THRESHOLD = 68; // can do a ratio of correct letters to incorrect ones
const int GENOTYPE_SIZE = 68;
const double MUTATION_RATE = 0.07;
char getRandomChar();
int getRandomNumber(int, int);
std::string getRandomGenotype();
void geneticAlgorithm();
void mutate(Organism&);
void evaluate(Organism&);
Organism breed(std::unique_ptr<Organism[]>&);
void mergeSort(std::unique_ptr<Organism[]>&, int, int);

void threadEvaluate(std::unique_ptr<Organism[]>&, const int, const int);
void threadBreedAndMutate(std::unique_ptr<Organism[]>&, std::unique_ptr<Organism[]>&, const int, const int);

int main() {
	{
	Timer time;
	geneticAlgorithm();
	std::cout << "End time: ";
	}
	std::cin.get();
	return 0;
}

void geneticAlgorithm() {
	std::vector<std::thread> threads;
	std::unique_ptr<Organism[]> generation = std::make_unique<Organism[]>(GENERATION_SIZE);

	for (int i = 0; i < GENERATION_SIZE; i++)
		generation[i] = Organism(getRandomGenotype(), GENOTYPE_SIZE);
	int itterations = 0;
	for (int g = 0; g < MAX_GENERATIONS; g++) 		{
		itterations++;
		
		threads.emplace_back(&threadEvaluate, std::ref(generation), 0, GENERATION_SIZE / 2);
		threads.emplace_back(&threadEvaluate, std::ref(generation), GENERATION_SIZE / 2, GENERATION_SIZE);

		for (auto& th : threads)
			th.join();
		threads.clear();

		mergeSort(generation, 0, GENERATION_SIZE-1);
		std::cout << "Gen " << (g+1) << ". " <<
			"Fitness: " << generation[0].getFitness() << " ";
		generation[0].printGenotype();

		if (generation[0].getFitness() >= FITNESS_THRESHOLD) 	{
			break;
		}

		std::unique_ptr<Organism[]> parents = std::make_unique<Organism[]>(PARENT_COUNT);
		for (int p = 0; p < PARENT_COUNT; p++) 		{
			parents[p] = generation[p];
		}
		std::unique_ptr<Organism[]> nextGeneration = std::make_unique<Organism[]>(GENERATION_SIZE);
		threads.emplace_back(&threadBreedAndMutate, std::ref(nextGeneration), std::ref(parents), 0, GENERATION_SIZE / 2);
		threads.emplace_back(&threadBreedAndMutate, std::ref(nextGeneration), std::ref(parents), GENERATION_SIZE / 2, GENERATION_SIZE);

		for (auto& th : threads)
			th.join();
		threads.clear();

		generation.reset();
		generation = std::move(nextGeneration);
		
	}
	std::cout << "# itterations: " << itterations << std::endl;
}

void threadEvaluate(std::unique_ptr<Organism[]>& generation, const int start, const int end) {
	for (thread_local int i = start; i < end; i++) {
		evaluate(generation[i]);
	}
}

void threadBreedAndMutate(std::unique_ptr<Organism[]>& nextGeneration, std::unique_ptr<Organism[]>& parents, const int start, const int end) {
	for (thread_local int i = start; i < end; i++) {
		nextGeneration[i] = breed(parents);
	}

	for (thread_local int i = start; i < end; i++) {
		mutate(nextGeneration[i]);
	}
}

void evaluate(Organism& organ) {
	std::string password = "What is the answer to life, the universe, and everything? Answer: 42";

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
		Organism parentToInheritFrom = parents[getRandomNumber(0, PARENT_COUNT-1)];
		genotype[i] = parentToInheritFrom.getAlleleAt(i);
	}
	
	return Organism((std::string)genotype.get(), GENOTYPE_SIZE);
}

void mutate(Organism& organ) {
	for (int i = 0; i < GENOTYPE_SIZE; i++) 		{
		if ((getRandomNumber(0,100) / 100.f)  < MUTATION_RATE) 	{
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
char getRandomChar() { return static_cast<char>(getRandomNumber(32, 122)); }

int getRandomNumber(int min, int max) {
	srand(time(0));
	thread_local std::mt19937 rng(rand()); //seed
	std::uniform_int_distribution<int> generator(min, max); // min, max
	return generator(rng);
}

void mergeSort(std::unique_ptr<Organism[]>& generation, int left, int right) {
	if (left < right) 	{
		int mid = (left + right) / 2;
		mergeSort(generation, left, mid);
		mergeSort(generation, mid + 1, right);
		std::unique_ptr<Organism[]> mergedArray = std::make_unique<Organism[]>(right - left + 1);
		int i = left,
			j = mid + 1,
			k = 0;
		while (i <= mid && j <= right) 	{
			if (generation[i].getFitness() > generation[j].getFitness()) {
				mergedArray[k] = generation[i];
				i++;
			}
			else {
				mergedArray[k] = generation[j];
				j++;
			}
			k++;
		}
		while (i <= mid) 	{
			mergedArray[k] = generation[i];
			i++;
			k++;
		}
		while (j <= right) 	{
			mergedArray[k] = generation[j];
			j++;
			k++;
		}

		int z = 0;
		for (int w = left; w <= right; w++) 		{
			generation[w] = mergedArray[z++];
		}
	}
}