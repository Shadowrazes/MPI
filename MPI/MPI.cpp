#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<iomanip>

using namespace std;

const double E = 0.0001;

void printMatrix(vector<vector<double>>& matrix) {
    cout.precision(6);
    for (auto& stroke : matrix) {
        for (auto& column : stroke) {
            cout << fixed << column << " ";
        }
        cout << endl << endl;
    }
}

void printVector(vector<double> vect) {
    for (const auto& it : vect)
        cout << it << " ";
    cout << endl << endl;
}

void strokeWrap(string& stroke, vector <double>& matrixStroke, vector <double>& var) {
    string buff = "";
    for (auto& it : stroke) {
        if (it == ' ') {
            if (buff.size() != 0) {
                matrixStroke.push_back(stod(buff));
                buff = "";
            }
            continue;
        }
        else
            buff += it;
    }
    var.push_back(stod(buff));
}

vector<double> matrixMultiplyZeidel(vector<vector<double>>& matrix1, vector<double> matrix2, vector <double>& var) {
    vector<double> multiply(matrix2.size(), 0);

    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix1.size(); j++) {
            multiply[i] += matrix1[i][j] * matrix2[j];
        }
        matrix2[i] = var[i] - multiply[i];
    }
    return matrix2;
}

vector<double> matrixMultiply(vector<vector<double>>& matrix1, vector<double>& matrix2) {
    vector<double> multiply(matrix2.size(), 0);

    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix1.size(); j++) {
            multiply[i] += matrix1[i][j] * matrix2[j];
        }
    }
    return multiply;
}

vector<double> subtraction(vector<double> matrix1, vector<double> matrix2) {
    for (int i = 0; i < matrix1.size(); i++) {
        matrix1[i] -= matrix2[i];
    }
    return matrix1;
}

bool checkE(vector<double>& x, vector<double>& xPrev) {
    for (int i = 0; i < x.size(); i++) {
        if (abs(x[i] - xPrev[i]) > E) {
            return true;
        }
    }
    return false;
}

void dissmilate(vector<vector<double>>& matrix, vector <double>& var) {
    for (int i = 0; i < matrix.size(); i++) {
        double diagonal = matrix[i][i];
        matrix[i][i] -= diagonal;
        for (int j = 0; j < matrix[i].size(); j++) {
            matrix[i][j] /= diagonal;
        }
        var[i] /= diagonal;
    }
}

int main()
{
    fstream matrixData("Matrix.txt");
    vector<vector<double>> matrix;
    vector <double> var;

    while (!matrixData.eof()) {
        vector<double> tmp;
        matrix.push_back(tmp);
        string buff;
        getline(matrixData, buff);
        strokeWrap(buff, matrix[matrix.size() - 1], var);
    }

    cout << "Original Matrix:" << endl;
    printMatrix(matrix);
    cout << "Equals:" << endl;
    printVector(var);

    dissmilate(matrix, var);

    cout << "After dissmilate:" << endl;
    cout << "Matrix:" << endl;
    printMatrix(matrix);
    cout << "Equals:" << endl;
    printVector(var);

    vector<vector<double>> valuesX;
    valuesX.push_back(vector<double>(matrix[0].size(), 0));
    valuesX.push_back(var);

    int i = 1;
    while (checkE(valuesX[i], valuesX[i - 1])) {
        valuesX.push_back(subtraction(var, matrixMultiply(matrix, valuesX[i])));
        i++;
    }

    for (i = 0; i < valuesX[0].size(); i++)
        cout << setw(8) << "x" + to_string(i);
    cout << endl << endl;
    for (i = 0; i < valuesX.size(); i++) {
        cout << i + 1 << ") ";
        printVector(valuesX[i]);
    }

    cout << endl << endl << "Zeidel:" << endl << endl;

    valuesX.clear();
    valuesX.push_back(vector<double>(matrix[0].size(), 0));
    
    valuesX.push_back(matrixMultiplyZeidel(matrix, valuesX[0], var));

    i = 1;
    while (checkE(valuesX[i], valuesX[i - 1])) {
        valuesX.push_back(matrixMultiplyZeidel(matrix, valuesX[i], var));
        i++;
    }

    for (i = 0; i < valuesX[0].size(); i++)
        cout << setw(8) << "x" + to_string(i);
    cout << endl << endl;
    for (i = 0; i < valuesX.size(); i++) {
        cout << i + 1 << ") ";
        printVector(valuesX[i]);
    }
}