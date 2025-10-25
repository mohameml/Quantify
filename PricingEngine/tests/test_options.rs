use ndarray::Array2;
use ndarray::array;
use pcpd::options::call::CallOption;
use pcpd::options::option::Option;
use pcpd::options::put::PutOption;

#[test]
fn test_payoff_of_call_1() {
    let path: Array2<f64> = array![[90.0], [100.0]];

    let strike = 90.;

    let call = CallOption::new(strike);

    let payoff_call = call.payoff(&path);
    assert_eq!(payoff_call, 10.)
}

#[test]
fn test_payoff_of_call_2() {
    let path: Array2<f64> = array![[90.0], [100.0]];

    let strike = 110.;

    let call = CallOption::new(strike);

    let payoff_call = call.payoff(&path);
    assert_eq!(payoff_call, 0.)
}

#[test]
fn test_payoff_of_put_1() {
    let path: Array2<f64> = array![[90.0], [100.0]];

    let strike = 90.;

    let put = PutOption::new(strike);

    let payoff_call = put.payoff(&path);
    assert_eq!(payoff_call, 0.)
}

#[test]
fn test_payoff_of_put_2() {
    let path: Array2<f64> = array![[90.0], [100.0]];

    let strike = 110.;

    let put = PutOption::new(strike);

    let payoff_call = put.payoff(&path);
    assert_eq!(payoff_call, 10.)
}
