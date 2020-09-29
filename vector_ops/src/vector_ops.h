#pragma once
#include <vector>
#include <iostream>


namespace task {

    std::vector<double> operator+(const std::vector<double>& vec1, const std::vector<double>& vec2) {
        std::vector<double> vec3(vec1.size());
        for (size_t i = 0; i < vec1.size(); ++i) {
            vec3[i] = vec1[i] + vec2[i];
        }
        return vec3;
    }

    std::vector<double> operator-(const std::vector<double>& vec1, const std::vector<double>& vec2) {
        std::vector<double> vec3(vec1.size());
        for (size_t i = 0; i < vec1.size(); ++i) {
            vec3[i] = vec1[i] - vec2[i];
        }
        return vec3;
    }

    std::vector<double> operator+(const std::vector<double>& vec) {
        return vec;
    }

    std::vector<double> operator-(const std::vector<double>& vec) {
        std::vector<double> vec1(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) {
            vec1[i] = -vec[i];
        }
        return vec1;
    }

    double operator*(const std::vector<double>& vec1, const std::vector<double>& vec2) {
        double dp = 0.0;
        for (size_t i = 0; i < vec1.size(); ++i) {
            dp += vec1[i] * vec2[i];
        }
        return dp;  // dot product
    }

    std::vector<double> operator%(const std::vector<double>& vec1, const std::vector<double>& vec2) {
        std::vector<double> vec3 = {
            vec1[1] * vec2[2] - vec1[2] * vec2[1],
            vec1[2] * vec2[0] - vec1[0] * vec2[2],
            vec1[0] * vec2[1] - vec1[1] * vec2[0]
        };
        return vec3;  // 3d cross product
    }

    bool operator||(const std::vector<double>& vec1, const std::vector<double>& vec2) {
        for (size_t i = 1; i < vec1.size(); ++i) {
            double delta = vec2[i] / vec1[i] - vec2[i - 1] / vec1[i - 1];
            if (abs(delta) > 1e-9) {  // epsilon = 1e-9
                return false;
            }
        }
        return true;  // collinearity check
    }

    bool operator&&(const std::vector<double>& vec1, const std::vector<double>& vec2) {
        for (size_t i = 0; i < vec1.size(); ++i) {
            if ((vec1[i] > 0) != (vec2[i] > 0)) {
                return false;
            }
        }
        return vec1 || vec2;  // codirectionality check
    }

    std::istream& operator>>(std::istream& istream, std::vector<double>& vec) {
        size_t vec_size;
        istream >> vec_size;
        vec.resize(vec_size);
        for (size_t i = 0; i < vec_size; ++i) {
            istream >> vec[i];
        }
        return istream;
    }

    std::ostream& operator<<(std::ostream& ostream, const std::vector<double>& vec) {
        for (size_t i = 0; i < vec.size(); ++i) {
            ostream << vec[i] << " ";
        }
        ostream << std::endl;
        return ostream;
    }

    void reverse(std::vector<double>& vec) {
        for (size_t i = 0; i < vec.size() / 2; ++i) {
            size_t j = vec.size() - 1 - i;
            std::swap(vec[i], vec[j]);
        }
    }

    std::vector<int> operator|(const std::vector<int>& vec1, const std::vector<int>& vec2) {
        std::vector<int> vec3(vec1.size());
        for (size_t i = 0; i < vec3.size(); ++i) {
            vec3[i] = vec1[i] | vec2[i];
        }
        return vec3;
    }

    std::vector<int> operator&(const std::vector<int>& vec1, const std::vector<int>& vec2) {
        std::vector<int> vec3(vec1.size());
        for (size_t i = 0; i < vec3.size(); ++i) {
            vec3[i] = vec1[i] & vec2[i];
        }
        return vec3;
    }

}  // namespace task
