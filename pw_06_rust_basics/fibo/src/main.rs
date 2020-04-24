fn main()
{
    for n in 0..=90 {
        
        println!("fibo({0}) = {1}", n, fibo(n));
    };
}

fn fibo(n: u64) -> u64
{
    let (mut first, mut second) = (0, 1);

    for _ in 0..n {

        let next = first + second;

        first = second;
        second = next;
    }

    first
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_fibo()
    {
        assert_eq!(fibo(0), 0);
        assert_eq!(fibo(1), 1);
        assert_eq!(fibo(2), 1);
        assert_eq!(fibo(6), 8);
        assert_eq!(fibo(10), 55);
        assert_eq!(fibo(81), 37_889_062_373_143_906);
        assert_eq!(fibo(90), 2_880_067_194_370_816_120);
    }
}
