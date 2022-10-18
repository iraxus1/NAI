#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <chrono>

/**
 * domain - generate domain points. Throws exception when all the points were returned
 */
auto brute_force = [](auto f, auto domain, int how_many) {
    auto current_p = domain();
    auto best_point = current_p;
    try {
        for(int i = 0; i<how_many; i++) {
            while (true) {
                if (f(current_p) < f(best_point)) {
                    best_point = current_p;
                }
                current_p = domain();
            }
        }
    } catch (std::exception &e) {
    }
    return best_point;
};
using domain_t = std::vector<double>;
std::random_device rd;
std::mt19937 mt_generator(rd());
domain_t hill_climbing(const std::function<double(domain_t)> &f, domain_t minimal_d, domain_t maximal_d, int max_iterations) {
    domain_t current_p(minimal_d.size());
    for (int i = 0; i < minimal_d.size(); i++) {
        std::uniform_real_distribution<double> dist(minimal_d[i], maximal_d[i]);
        current_p[i] = dist(mt_generator);
    }
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        domain_t new_p(minimal_d.size());
        for (int i = 0; i < minimal_d.size(); i++) {
            std::uniform_real_distribution<double> dist(-1.0/128.0, 1.0/128.0);
            new_p[i] = current_p[i] + dist(mt_generator);
        }
        if (f(current_p) > f(new_p)) {
            current_p = new_p;
        }
    }
    return current_p;
}

int main() {
    //sphere function
    auto sphere_f = [](double x) {return x*x;};
    double current_sphere_x = -10;
    auto sphere_generator = [&current_sphere_x]() {
        if (current_sphere_x > 10) {
            throw std::exception();
        }
        return current_sphere_x++;
    };
    //sphere function
    //ackley function
    auto ackley_f = [](double x){
        return -20*exp(-0.2*sqrt(0.5*(x*x))) - exp(0.5*cos(2*M_PI*x)) + M_E + 20;
    };
    double current_ackley_x = -10;
    auto ackley_generator = [&current_ackley_x]() {
        if (current_ackley_x > 10) {
            throw std::exception();
        }
        return current_ackley_x++;
    };
    //ackley function
    //matyas function
    auto matyas_f = [](double x){
        return 0.26*(x*x) - 0.48*x;
    };
    double current_matyas_x = -10;
    auto matyas_generator = [&current_matyas_x]() {
        if (current_matyas_x > 10) {
            throw std::exception();
        }
        return current_matyas_x++;
    };
    //matyas function
    //levi function
    auto levi_f = [](double x){
        return sin(3*M_PI*x)*sin(3*M_PI*x) + (x-1)*(x-1)*(1+sin(3*M_PI*x)*sin(3*M_PI*x));
    };
    double current_levi_x = -10;
    auto levi_generator = [&current_levi_x]() {
        if (current_levi_x > 10) {
            throw std::exception();
        }
        return current_levi_x++;
    };
    //levi function

    std::cout << "1.Sphere function\n2.Ackley function\n3.Matyas function\n4.Levi function\n5.Levi test 20" << std::endl;
    int point = 1;
    std::cin >> point;
    switch(point){
        case 1: {
            auto best_point = brute_force(sphere_f, sphere_generator, 1000);
            std::cout << "best x = " << best_point << std::endl;
            auto sphere_f_v = [](domain_t x) { return x[0] * x[0]; };
            auto best2 = hill_climbing(sphere_f_v, {-10}, {10}, 10000);
            std::cout << "best x = " << best2[0] << std::endl;
            break;
        }
        case 2: {
            auto best_point = brute_force(ackley_f, ackley_generator, 1000);
            std::cout << "best x = " << best_point << std::endl;
            auto ackley_f_v = [](domain_t x) {
                return -20 * exp(-0.2 * sqrt(0.5 * (x[0] * x[0]))) - exp(0.5 * cos(2 * M_PI * x[0])) + M_E + 20;
            };
            auto best2 = hill_climbing(ackley_f_v, {-10}, {10}, 10000);
            std::cout << "best x = " << best2[0] << std::endl;
        }
            break;
        case 3: {
            auto best_point = brute_force(matyas_f, matyas_generator, 1000);
            std::cout << "best x = " << best_point << std::endl;
            auto matyas_f_v = [](domain_t x) {
                return 0.26 * (x[0] * x[0]) - 0.48 * x[0];
            };
            auto best2 = hill_climbing(matyas_f_v, {-10}, {10}, 10000);
            std::cout << "best x = " << best2[0] << std::endl;
            break;
        }
        case 4: {
            auto best_point = brute_force(levi_f, levi_generator, 1000000);
            std::cout << "best x = " << best_point << std::endl;
            auto levi_f_v = [](domain_t x) {
                return sin(3 * M_PI * x[0]) * sin(3 * M_PI * x[0]) + (x[0] - 1) * (x[0] - 1) * (1 + sin(3 * M_PI * x[0]) * sin(3 * M_PI * x[0]));
            };
            auto best2 = hill_climbing(levi_f_v, {-10}, {10}, 1000000);
            std::cout << "best x = " << best2[0] << std::endl;
            break;
        }
        case 5:{
            auto best_point = brute_force(levi_f, levi_generator, 1000000);
            std::cout << "best x1 = " << best_point << std::endl;
            auto levi_f_v = [](domain_t x) {
                return sin(3 * M_PI * x[0]) * sin(3 * M_PI * x[0]) + (x[0] - 1) * (x[0] - 1) * (1 + sin(3 * M_PI * x[0]) * sin(3 * M_PI * x[0]));
            };
            for (int i = 0; i < 20; ++i) {
                std::cout << "Pomiar "<< i+1 << " ilosc iteracji: " << 1000000*(i+1) << std::endl;
                auto begin = std::chrono::high_resolution_clock::now();
                auto best2 = hill_climbing(levi_f_v, {-10}, {10}, 1000000*(i+1));
                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
                std::cout << "czas: " << elapsed.count() << "s" << std::endl;
                std::cout << "best x2 = " << best2[0] << std::endl;
            }
            break;
        }
        default:
            std::cout << "Wrong operation" << std::endl;
            break;
    }
    return 0;
}