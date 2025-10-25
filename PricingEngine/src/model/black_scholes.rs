use crate::math::random::normal_vec;
use ndarray::{Array1, Array2};
use rand::rngs::ThreadRng;
use serde_json::Value;

pub struct BlackScholesModel {
    pub model_size: usize,           // nombre d'actifs du modèle
    pub interest_rate: f64,          // taux d'intérêt
    pub correlation: f64,            // paramètre de corrélation
    pub volatility: Array1<f64>,     // vecteur de volatilités
    pub spots: Array1<f64>,          // valeurs initiales des sous-jacents
    pub l: Array2<f64>,              // racine carrée de matrice de corrélation
    pub fixings_dates_number: usize, // fixings date Number N
    pub time_step: f64,              // time step = T / N
}

impl BlackScholesModel {
    pub fn new() -> Self {
        BlackScholesModel {
            model_size: 0,
            interest_rate: 0.0,
            correlation: 0.0,
            volatility: Array1::zeros(0),
            spots: Array1::zeros(0),
            l: Array2::zeros((0, 0)),
            fixings_dates_number: 1,
            time_step: 0.0,
        }
    }
}

impl BlackScholesModel {
    pub fn from_json(json: &Value) -> Self {
        let t = json["maturity"].as_f64().unwrap();
        let n = json["fixing dates number"].as_f64().unwrap();
        let time_step = t / n;

        let model_size = json["option size"].as_u64().unwrap() as usize;
        let interest_rate = json["interest rate"].as_f64().unwrap();
        let correlation = json["correlation"].as_f64().unwrap();

        // Volatility
        let volatility: Array1<f64> = {
            let arr = json["volatility"].as_array().unwrap();
            let mut v: Vec<f64> = arr.iter().map(|x| x.as_f64().unwrap()).collect();
            if v.len() == 1 && model_size > 1 {
                v = vec![v[0]; model_size];
            }
            Array1::from(v)
        };

        // Spots
        let spots: Array1<f64> = {
            let arr = json["spot"].as_array().unwrap();
            let mut v: Vec<f64> = arr.iter().map(|x| x.as_f64().unwrap()).collect();
            if v.len() == 1 && model_size > 1 {
                v = vec![v[0]; model_size];
            }
            Array1::from(v)
        };

        // Matrice de corrélation
        let mut l = Array2::<f64>::from_elem((model_size, model_size), correlation);
        for i in 0..model_size {
            l[(i, i)] = 1.0;
        }
        // À remplacer par une vraie décomposition de Cholesky si besoin
        // l = cholesky(&l);

        BlackScholesModel {
            model_size,
            interest_rate,
            correlation,
            volatility,
            spots,
            l,
            fixings_dates_number: n as usize,
            time_step,
        }
    }
}

impl BlackScholesModel {
    pub fn gen_path(&self, rng: &mut ThreadRng) -> Array2<f64> {
        let d = self.model_size;
        let r = self.interest_rate;
        let n = self.fixings_dates_number;

        let mut path = Array2::<f64>::from_elem((n, d), 0.);

        // Condition initiale
        path.row_mut(0).assign(&Array1::from(self.spots.clone()));

        for i in 1..n {
            // vecteur Gaussien i.i.d
            let g = Array1::from(normal_vec(d, rng, 0.0, 1.0));

            // vecteur corrélé z = L * g
            let z = self.l.dot(&g);

            for j in 0..d {
                let sigma = self.volatility[j];

                let drift = (r - 0.5 * sigma * sigma) * self.time_step;
                let diffusion = sigma * self.time_step.sqrt() * z[j];

                let facteur = (drift + diffusion).exp();

                path[[i, j]] = path[[i - 1, j]] * facteur;
            }
        }

        path
    }
}
