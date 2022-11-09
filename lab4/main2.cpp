#include <iostream>
#include <vector>
#include <functional>
#include <random>
std::random_device rd;
std::mt19937 mt_generator(rd());
auto genetic_algorithm = [](
        auto initial_population, auto fitness, auto term_condition,
        auto selection, double p_crossover,
        auto crossover, double p_mutation,  auto mutation) {
    using namespace std;
    uniform_real_distribution<double> uniform(0.0,1.0);
    auto population = initial_population;
    vector<double> population_fit = fitness(population);
    while (!term_condition(population,population_fit)) {
        auto parents_indexes = selection(population_fit);
        decltype(population) new_population;
        for (int i = 0 ; i < parents_indexes.size(); i+=2) {
            decltype(initial_population) offspring = {population[i],population[i+1]};
            if (uniform(mt_generator) < p_crossover) {
                offspring = crossover(offspring);
            }
            for (const auto& chromosome : offspring) new_population.push_back(chromosome);
        }
        for (auto & chromosome : new_population) {
            chromosome = mutation(chromosome,p_mutation);
        }
        population = new_population;
        population_fit = fitness(population);
    }
    return population;
};
using chromosome_t = std::vector<int>;
using population_t = std::vector<chromosome_t>;
std::vector<double> fitness_function(const population_t& pop){
    std::vector<double> fitness;
    for (const auto& chromosome : pop) {
        double sum = 0;
        for (auto gene : chromosome) sum += gene;
        fitness.push_back(sum);
    }
    return fitness;
}
std::vector<int> selection(const std::vector<double>& fitnesses) {
    std::vector<int> parents;
    std::uniform_int_distribution<int> uniform(0,fitnesses.size()-1);
    for (int i = 0; i < fitnesses.size(); i++) {
        parents.push_back(uniform(mt_generator));
    }
    return parents;
}
std::vector<chromosome_t > crossover(std::vector<chromosome_t > parents) {
    std::vector<chromosome_t > offspring;
    std::uniform_int_distribution<int> uniform(0,parents[0].size()-1);
    int crossover_point = uniform(mt_generator);
    chromosome_t child1,child2;
    for (int i = 0; i < crossover_point; i++) {
        child1.push_back(parents[0][i]);
        child2.push_back(parents[1][i]);
    }
    for (int i = crossover_point; i < parents[0].size(); i++) {
        child1.push_back(parents[1][i]);
        child2.push_back(parents[0][i]);
    }
    offspring.push_back(child1);
    offspring.push_back(child2);
    return offspring;
}
chromosome_t mutation(chromosome_t parents, double p_mutation) {
    std::uniform_real_distribution<double> uniform(0.0,1.0);
    for (int & parent : parents) {
        if (uniform(mt_generator) < p_mutation) {
            parent = !parent;
        }
    }
    return parents;
}

auto decodeGreyCode = [](const chromosome_t& chromosome) {
    chromosome_t decoded;
    int sum = 0;
    for (int i = 0; i < chromosome.size(); i++) {
        sum += chromosome[i] * std::pow(2,chromosome.size()-i-1);
    }
    decoded.push_back(sum);
    return decoded;
};


int main() {
    using namespace std;
    //ackley
    auto term_condition = [](const population_t& population, const vector<double>& fitnesses) {
        for (const auto& fitness : fitnesses) {
            if (fitness < 0.0001) return true;
        }
        return false;
    };
    //genotype
    vector<int> genotype;
    //genotype length (100+(22642 mod 10)*2 ).
    int genotype_length = 100 + (22642 % 10) * 2;
    for (int i = 0; i < genotype_length; i++) genotype.push_back(0);
    //genotype into fenotype
    auto fenotype = [](const chromosome_t& chromosome) {
        return decodeGreyCode(chromosome);
    };
    //initial population
    population_t initial_population;
    for (int i = 0; i < 100; i++) {
        initial_population.push_back(genotype);
    }
    //genetic algorithm
    auto population = genetic_algorithm(
            initial_population,fitness_function,term_condition,
            selection,0.8,crossover,0.01,mutation);
    //print results
    for (const auto& chromosome : population) {
        cout << fenotype(chromosome)[0] << " " << fenotype(chromosome)[1] << endl;
    }

    return 0;

}