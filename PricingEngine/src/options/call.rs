use crate::options::option::Option;

pub struct CallOption {
    pub strike: f64,
}

impl CallOption {
    pub fn new(strike: f64) -> Self {
        CallOption { strike: strike }
    }
}

impl Option for CallOption {
    fn payoff(&self, path: &ndarray::Array2<f64>) -> f64 {
        let s_t = path[[path.nrows() - 1, 0]];

        (s_t - self.strike).max(0.0)
    }
}
