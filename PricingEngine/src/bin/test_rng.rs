use rand::{rng, rngs::ThreadRng};
use rand_distr::{Distribution, Normal};

fn main() {
    let mut _rng = rng();

    let v_normal = normal_vec(10, &mut _rng, 0.0, 1.0);

    println!("v_normal : {:?}", v_normal);
}

fn normal_vec(n: usize, rng: &mut ThreadRng, mean: f64, std_dev: f64) -> Vec<f64> {
    let normal = Normal::new(mean, std_dev).unwrap();
    (0..n).map(|_| normal.sample(rng)).collect()
}
