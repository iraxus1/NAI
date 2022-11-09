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
        std::cout << iteration << " " << current_point << " " << f(current_point)
                  << std::endl;
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
        std::cout << iteration << " " << current_point << " " << f(current_point)
                  << std::endl;
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
        std::cout << iteration << " " << current_point << " " << f(current_point)
                  << std::endl;
        std::vector<domain_t> close_points = get_close_points(current_point);
        current_point =
                *std::min_element(close_points.begin(), close_points.end(),
                                  [f](auto a, auto b) { return f(a) < f(b); });
        if (f(best_point) > f(current_point)) best_point = current_point;
    }
    return best_point;
}
domain_t brute_force_method(const std::function<double(domain_t)> &f, const std::function<std::optional<domain_t>()> &domain_generator) {

    auto best_p = domain_generator();
    for (auto current_p = best_p; current_p.has_value(); current_p = domain_generator()) {
        std::cout << current_p.value() << " " << f(current_p.value()) << std::endl;
        if (f(current_p.value()) < f(best_p.value())) {
            best_p = current_p;
        }
    }
    return best_p.value_or(domain_t());
}
int main() {
    const double precision = 1.0 / 16;
    auto rastrigin_f_v = [](domain_t x) {
        const double A = 10.0;
        double ret = A * x.size();
        for (int i = 0; i < x.size(); i++) {
            ret = ret + x[i] * x[i] - A * std::cos(2 * M_PI * x[i]);
        }
        return ret;
    };
    auto rastrigin_f_generator = [precision]() {
        std::uniform_real_distribution<> dis(-5.12, 5.12);
        domain_t ret;
        for (int i = 0; i < 3; i++) {
            ret.push_back(dis(mt_generator));
        }
        return ret;
    };
    auto sphere_f_v = [](domain_t x) { return x[0] * x[0] + x[1] * x[1]; };
    auto sphere_f_generator = [precision]() -> std::optional<domain_t> {
        static domain_t p = {-10, -10};
        int i = 0;
        for (i; i < p.size(); i++) {
            p[i] = p[i] + precision;
            if (p[i] < 10) return std::optional(p);
            p[i] = -10;
        }
        return {};
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
//    auto best0 =
//            tabu_method(rastrigin_f_v, get_random_point(), get_close_points, 1000);
//    std::cout << "# tabu x = " << best0[0] << " " << best0[1] << std::endl;
//    auto best1 = annealing3D(rastrigin_f_v, get_random_point(), get_close_points, 30000, 1000);
//    std::cout << "# simulatedAnnealing x = " << best1[0] << " " << best1[1] << std::endl;
//    auto best2 = hill_climbing(sphere_f_v, get_random_point(), get_close_points, 1000);
//    std::cout << "# hillClimbing x = " << best2[0] << " " << best2[1] << std::endl;
    auto best3 = brute_force_method(rastrigin_f_v, rastrigin_f_generator);
    std::cout << "# bruteForce x = " << best3[0] << " " << best3[1] << std::endl;
    return 0;
}
