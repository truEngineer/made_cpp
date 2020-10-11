#include "matrix.h"

using namespace task;


Matrix::Matrix() {
    m_rows = 1;
    m_cols = 1;
    m_data = new double*[1];
    m_data[0] = new double[1];
    m_data[0][0] = 1;
};

Matrix::Matrix(size_t rows, size_t cols) {
    m_rows = rows;
    m_cols = cols;
    m_data = new double*[rows];

    for (size_t i = 0; i < rows; ++i)
        m_data[i] = new double[cols];

    for (size_t i = 0; i < std::min(cols, rows); ++i)
        m_data[i][i] = 1.0;
};

Matrix::Matrix(const Matrix& copy) {
    m_rows = copy.m_rows;
    m_cols = copy.m_cols;
    m_data = new double*[m_rows];

    for (size_t i = 0; i < m_rows; ++i)
        m_data[i] = new double[m_cols];

    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            m_data[i][j] = copy[i][j];
}

Matrix& Matrix::operator=(const Matrix& a) {
    if (&a == this)
        return *this;

    for (size_t i = 0; i < m_rows; ++i)
        delete[] m_data[i];
    delete[] m_data;

    m_rows = a.m_rows;
    m_cols = a.m_cols;

    m_data = new double*[m_rows];
    for (size_t i = 0; i < m_rows; ++i)
        m_data[i] = new double[m_cols];
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            m_data[i][j] = a[i][j];

    return *this;
}

Matrix::~Matrix() {
    for (size_t i = 0; i < m_rows; ++i)
        delete[] m_data[i];
    delete[] m_data;
};

double& Matrix::get(size_t row, size_t col) {
    if (row >= m_rows or col >= m_cols)
        throw OutOfBoundsException();

    return m_data[row][col];
}

const double& Matrix::get(size_t row, size_t col) const {
    if (row >= m_rows or col >= m_cols)
        throw OutOfBoundsException();

    return m_data[row][col];
}

void Matrix::set(size_t row, size_t col, const double& value) {
    if (row >= m_rows or col >= m_cols)
        throw OutOfBoundsException();

    m_data[row][col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
    auto new_data = new double*[new_rows];

    for (size_t i = 0; i < new_rows; ++i)
        new_data[i] = new double[new_cols];

    for (size_t i = 0; i < std::min(m_rows, new_rows); ++i)
        for (size_t j = 0; j < std::min(m_cols, new_cols); ++j)
            new_data[i][j] = m_data[i][j];

    for (size_t i = 0; i < m_rows; ++i)
        delete[] m_data[i];
    delete[] m_data;

    m_data = new_data;
    m_rows = new_rows;
    m_cols = new_cols;
}

double* Matrix::operator[](size_t row) {
    if (row >= m_rows)
        throw SizeMismatchException();

    return m_data[row];
}

const double* Matrix::operator[](size_t row) const {
    if (row >= m_rows)
        throw SizeMismatchException();

    return m_data[row];
}

Matrix& Matrix::operator+=(const Matrix& a) {
    if (a.m_rows != m_rows or a.m_cols != m_cols)
        throw SizeMismatchException();

    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            m_data[i][j] += a[i][j];

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
    if (a.m_rows != m_rows or a.m_cols != m_cols)
        throw SizeMismatchException();

    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            m_data[i][j] -= a[i][j];

    return *this;
}

Matrix& Matrix::operator*=(const double& number) {
    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            m_data[i][j] *= number;

    return *this;
}

Matrix Matrix::operator+(const Matrix& a) const {
    if (a.m_rows != m_rows or a.m_cols != m_cols)
        throw SizeMismatchException();

    Matrix result = *this;
    result += a;

    return result;
}

Matrix Matrix::operator-(const Matrix& a) const {
    if (a.m_rows != m_rows or a.m_cols != m_cols)
        throw SizeMismatchException();

    Matrix result = *this;
    result -= a;

    return result;
}

Matrix Matrix::operator*(const Matrix& a) const {
    if (m_cols != a.m_rows)
        throw SizeMismatchException();

    Matrix result(m_rows, a.m_cols);

    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < a.m_cols; ++j) {
            result[i][j] = 0.0;
            for (size_t k = 0; k < m_cols; ++k) {
                result[i][j] += m_data[i][k] * a[k][j];
            }
        }
    }

    return result;
}

Matrix& Matrix::operator*=(const Matrix& a) {
    if (m_cols != a.m_rows)
        throw SizeMismatchException();

    *this = *this * a;

    return *this;
}

Matrix Matrix::operator*(const double& a) const {
    Matrix result(m_rows, m_cols);

    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            result[i][j] = m_data[i][j] * a;

    return result;
}

Matrix Matrix::operator-() const {
    Matrix result(m_rows, m_cols);

    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            result[i][j] = -m_data[i][j];

    return result;
}

Matrix Matrix::operator+() const {
    return *this;
}

double Matrix::det() const{
    if (m_rows != m_cols)
        throw SizeMismatchException();

    Matrix mcopy(*this);
    size_t n = mcopy.m_rows;
    double det = 1.0;

    // Row operations for i = 0, ... , n - 2 (n - 1 not needed)
    for (size_t i = 0; i < n - 1; i++) {
        // Partial pivot: find row r below with largest element in column i
        size_t r = i;
        double max_val = abs(mcopy[i][i]);
        for (size_t k = i + 1; k < n; k++) {
            double val = abs(mcopy[k][i]);
            if (val > max_val) {
                r = k;
                max_val = val;
            }
        }
        if (r != i) {
            for (size_t j = i; j < n; j++)
                std::swap(mcopy[i][j], mcopy[r][j]);
            det = -det;
        }

        // Row operations to make upper-triangular
        double pivot = mcopy[i][i];
        if (abs(pivot) < EPS)
            return 0.0; // Singular matrix

        for (size_t l = i + 1; l < n; l++) {  // On lower rows
            double multiple = mcopy[l][i] / pivot;  // Multiple of row i to clear element in ith column
            for (size_t j = i; j < n; j++)
                mcopy[l][j] -= multiple * mcopy[i][j];
        }
        det *= pivot;  // Determinant is product of diagonal
    }

    det *= mcopy[n - 1][n - 1];

    return det;
}

Matrix Matrix::transposed() const {
    Matrix result(m_cols, m_rows);

    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            result[j][i] = m_data[i][j];

    return result;
}

void Matrix::transpose() {
    Matrix mcopy(*this);
    *this = mcopy.transposed();;
}

double Matrix::trace() const {
    if (m_rows != m_cols)
        throw SizeMismatchException();

    double trace = 0.0;
    for (size_t i = 0; i < m_rows; ++i)
        trace += m_data[i][i];

    return trace;
}

std::vector<double> Matrix::getRow(size_t row) {
    if (row >= m_rows)
        throw OutOfBoundsException();

    std::vector<double> result(m_cols);
    for (size_t i = 0; i < m_cols; ++i)
        result[i] = m_data[row][i];

    return result;
}

std::vector<double> Matrix::getColumn(size_t column) {
    if (column >= m_cols)
        throw OutOfBoundsException();

    std::vector<double> result(m_rows);
    for (size_t i = 0; i < m_rows; ++i)
        result[i] = m_data[i][column];

    return result;
}

std::pair<size_t, size_t> Matrix::getSize() const {
    std::pair<size_t, size_t> rows_cols = {m_rows, m_cols};

    return rows_cols;
}

bool Matrix::operator==(const Matrix& a) const {
    if (a.m_rows != m_rows or a.m_cols != m_cols)
        throw SizeMismatchException();

    for (size_t i = 0; i < m_rows; ++i)
        for (size_t j = 0; j < m_cols; ++j)
            if (abs(m_data[i][j] - a[i][j]) >= EPS)
                return false;

    return true;
}

bool Matrix::operator!=(const Matrix& a) const {
    return !(*this == a);
}

Matrix task::operator*(const double& a, const Matrix& b) {
    Matrix result(b);
    result *= a;

    return result;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix) {
    auto size = matrix.getSize();
    size_t rows = size.first;
    size_t cols = size.second;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j)
            output << matrix[i][j] << " ";
        output << "\n";
    }

    return output;
}

std::istream& task::operator>>(std::istream& input, Matrix& matrix) {
    size_t rows, cols;
    input >> rows >> cols;

    matrix = Matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            input >> matrix[i][j];

    return input;
}