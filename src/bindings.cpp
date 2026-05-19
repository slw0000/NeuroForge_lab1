#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include "../include/neural_network.h"
#include "../include/utils.h"

namespace py = pybind11;

nnlab::Matrix py_array_to_matrix(py::array_t<double> arr) {
    auto buf = arr.request();
    if (buf.ndim != 2) throw std::runtime_error("Ожидается 2D массив (rows, cols)");
    nnlab::Matrix m(buf.shape[0], buf.shape[1]);
    double* ptr = static_cast<double*>(buf.ptr);
    for (size_t i = 0; i < m.rows(); ++i)
        for (size_t j = 0; j < m.cols(); ++j)
            m(i, j) = ptr[i * m.cols() + j];
    return m;
}

std::vector<nnlab::Matrix> py_X_to_matrices(py::array_t<double> X) {
    auto buf = X.request();
    if (buf.ndim != 2) throw std::runtime_error("X должен быть 2D массивом (samples, features)");
    size_t n = buf.shape[0];
    size_t f = buf.shape[1];
    double* ptr = static_cast<double*>(buf.ptr);
    std::vector<nnlab::Matrix> inputs;
    inputs.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        nnlab::Matrix col(f, 1);
        for (size_t j = 0; j < f; ++j) col(j, 0) = ptr[i * f + j];
        inputs.push_back(std::move(col));
    }
    return inputs;
}

py::array_t<double> matrix_to_py_array(const nnlab::Matrix& m) {
    std::vector<py::ssize_t> shape = {static_cast<py::ssize_t>(m.rows()), static_cast<py::ssize_t>(m.cols())};
    py::array_t<double> res(shape);
    auto buf = res.request();
    double* ptr = static_cast<double*>(buf.ptr);
    for (size_t i = 0; i < m.rows(); ++i)
        for (size_t j = 0; j < m.cols(); ++j)
            ptr[i * m.cols() + j] = m(i, j);
    return res;
}

PYBIND11_MODULE(nnlab_py, m) {
    m.doc() = "NeuroForge C++ Neural Network bindings";

    py::class_<NeuralNetwork>(m, "NeuralNetwork")
        .def(py::init([](const std::vector<int>& structure,
                         py::object activations,
                         py::object activations_deriv) {
            std::vector<std::function<double(double)>> act_funcs;
            std::vector<std::function<double(double)>> act_derivs;
            if (!activations.is_none()) act_funcs = activations.cast<std::vector<std::function<double(double)>>>();
            if (!activations_deriv.is_none()) act_derivs = activations_deriv.cast<std::vector<std::function<double(double)>>>();
            return new NeuralNetwork(structure, act_funcs, act_derivs);
        }), py::arg("structure"), py::arg("activations") = py::none(), py::arg("activations_deriv") = py::none())

        .def("train",
             [](NeuralNetwork& self, py::array_t<double> X, py::array_t<int> y,
                py::object loss_func, py::object loss_deriv,
                int max_epochs, double lr, double min_delta, int patience) {

                 auto lf = loss_func.is_none() ? &nnlab::mseLoss : loss_func.cast<std::function<double(double, int)>>();
                 auto ld = loss_deriv.is_none() ? &nnlab::mseDerivative : loss_deriv.cast<std::function<double(double, int)>>();

                 auto x_buf = X.request();
                 auto y_buf = y.request();
                 if (x_buf.ndim != 2) throw std::runtime_error("X must be 2D");
                 if (y_buf.ndim != 1) throw std::runtime_error("y must be 1D");
                 if (x_buf.shape[0] != y_buf.shape[0]) throw std::runtime_error("X and y length mismatch");

                 auto inputs = py_X_to_matrices(X);
                 std::vector<int> labels(static_cast<int*>(y_buf.ptr),
                                         static_cast<int*>(y_buf.ptr) + y_buf.shape[0]);

                 Dataset train_data{std::move(inputs), std::move(labels)};
                 self.train(train_data, lf, ld, max_epochs, lr, min_delta, patience);
             },
             py::arg("X"), py::arg("y"),
             py::arg("loss_func") = py::none(),
             py::arg("loss_deriv") = py::none(),
             py::arg("max_epochs") = 1000, py::arg("lr") = 0.01,
             py::arg("min_delta") = 1e-5, py::arg("patience") = 20)

        .def("predict", [](NeuralNetwork& self, py::array_t<double> X) {
                 return self.predict(py_X_to_matrices(X));
             }, py::arg("X"))

        .def("predict_proba", [](NeuralNetwork& self, py::array_t<double> X) {
                 return self.predictProba(py_X_to_matrices(X));
             }, py::arg("X"))

        .def("get_weights", [](NeuralNetwork& self) {
                 py::list res;
                 for (const auto& w : self.getWeights())
                     res.append(matrix_to_py_array(w));
                 return res;
             })

        .def("set_weights", [](NeuralNetwork& self, py::list weights) {
                 std::vector<nnlab::Matrix> cpp_w;
                 cpp_w.reserve(weights.size());
                 for (const auto& w : weights)
                     cpp_w.push_back(py_array_to_matrix(w.cast<py::array_t<double>>()));
                 self.setWeights(cpp_w);
             }, py::arg("weights"));

    m.def("gen_dataset", [](int count, double noise, double x0, double y0, double x1, double y1) {
        auto data = nnlab::genBinClassifyDataset(count, noise, x0, y0, x1, y1);
        size_t n = data.first.size();

        std::vector<py::ssize_t> x_shape = {static_cast<py::ssize_t>(n), 2};
        std::vector<py::ssize_t> y_shape = {static_cast<py::ssize_t>(n)};

        py::array_t<double> X(x_shape);
        py::array_t<int> y(y_shape);

        auto X_buf = X.request();
        auto y_buf = y.request();
        double* X_ptr = static_cast<double*>(X_buf.ptr);
        int* y_ptr = static_cast<int*>(y_buf.ptr);

        for(size_t i = 0; i < n; ++i) {
            X_ptr[i*2 + 0] = data.first[i](0,0);
            X_ptr[i*2 + 1] = data.first[i](1,0);
            y_ptr[i] = data.second[i];
        }
        return std::make_pair(X, y);
    }, py::arg("count"), py::arg("noise") = 0.5,
       py::arg("x0_center") = -1.0, py::arg("y0_center") = -1.0,
       py::arg("x1_center") = 1.0, py::arg("y1_center") = 1.0);

    m.def("min_max_normalization", [](const py::array_t<double>& X, const py::array_t<int>& y) {
        auto x_buf = X.request();
        auto y_buf = y.request();
        if (x_buf.ndim != 2) throw std::runtime_error("X должен быть 2D массивом");
        if (y_buf.ndim != 1) throw std::runtime_error("y должен быть 1D массивом");
        if (x_buf.shape[0] != y_buf.shape[0]) throw std::runtime_error("X и y должны иметь одинаковую длину");

        std::vector<nnlab::Matrix> cords;
        size_t n = x_buf.shape[0];
        size_t f = x_buf.shape[1];
        cords.reserve(n);
        double* x_ptr = static_cast<double*>(x_buf.ptr);

        for (size_t i = 0; i < n; ++i) {
            nnlab::Matrix m(f, 1);
            for(size_t j=0; j<f; ++j) m(j, 0) = x_ptr[i*f + j];
            cords.push_back(std::move(m));
        }
        std::vector<int> labels(static_cast<int*>(y_buf.ptr), static_cast<int*>(y_buf.ptr) + y_buf.shape[0]);
        Dataset data{std::move(cords), std::move(labels)};

        Dataset norm_data = nnlab::minMaxNormalization(data);

        size_t n_norm = norm_data.first.size();
        size_t f_norm = norm_data.first[0].rows();

        std::vector<py::ssize_t> X_norm_shape = {static_cast<py::ssize_t>(n_norm), static_cast<py::ssize_t>(f_norm)};
        std::vector<py::ssize_t> y_norm_shape = {static_cast<py::ssize_t>(n_norm)};

        py::array_t<double> X_norm(X_norm_shape);
        py::array_t<int> y_norm(y_norm_shape);

        auto X_buf_n = X_norm.request();
        auto y_buf_n = y_norm.request();
        double* X_n_ptr = static_cast<double*>(X_buf_n.ptr);
        int* y_n_ptr = static_cast<int*>(y_buf_n.ptr);

        for(size_t i = 0; i < n_norm; ++i) {
            for(size_t j=0; j<f_norm; ++j)
                X_n_ptr[i*f_norm + j] = norm_data.first[i](j,0);
            y_n_ptr[i] = norm_data.second[i];
        }
        return std::make_pair(X_norm, y_norm);
    }, py::arg("X"), py::arg("y"));

    m.def("mseLoss", &nnlab::mseLoss);
    m.def("mseDerivative", &nnlab::mseDerivative);
    m.def("bceLoss", &nnlab::bceLoss);
    m.def("bceDerivative", &nnlab::bceDerivative);
    m.def("sigmoid", &nnlab::sigmoid);
    m.def("sigmoidDerivative", &nnlab::sigmoidDerivative);
    m.def("relu", &nnlab::relu);
    m.def("reluDerivative", &nnlab::reluDerivative);
    m.def("tanh", &nnlab::tanh);
    m.def("tanhDerivative", &nnlab::tanhDerivative);
}