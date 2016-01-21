//
// Created by Makram Kamaleddine on 1/21/16.
//

#ifndef QR_DECOMPOSITION_NVECTOR_HPP
#define QR_DECOMPOSITION_NVECTOR_HPP

#include "matrix.hpp"

// forward declaration
template<typename, typename = void> struct nvector;

// partial specialization of matrix in order to add some familiar operator[] syntax
// rather than using operator() for both matrices and vectors
template<typename T>
struct nvector<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {

    nvector(unsigned N)
            : vec(N, 1)
    {

    }

    nvector(unsigned N, T filler)
            : vec(N, 1, filler)
    {

    }

    nvector(const nvector& other)
            : vec(other.vec)
    {

    }

    nvector(unsigned N, std::initializer_list<T> lst)
            : vec(N, 1, lst)
    {

    }

    T& operator[](unsigned int i)
    {
        return vec(i, 0);
    }

    const T& operator[](unsigned int i) const
    {
        return vec(i, 0);
    }

    unsigned int size() const
    {
        return vec.rowCount();
    }

    typename std::vector<T>::const_iterator begin() const
    {
        return vec.data().cbegin();
    }

    typename std::vector<T>::const_iterator end() const
    {
        return vec.data().cend();
    }

    T inf_norm() const
    {
        std::vector<T> data(vec.data());
        std::for_each(data.begin(), data.end(), [](T value) -> T {
            return std::abs(value);
        });
        return *(std::max_element(data.begin(), data.end()));
    }
private:
    matrix<T> vec;
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const nvector<T>& nvec)
{
    stream << "[";
    for (auto i = 0; i < nvec.size(); ++i)
    {
        if (i != nvec.size() - 1) {
            stream << nvec[i] << "; ";
        } else {
            stream << nvec[i];
        }
    }
    stream << "]";
    return stream;
}

// overload operator* for matrix-vector multiplication
template<typename T>
nvector<T> operator*(const matrix<T>& M, const nvector<T>& v)
{
    // dimension check
    if (M.colCount() != v.size()) {
        throw std::invalid_argument("Matrix and vector dimensions are not compatible");
    }

    nvector<T> result(v.size());
    for (auto i = 0; i < M.rowCount(); ++i)
    {
        T sum = 0;
        // do an inner product
        for (auto j = 0; j < M.colCount(); ++j)
        {
            sum += M(i, j) * v[j];
        }
        result[i] = sum;
    }

    return result;
}

// scalar-vector multiplication
template<typename T>
nvector<T> operator*(const T& scalar, const nvector<T>& v)
{
    nvector<T> result(v.size());
    for (auto i = 0; i < result.size(); ++i)
    {
        result[i] = v[i] * scalar;
    }

    return result;
}

template<typename T>
nvector<T> operator-(const nvector<T>& v1, const nvector<T>& v2)
{
    // dim check
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vector dimensions are not equal");
    }

    nvector<T> result(v1.size());
    for (auto i = 0; i < v1.size(); ++i)
    {
        result[i] = v1[i] - v2[i];
    }

    return result;
}
#endif //QR_DECOMPOSITION_NVECTOR_HPP
