//use ndarray::*;
// fn main() {
//     let model_size = 3;
//     let correlation = 1.23;

//     let mut l = Array2::<f64>::from_elem((model_size, model_size), correlation);

//     println!("l = {:?}", l);
// }

use ndarray::{Array2, Axis};

fn main() {
    // Exemple : matrice 3x2
    let a = Array2::from_shape_vec((3, 2), vec![1.0, 2.0, 3.0, 4.0, 5.0, 6.0]).unwrap();

    // Moyenne colonne par colonne (axis=0)
    let col_means = a.mean_axis(Axis(0)).unwrap();

    println!("Matrice:\n{:?}", a);
    println!("Moyenne des colonnes: {:?}", col_means);
}
