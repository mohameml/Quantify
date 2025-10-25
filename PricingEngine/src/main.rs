use ndarray::Array2;
use ndarray::array;
use pcpd::options::call::CallOption;
use pcpd::options::option::Option;
use pcpd::options::put::PutOption;

fn main() {
    let path: Array2<f64> = array![[90.0], [100.0]];

    let strike = 90.;

    let call = CallOption::new(strike);

    let put = PutOption::new(strike);

    let payoff_call = call.payoff(&path);
    println!(
        "The payoff of call option with strike {} is {}",
        strike, payoff_call
    );

    let payoff_put = put.payoff(&path);
    println!(
        "The payoff of put option with strike {} is {}",
        strike, payoff_put
    );
}
