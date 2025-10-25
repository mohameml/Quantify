use ndarray::{Array1, Array2, Axis};
use serde_json::Value;
use std::ops::Sub;

use crate::options::option::Option;

pub struct AsianOption {
    pub strike: f64,
    pub model_size: usize,
    pub payoff_coeffcients: Array1<f64>,
}

impl AsianOption {
    pub fn new(strike: f64) -> Self {
        AsianOption {
            strike: strike,
            model_size: 1,
            payoff_coeffcients: Array1::from(vec![1.0]),
        }
    }
}

impl AsianOption {
    pub fn from_json(json: &Value) -> Self {
        let strike = json["strike"].as_f64().unwrap();

        let model_size = json["option size"].as_i64().unwrap() as usize;

        let payoff_coeffcients: Array1<f64> = {
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

        AsianOption {
            strike: strike,
            model_size: model_size,
            payoff_coeffcients: payoff_coeffcients,
        }
    }
}

impl Option for AsianOption {
    fn payoff(&self, path: &Array2<f64>) -> f64 {
        path.mean_axis(Axis(0))
            .unwrap()
            .dot(&self.payoff_coeffcients)
            .sub(self.strike)
            .max(0.0)
    }
}
