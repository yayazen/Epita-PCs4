fn main()
{
    for n in 0..64 {
    
        println!("power_of_two({0}) = {1}", n, power_of_two(n));
    };
}

fn power_of_two(n: u8) -> u64
{
    1 << n
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_power_of_two()
    {
        assert_eq!(power_of_two(0), 1);
        assert_eq!(power_of_two(1), 2);
        assert_eq!(power_of_two(4), 16);
        assert_eq!(power_of_two(8), 256);
        assert_eq!(power_of_two(16), 65_536);
        assert_eq!(power_of_two(24), 16_777_216);
        assert_eq!(power_of_two(63), 9_223_372_036_854_775_808);
    }
}
 
