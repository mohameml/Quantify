use std::ops::Sub;

use ndarray::{Array1, Array2};
use serde_json::Value;

use crate::options::option::Option;

pub struct BasketOption {
    pub strike: f64,
    pub model_size: usize,
    pub payoff_coeffcients: Array1<f64>,
}

impl BasketOption {
    pub fn new(strike: f64) -> Self {
        BasketOption {
            strike: strike,
            model_size: 1,
            payoff_coeffcients: Array1::from(vec![1.0]),
        }
    }
}

impl BasketOption {
    pub fn from_json(json: &Value) -> Self {
        let strike = json["strike"].as_f64().unwrap();

        let model_size = json["option size"].as_i64().unwrap() as usize;

        let payoff_coeff: Array1<f64> = {
            let mut coeff_: Vec<f64> = json["payoff coefficients"]
                .as_array()
                .unwrap()
                .iter()
                .map(|x| x.as_f64().unwrap())
                .collect();

            if coeff_.len() == 1 && model_size > 1 {
                coeff_ = vec![coeff_[0]; model_size];
            }

            Array1::from(coeff_)
        };

        BasketOption {
            strike: strike,
            model_size: model_size,
            payoff_coeffcients: payoff_coeff,
        }
    }
}

impl Option for BasketOption {
    fn payoff(&self, path: &Array2<f64>) -> f64 {
        assert!(path.nrows() > 0, "Path is empty!");

        path.row(path.nrows() - 1)
            .dot(&self.payoff_coeffcients)
            .sub(self.strike)
            .max(0.0)
    }
}
