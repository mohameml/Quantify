use rand::rngs::ThreadRng;
use rand_distr::{Distribution, Normal};

pub fn normal_vec(n: usize, rng: &mut ThreadRng, mean: f64, std_dev: f64) -> Vec<f64> {
    let normal = Normal::new(mean, std_dev).unwrap();
    (0..n).map(|_| normal.sample(rng)).collect()
}
