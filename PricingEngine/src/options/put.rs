use crate::options::option::Option;

pub struct PutOption {
    strike: f64,
}

impl PutOption {
    pub fn new(strike: f64) -> Self {
        PutOption { strike: strike }
    }
}

impl Option for PutOption {
    fn payoff(&self, path: &ndarray::Array2<f64>) -> f64 {
        let s_t = path[[path.nrows() - 1, 0]];

        (self.strike - s_t).max(0.0)
    }
}
