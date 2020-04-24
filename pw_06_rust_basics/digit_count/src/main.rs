fn main()
{
    let mut n = 0;

    for _ in 0..=64 {
        
        println!("digit_count({0}) = {1}", n, digit_count(n));
        
        n = if n != 0 { n << 1 } else { 1 };
    };
}

fn digit_count(mut n: u64) -> u8
{
    let mut c = 1;

    loop {

        if {n /= 10; n == 0} {

            break c
        }

        c += 1;
    }
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_digit_count()
    {
        assert_eq!(digit_count(0), 1);
        assert_eq!(digit_count(1), 1);
        assert_eq!(digit_count(1 << 3), 1);
        assert_eq!(digit_count(1 << 6), 2);
        assert_eq!(digit_count(1 << 10), 4);
        assert_eq!(digit_count(1 << 24), 8);
        assert_eq!(digit_count(1 << 63), 19);
        assert_eq!(digit_count(u64::max_value()), 20);
    }
}
