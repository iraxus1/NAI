#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <random>
#include <vector>

std::mt19937 mt_generator((std::random_device())());

using domain_t = std::vector<double>;
std::ostream &operator<<(std::ostream &o, domain_t &d) {
    o << d[0] << " " << d[1];
    return o;
}
domain_t tabu_method(
        const std::function<double(domain_t)> &f, domain_t start_point,
        std::function<std::vector<domain_t>(domain_t)> get_close_points,
        int max_iterations) {
    domain_t best_point = start_point;
    domain_t current_point = start_point;
    std::vector<domain_t> tabu_list = {current_point};
    for (int iteration = 0; iteration < max_iterations; iteration++) {
//        std::cout << iteration << " " << current_point << " " << f(current_point)
//                  << std::endl;
        std::vector<domain_t> close_points;
        int tabu_idx = tabu_list.size()-1;
        do {
            auto close_points_all = get_close_points(tabu_list.at(tabu_idx));
            for (auto p : close_points_all) {
                bool is_in_tabu = false;
                for (auto p_tabu : tabu_list) {
                    if (p == p_tabu) {
                        is_in_tabu = true;
                        break;
                    }
                }
                if (!is_in_tabu) close_points.push_back(p);
            }
            tabu_idx--;
        } while ((tabu_idx >= 0) && (close_points.size() == 0));
        current_point =
                *std::min_element(close_points.begin(), close_points.end(),
                                  [f](auto a, auto b) { return f(a) < f(b); });
        tabu_list.push_back(current_point);
        if (f(best_point) > f(current_point)) best_point = current_point;
    }
    return best_point;
}

//annealing function but without stop condition (for testing) and for 3D
domain_t annealing3D(const std::function<double(domain_t)> &f, const domain_t& start_point,
                     const std::function<std::vector<domain_t>(domain_t)>& get_close_points,
                     int max_iterations, double temperature) {
    domain_t best_point = start_point;
    domain_t current_point = start_point;
    std::vector<domain_t> annealing_list = {current_point};
    for (int iteration = 0; iteration < max_iterations; iteration++) {
//        std::cout << iteration << " " << current_point << " " << f(current_point)
//                  << std::endl;
        int annealing_idx = annealing_list.size()-1;
        std::vector<domain_t> close_points = get_close_points(current_point);
        std::uniform_int_distribution<> dis(0, close_points.size() - 1);

        int idx = dis(mt_generator);
        domain_t new_point = close_points.at(idx);
        do {
            if (f(new_point) < f(current_point)) {
                current_point = new_point;
            } else {
                double p = std::exp((f(current_point) - f(new_point)) / temperature);
                std::uniform_real_distribution<> dis(0, 1);
                double r = dis(mt_generator);
                if (r < p) current_point = new_point;
            }
            annealing_list.push_back(current_point);
            if (f(best_point) > f(current_point)) best_point = current_point;
            temperature *= 0.99;
            annealing_idx--;
        } while ((annealing_idx >= 0) && (close_points.size() == 0));

        *std::min_element(close_points.begin(), close_points.end(),
                          [f](auto a, auto b) { return f(a) < f(b); });
        annealing_list.push_back(current_point);
        if (f(best_point) > f(current_point)) best_point = current_point;

    }
    return best_point;
}

domain_t hill_climbing(const std::function<double(domain_t)> &f,
                       domain_t start_point,
                       std::function<std::vector<domain_t>(domain_t)> get_close_points,
                       int max_iterations) {
    domain_t best_point = start_point;
    domain_t current_point = start_point;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
//        std::cout << iteration << " " << current_point << " " << f(current_point)
//                  << std::endl;
        std::vector<domain_t> close_points = get_close_points(current_point);
        current_point =
                *std::min_element(close_points.begin(), close_points.end(),
                                  [f](auto a, auto b) { return f(a) < f(b); });
        if (f(best_point) > f(current_point)) best_point = current_point;
    }
    return best_point;
}
auto brute_force(const std::function<double(domain_t)> &f, domain_t start_point,
                 std::function<std::vector<domain_t>(domain_t)> get_close_points,
                 int max_iterations) {
    domain_t best_point = start_point;
    domain_t current_point = start_point;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
//        std::cout << iteration << " " << current_point << " " << f(current_point)
//                  << std::endl;
        std::vector<domain_t> close_points = get_close_points(current_point);
        for (auto p: close_points) {
            if (f(best_point) > f(p)) best_point = p;
        }
    }
    return best_point;
}

int main() {
    auto beale_f_v = [](auto x) {
        return std::pow(1.5 - x[0] + x[0] * x[1], 2) +
               std::pow(2.25 - x[0] + x[0] * x[1] * x[1], 2) +
               std::pow(2.625 - x[0] + x[0] * x[1] * x[1] * x[1], 2);
    };
    auto sphere_f_v = [](domain_t x) { return x[0] * x[0] + x[1] * x[1]; };
    auto ackley_f_v = [](domain_t x) {
        return -20 * std::exp(-0.2 * std::sqrt(0.5 * (x[0] * x[0] + x[1] * x[1]))) -
               std::exp(0.5 * (std::cos(2 * M_PI * x[0]) + std::cos(2 * M_PI * x[1]))) +
               std::exp(1) + 20;
    };

    auto get_random_point = []() -> domain_t {
        std::uniform_real_distribution<double> distr(-10, 10);
        return {distr(mt_generator), distr(mt_generator)};
    };
    auto get_close_points_random = [](domain_t p0) -> std::vector<domain_t> {
        std::uniform_real_distribution<double> distr(-10, 10);
        return {{distr(mt_generator), distr(mt_generator)}};
    };
    auto get_close_points = [](domain_t p0) -> std::vector<domain_t> {
        std::vector<domain_t> ret;
        for (int i = 0; i < p0.size(); i++) {
            domain_t v = p0;
            v[i] += 1.0 / 128.0;
            ret.push_back(v);
            v = p0;
            v[i] -= 1.0 / 128.0;
            ret.push_back(v);
        }
        return ret;
    };
    //beale
    std::cout << "beale" << std::endl;
    auto best0 = tabu_method(beale_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# tabu x = " << best0[0] << " " << best0[1] << std::endl;
    auto best1 = annealing3D(beale_f_v, get_random_point(), get_close_points, 30000, 1000);
    std::cout << "# simulatedAnnealing x = " << best1[0] << " " << best1[1] << std::endl;
    auto best2 = hill_climbing(beale_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# hillClimbing x = " << best2[0] << " " << best2[1] << std::endl;
    auto best3 = brute_force(beale_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# bruteForce = " << best3 << std::endl;
    //sphere
    std::cout << "sphere" << std::endl;
    auto best4 = tabu_method(sphere_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# tabu x = " << best4[0] << " " << best4[1] << std::endl;
    auto best5 = annealing3D(sphere_f_v, get_random_point(), get_close_points, 30000, 1000);
    std::cout << "# simulatedAnnealing x = " << best5[0] << " " << best5[1] << std::endl;
    auto best6 = hill_climbing(sphere_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# hillClimbing x = " << best6[0] << " " << best6[1] << std::endl;
    auto best7 = brute_force(sphere_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# bruteForce = " << best7 << std::endl;
    std::cout << "ackley" << std::endl;
    auto best8 = tabu_method(ackley_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# tabu x = " << best8[0] << " " << best8[1] << std::endl;
    auto best9 = annealing3D(ackley_f_v, get_random_point(), get_close_points, 30000, 1000);
    std::cout << "# simulatedAnnealing x = " << best9[0] << " " << best9[1] << std::endl;
    auto best10 = hill_climbing(ackley_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# hillClimbing x = " << best10[0] << " " << best10[1] << std::endl;
    auto best11 = brute_force(ackley_f_v, get_random_point(), get_close_points, 1000);
    std::cout << "# bruteForce = " << best11 << std::endl;
    return 0;
}