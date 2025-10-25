use ndarray::Array2;

pub trait Option {
    fn payoff(&self, path: &Array2<f64>) -> f64;
}
