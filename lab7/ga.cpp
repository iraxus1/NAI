//
#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>

using genotype_t = std::vector<char>;
using population_t = std::vector<genotype_t>;
using population_fitness_t = std::vector<double>;

std::random_device random_dev;
std::default_random_engine random_engine(random_dev());
std::uniform_real_distribution<double> double_random_dist(0.0, 1.0);
std::uniform_int_distribution<char> rand_01(0, 1);

//genetic algorithm
population_t ga(population_t population, population_fitness_t fitnesses, int n_generations, int n_parents, double p_m,
                std::function<std::vector<int>(population_fitness_t)> selection,
                std::function<std::pair<genotype_t, genotype_t>(genotype_t, genotype_t)> crossover,
                std::function<genotype_t(genotype_t, double)> mutation) {
    population_t new_population;
    for (int i = 0; i < n_generations; i++) {
        std::vector<int> parents = selection(fitnesses);
        for (int j = 0; j < n_parents; j += 2) {
            auto [a, b] = crossover(population[parents[j]], population[parents[j + 1]]);
            new_population.push_back(mutation(a, p_m));
            new_population.push_back(mutation(b, p_m));
        }
        population = new_population;
        new_population.clear();

        auto replaceElite = [](population_t &population, population_fitness_t &fitnesses, int n_elite) {
            std::vector<std::pair<double, int>> fitnesses_with_index;
            for (int i = 0; i < fitnesses.size(); i++) {
                fitnesses_with_index.emplace_back(fitnesses[i], i);
            }
            std::sort(fitnesses_with_index.begin(), fitnesses_with_index.end(),
                      [](const std::pair<double, int> &a, const std::pair<double, int> &b) {
                          return a.first > b.first;
                      });
            for (int i = 0; i < n_elite; i++) {
                population[i] = population[fitnesses_with_index[i].second];
            }
        };
        replaceElite(population, fitnesses, n_parents);

        for (auto &e: population) {
            fitnesses.push_back(0);
        }

    }
    return population;
}

//selection roulette
std::vector<int> selectionRoulette(population_fitness_t fitness) {
    std::vector<int> ret;
    double sum_fit = std::accumulate(fitness.begin(), fitness.end(), 0.0);
    std::uniform_real_distribution<double> random_distr(0.0, sum_fit);
    for (int j = 0; j < fitness.size(); j++) {
        double rand_selected = random_distr(random_engine);
        double s = fitness[0];
        int selected_i = fitness.size() - 1;
        for (int i = 0; i < fitness.size() - 1; i++) {
            if (s > rand_selected) {
                selected_i = i;
                break;
            }
            s = s + fitness[i + 1];
        }

        ret.push_back(selected_i);
    }
    return ret;
}

//crossover one point
std::pair<genotype_t, genotype_t> crossoverOnePoint(genotype_t a, genotype_t b) {
    std::uniform_int_distribution<int> cross_p_dist(0, a.size() - 1);
    int cross_p = cross_p_dist(random_engine);
    genotype_t a2 = a, b2 = b;
    for (int i = cross_p; i < a.size(); i++) {
        std::swap(a2[i], b2[i]);
    }
    return {a2, b2};
}

//uniform mutation
genotype_t uniformMutation(genotype_t a, double p_m) {
    for (int i = 0; i < a.size(); i++) {
        if (double_random_dist(random_engine) < p_m) {
            a[i] = rand_01(random_engine);
        }
    }
    return a;
}

double oneMaxFitness(genotype_t a) {
    return std::accumulate(a.begin(), a.end(), 0.0);
}

int main() {
    int population_size = 100;
    int gene_size = 100;
    double crossover_probability = 0.9;
    double mutation_probability = 0.01;
    int iterations = 100;

    population_t population;
    // init population
    // random
    for (int i = 0; i < population_size; i++) {
        genotype_t gene;
        for (int j = 0; j < gene_size; j++) {
            gene.push_back(rand_01(random_engine));
        }
        population.push_back(gene);
    }

    // genetic algorithm
    auto result = ga(population, {}, iterations, population_size, mutation_probability, selectionRoulette,
                     crossoverOnePoint, uniformMutation);

    // print best result
    for (auto &gene: result) {
        std::cout << oneMaxFitness(gene) << std::endl;
    }
    return 0;
}