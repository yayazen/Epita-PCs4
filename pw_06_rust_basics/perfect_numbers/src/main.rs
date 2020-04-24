use divisor_sum::divisor_sum;

fn main()
{
    for n in 1..=100_000 {

        if is_perfect_number(n) {

            println!("{}", n);
        }
    };
}

fn is_perfect_number(n: u64) -> bool
{
    divisor_sum(n) == n
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_is_perfect_number()
    {
        assert_eq!(is_perfect_number(1), false);
        assert_eq!(is_perfect_number(6), true);
        assert_eq!(is_perfect_number(28), true);
        assert_eq!(is_perfect_number(4096), false);
        assert_eq!(is_perfect_number(8_589_869_056), true);
        assert_eq!(is_perfect_number(137_438_691_328), true);
    }
}

